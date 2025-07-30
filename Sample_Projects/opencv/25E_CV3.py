'''
readme:
只有当canny后的框是一个完美的回字形时才能识别，属于不会误判也很难很难识别

'''


import cv2  # OpenCV库，用于图像处理
import numpy as np  # NumPy库，用于数值计算
import time  # 时间库
import struct
#from filterpy.kalman import KalmanFilter
#import matplotlib.pyplot as plt
# import serial  # 串口通信库
# 初始化串口
# 参数说明：'COM3'是串口号，115200是波特率，根据实际情况修改
# ser = serial.Serial('COM3', 115200)  

# 初始化摄像头
cap = cv2.VideoCapture(0)  
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)   # 设置图像宽度为320
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)  # 设置图像高度为240
cap.set(cv2.CAP_PROP_BRIGHTNESS, 0.2)    # 设置亮度，范围0-1，降低亮度
cap.set(cv2.CAP_PROP_AUTO_WB, 0)         # 关闭自动白平衡
# 关闭自动曝光（必须关闭才能手动设置）
cap.set(cv2.CAP_PROP_AUTO_EXPOSURE, 0)  # 0.25 或 0 表示手动曝光
# 调小曝光值（减少进光量）
cap.set(cv2.CAP_PROP_EXPOSURE, -8)  # 典型范围：-8（最暗）到 -1（较亮），或 0.1~0.001（秒）




