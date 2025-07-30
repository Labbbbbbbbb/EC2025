# 导入必要的库
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
cap = cv2.VideoCapture(1)  
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 320)   # 设置图像宽度为320
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 240)  # 设置图像高度为240
cap.set(cv2.CAP_PROP_BRIGHTNESS, 1.0)    # 设置亮度，范围0-1
cap.set(cv2.CAP_PROP_AUTO_WB, 0)         # 关闭自动白平衡
# 关闭自动曝光（必须关闭才能手动设置）
cap.set(cv2.CAP_PROP_AUTO_EXPOSURE, 0.25)  # 0.25 或 0 表示手动曝光
# 调小曝光值（减少进光量）
cap.set(cv2.CAP_PROP_EXPOSURE, -6)  # 典型范围：-8（最暗）到 -1（较亮），或 0.1~0.001（秒）

# 定义感兴趣区域(ROI)
# 格式：(x起始坐标, y起始坐标, 宽度, 高度)
#ROI = (0, 0, 160, 120)

def detect_black_border(img):
    
    # 2. 灰度化 + 高斯模糊
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    blurred = cv2.GaussianBlur(gray, (5, 5), 0)
    
    # 3. Canny边缘检测
    edges = cv2.Canny(blurred, 30, 100)  # 阈值可调整
    
    # 4. 查找轮廓
    contours, _ = cv2.findContours(edges, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    
    # 5. 筛选轮廓（按面积和形状）
    max_area = 0
    best_contour = None
    
    for cnt in contours:
        area = cv2.contourArea(cnt)
        if area < 1000:  # 忽略小噪点
            continue
        
        # 近似多边形（减少轮廓点数）
        epsilon = 0.02 * cv2.arcLength(cnt, True)
        approx = cv2.approxPolyDP(cnt, epsilon, True)
        
        # 筛选四边形（矩形边框）
        if len(approx) == 4:
            if area > max_area:
                max_area = area
                best_contour = approx
    
    # 6. 绘制结果
    if best_contour is not None:
        cv2.drawContours(img, [best_contour], -1, (0, 255, 0), 3)  # 绿色边框
        print("检测到黑色边框！")
    else:
        print("未检测到有效边框！")



while cap.isOpened():
    # 读取摄像头帧
    ret, frame = cap.read()
    if not ret:
        break
    
    # 创建一个用于绘制的图像副本
    display_frame = frame.copy()
    
    # 检测黑色矩形
    detect_black_border(display_frame)
    
    

    # 显示最终结果
    cv2.imshow("Detection", display_frame)
    
    key = cv2.waitKey(1)
    if key == 32:
        break  
    if key == 27:
        break  
    

# 释放资源
cap.release()
cv2.destroyAllWindows()