while cap.isOpened():
    # 读取摄像头帧
    ret, frame = cap.read()
    if not ret:
        break
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    
    # 自适应阈值处理（突出黑色边框）
    thresh = cv2.adaptiveThreshold(
        gray, 255, 
        cv2.ADAPTIVE_THRESH_GAUSSIAN_C, 
        cv2.THRESH_BINARY_INV, 11, 2
    )
    '''
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    #hsv = cv2.LUT(hsv, lutSRaisen)             # 饱和度增大
    
    # 定义黑色的HSV范围
    lower_black = np.array([0, 0, 0])
    upper_black = np.array([179, 164, 229])
    
    thresh=cv2.inRange(hsv, lower_black, upper_black)
    '''
    # 去噪处理步骤
    # 1. 中值滤波去除椒盐噪声
    denoised = cv2.medianBlur(thresh, 5)
    
    # 2. 形态学开操作去除小噪点
    kernel_small = np.ones((3, 3), np.uint8)
    opened = cv2.morphologyEx(denoised, cv2.MORPH_OPEN, kernel_small)
    
    # 3. 形态学闭操作填充边缘缝隙
    kernel_close = np.ones((5, 5), np.uint8)
    closed = cv2.morphologyEx(opened, cv2.MORPH_CLOSE, kernel_close)
    
    # 4. 高斯模糊进一步平滑
    blurred = cv2.GaussianBlur(closed, (5, 5), 0)
    
    # 5. 连通域分析，去除面积过小的区域
    num_labels, labels, stats, centroids = cv2.connectedComponentsWithStats(blurred, connectivity=8)
    
    # 创建清洁后的图像
    cleaned = np.zeros_like(blurred)
    min_area = 100  # 最小连通域面积阈值，可根据需要调整
    
    for i in range(1, num_labels):  # 跳过背景标签0
        if stats[i, cv2.CC_STAT_AREA] >= min_area:
            cleaned[labels == i] = 255
    
    cv2.imshow('thresh_original', thresh)
    cv2.imshow('cleaned', cleaned)
    cv2.imshow('blurred', blurred)
    
    # Canny边缘检测
    edges = cv2.Canny(cleaned, 30, 100)
    cv2.imshow('edges', edges)
    # 查找所有轮廓（包括层级关系）
    contours, hierarchy = cv2.findContours(
        edges, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE
    )
    
    # 存储有效的内外边框对
    border_pairs = []

    for i, cnt in enumerate(contours):
    # 近似多边形（筛选四边形）
        epsilon = 0.02 * cv2.arcLength(cnt, True)
        approx = cv2.approxPolyDP(cnt, epsilon, True)
    
        if len(approx) == 4:  # 确保是四边形
            # 检查是否有子轮廓（内边框）
            if hierarchy[0][i][2] != -1:
                child_idx = hierarchy[0][i][2]
                child_cnt = contours[child_idx]
                child_approx = cv2.approxPolyDP(child_cnt, epsilon, True)
            
                if len(child_approx) == 4:
                    # 计算内外边框面积
                    area_outer = cv2.contourArea(approx)
                    area_inner = cv2.contourArea(child_approx)
                    
                    # 内边框面积控制 - 只保留面积大于阈值的内边框
                    min_inner_area = 210  # 内边框最小面积阈值，可根据需要调整
                    
                    # 面积比值控制 - 内外边框面积比值必须在合理范围内
                    area_ratio = area_inner / area_outer
                    min_area_ratio = 0.8  # 最小面积比值阈值，可根据需要调整
                    max_area_ratio = 0.95  # 最大面积比值阈值
                    
                    # 长宽比控制 - 确保是横向长方形
                    outer_rect = cv2.minAreaRect(approx)
                    w, h = outer_rect[1]
                    
                    # 确保宽度大于高度（横向长方形）
                    if w < h:
                        w, h = h, w  # 交换宽高，确保w是长边
                    
                    aspect_ratio = w / h  # 长宽比
                    min_aspect_ratio = 1.2  # 最小长宽比，确保是长方形
                    max_aspect_ratio = 2.2  # 最大长宽比，避免过于细长
                    
                    if (area_inner >= min_inner_area and 
                        min_area_ratio <= area_ratio <= max_area_ratio and
                        min_aspect_ratio <= aspect_ratio <= max_aspect_ratio and
                        w > h):  # 确保是横向长方形
                        border_pairs.append((approx, child_approx))
        
        
        # 已知条件：边框物理宽度1.8cm，图像中1cm = N像素
    #px_per_cm = 50  # 根据实际校准（例如用标定板测量）


    #接下来是清洗操作
    resonable_border_pairs=[]
    for outer, inner in border_pairs:
        cv2.drawContours(frame, [outer], -1, (255, 0, 0), 2) 
        cv2.drawContours(frame, [inner], -1, (255, 0, 0), 2) 
    # 计算内外边框间距（取四边平均）
        '''
        dist = []
        for i in range(4):
            pt_outer = outer[i][0]
            pt_inner = inner[i][0]
            dist.append(np.linalg.norm(pt_outer - pt_inner))
    
        avg_dist_px = np.mean(dist)
        avg_dist_cm = avg_dist_px / px_per_cm
        '''
    # 验证是否为1.8cm边框
        outer_center = np.mean(outer, axis=0)[0]
        inner_center = np.mean(inner, axis=0)[0]
        center_dist = np.linalg.norm(outer_center - inner_center)
    
        if center_dist > 20:  # 中心偏移超过10像素则剔除
            print('not same center')
            pass
        else :
            outer_rect = cv2.minAreaRect(outer)
            w, h = outer_rect[1]
            aspect_ratio = max(w, h) / min(w, h)
            if aspect_ratio > 1.3:  # 非正方形则剔除
                pass
        resonable_border_pairs.append((outer,inner))
        
    if(resonable_border_pairs):
        cv2.drawContours(frame, [outer], -1, (0, 255, 0), 2)  # 外边框绿色
        cv2.drawContours(frame, [inner], -1, (0, 0, 255), 2)  # 内边框红色
    else:
        print("no border pairs found")
    
    # 显示最终结果
    cv2.imshow("Detection", frame)
    key = cv2.waitKey(1)
    if key == 32:
        break  
    if key == 27:
        break  
    

# 释放资源
cap.release()
cv2.destroyAllWindows()
