import cv2  # OpenCV库，用于图像处理
import numpy as np  # NumPy库，用于数值计算
import time  # 时间库
import struct
import serial
import serial.tools.list_ports
serial_port = serial.Serial("/dev/ttyAMA0", 115200, timeout=0.5)
# serial_port_state = serial_port.is_open

# 初始化摄像头
cap = cv2.VideoCapture(0)  
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)   # 设置图像宽度
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)  # 设置图像高度
cap.set(cv2.CAP_PROP_BRIGHTNESS, 0.2)    # 设置亮度，范围0-1，降低亮度
cap.set(cv2.CAP_PROP_AUTO_WB, 0)         # 关闭自动白平衡
cap.set(cv2.CAP_PROP_AUTO_EXPOSURE, 0)   # 关闭自动曝光
cap.set(cv2.CAP_PROP_EXPOSURE, -8)       # 调小曝光值

while cap.isOpened():
    # 读取摄像头帧
    ret, frame = cap.read()
    if not ret:
        break
    
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    
    # 自适应阈值处理
    thresh = cv2.adaptiveThreshold(
        gray, 255, 
        cv2.ADAPTIVE_THRESH_GAUSSIAN_C, 
        cv2.THRESH_BINARY_INV, 11, 2
    )
    
    # 去噪处理
    denoised = cv2.medianBlur(thresh, 5)
    
    # 形态学操作 - 先开操作去小噪点，再闭操作连接间隙
    kernel_small = np.ones((3, 3), np.uint8)
    opened = cv2.morphologyEx(denoised, cv2.MORPH_OPEN, kernel_small)
    
    kernel_close = np.ones((7, 7), np.uint8)
    closed = cv2.morphologyEx(opened, cv2.MORPH_CLOSE, kernel_close)
    
    # 连通域分析，过滤小区域
    num_labels, labels, stats, centroids = cv2.connectedComponentsWithStats(closed, connectivity=8)
    
    cleaned = np.zeros_like(closed)
    min_area = 220  # 候选矩形最小面积
    
    for i in range(1, num_labels):
        if stats[i, cv2.CC_STAT_AREA] >= min_area:
            cleaned[labels == i] = 255
    
    # 边缘检测
    edges = cv2.Canny(cleaned, 50, 150)
    
    # 查找轮廓
    contours, hierarchy = cv2.findContours(edges, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    
    # 候选矩形列表
    candidate_rectangles = []
    
    # 第一步：矩形特征检测（主要条件）
    for i, cnt in enumerate(contours):
        # 轮廓近似
        epsilon = 0.02 * cv2.arcLength(cnt, True)
        approx = cv2.approxPolyDP(cnt, epsilon, True)
        
        # 检查是否为四边形
        if len(approx) == 4:
            area = cv2.contourArea(approx)
            
            # 面积过滤
            if area < 250:  # 最小面积阈值
                continue
                
            # 长宽比检测
            rect = cv2.minAreaRect(approx)
            w, h = rect[1]
            if w < h:
                w, h = h, w  # 确保w是长边
            
            aspect_ratio = w / h
            
            # 矩形特征判断
            if 1.1 <= aspect_ratio <= 2.1:  # 长宽比范围
                # 计算轮廓的凸性
                hull = cv2.convexHull(approx)
                hull_area = cv2.contourArea(hull)
                solidity = area / hull_area if hull_area > 0 else 0
                
                # 矩形度检测
                if solidity > 0.85:  # 凸性阈值
                    candidate_rectangles.append({
                        'contour': approx,
                        'area': area,
                        'aspect_ratio': aspect_ratio,
                        'solidity': solidity,
                        'center': rect[0],
                        'hierarchy_idx': i
                    })
    
    # 第二步：多层边框辅助验证
    valid_targets = []
    
    for candidate in candidate_rectangles:
        idx = candidate['hierarchy_idx']
        
        # 检查是否有子轮廓（内边框）
        has_inner_border = False
        inner_border_count = 0
        
        if hierarchy is not None and hierarchy[0][idx][2] != -1:
            # 遍历所有子轮廓
            child_idx = hierarchy[0][idx][2]
            while child_idx != -1:
                child_cnt = contours[child_idx]
                child_epsilon = 0.02 * cv2.arcLength(child_cnt, True)
                child_approx = cv2.approxPolyDP(child_cnt, child_epsilon, True)
                
                if len(child_approx) == 4:
                    child_area = cv2.contourArea(child_approx)
                    area_ratio = child_area / candidate['area']
                    
                    # 内边框面积比例合理
                    if 0.3 <= area_ratio <= 0.9:
                        has_inner_border = True
                        inner_border_count += 1
                        break
                
                child_idx = hierarchy[0][child_idx][0]  # 下一个兄弟轮廓
        
        # 评分系统
        score = 0
        
        # 矩形特征评分（主要）
        if 1.2 <= candidate['aspect_ratio'] <= 1.7:  # 理想长宽比
            score += 30
        elif 1.1 <= candidate['aspect_ratio'] <= 2.3:  # 可接受长宽比
            score += 20
            
        # 面积阈值判断
        if candidate['area'] < 4000:  # 面积小于500像素的目标不予考虑
            continue
            
        if candidate['solidity'] > 0.9:  # 高矩形度
            score += 25
        elif candidate['solidity'] > 0.85:  # 中等矩形度
            score += 15
            
        if candidate['area'] > 7+000:  # 面积加分
            score += 20
        elif candidate['area'] > 5000:
            score += 10
            
        # 多层边框评分（辅助）
        if has_inner_border:
            score += 25  # 有内边框大幅加分
            
        # 颜色差异检测（如果有内边框）
        if has_inner_border:
            try:
                # 创建外轮廓mask
                outer_mask = np.zeros(gray.shape, dtype=np.uint8)
                cv2.fillPoly(outer_mask, [candidate['contour']], 255)
                
                # 找到内轮廓
                child_idx = hierarchy[0][idx][2]
                if child_idx != -1:
                    inner_cnt = contours[child_idx]
                    inner_mask = np.zeros(gray.shape, dtype=np.uint8)
                    cv2.fillPoly(inner_mask, [inner_cnt], 255)
                    
                    # 计算边框区域
                    border_mask = cv2.bitwise_and(outer_mask, cv2.bitwise_not(inner_mask))
                    
                    # 计算色差
                    if cv2.countNonZero(border_mask) > 0 and cv2.countNonZero(inner_mask) > 0:
                        border_mean = cv2.mean(gray, mask=border_mask)[0]
                        inner_mean = cv2.mean(gray, mask=inner_mask)[0]
                        color_diff = abs(border_mean - inner_mean)
                        
                        if color_diff > 30:  # 明显色差
                            score += 20
                        elif color_diff > 15:  # 轻微色差
                            score += 10
            except:
                pass  # 忽略计算错误
        
        # 根据评分决定是否为有效目标
        if score >= 50:  # 评分阈值
            candidate['score'] = score
            valid_targets.append(candidate)
    
    # 按评分排序，取最佳目标
    valid_targets.sort(key=lambda x: x['score'], reverse=True)
    
    # 打印得分最高的轮廓中心坐标
    
    
    # 绘制检测结果
    for i, target in enumerate(valid_targets[:3]):  # 最多显示3个最佳目标
        color = [(0, 255, 0), (255, 0, 0), (0, 0, 255)][i]  # 绿、红、蓝
        
        # 绘制外轮廓
        cv2.drawContours(frame, [target['contour']], -1, color, 3)
        
        # 绘制中心点和信息
        center = tuple(map(int, target['center']))
        cv2.circle(frame, center, 8, color, -1)
        
        # 显示评分和信息
        text = f"Score:{target['score']:.0f} AR:{target['aspect_ratio']:.2f} Area:{target['area']:.0f}"
        cv2.putText(frame, text, (center[0]-50, center[1]-25), 
                   cv2.FONT_HERSHEY_SIMPLEX, 0.6, color, 2)
        
        # 如果有内边框，也绘制出来
        idx = target['hierarchy_idx']
        if hierarchy is not None and hierarchy[0][idx][2] != -1:
            child_idx = hierarchy[0][idx][2]
            if child_idx != -1:
                cv2.drawContours(frame, [contours[child_idx]], -1, (255, 255, 0), 2)
    
    # 显示处理结果
    cv2.imshow('thresh', thresh)
    cv2.imshow('cleaned', cleaned)
    cv2.imshow('edges', edges)
    cv2.imshow('Detection', frame)
    
    if valid_targets:
        best_target = valid_targets[0]
        center_x, center_y = best_target['center']
        print(f"最高得分目标中心坐标: ({center_x:.1f}, {center_y:.1f}), 得分: {best_target['score']:.0f}")
        xh = int(center_x*10)>>8  #x高位
        xl = int(center_x*10)&0xFF  #x低位
        yh = int(center_y * 10) >> 8  # y高位
        yl = int(center_y * 10) & 0xFF  # y低位
        print(center_x,center_y)
        pack_data = struct.pack(
            ">BB4BBB",
            0xAA,
            0xFF,
            xh,
            xl,
            yh,     
            yl,
            0xFF,
            0xAA,
        )
        serial_port.write(pack_data)
        print(f"发送数据: {xh}, {xl}, {yh}, {yl}")
    
    key = cv2.waitKey(1)
    if key == 32 or key == 27:  # 空格或ESC退出
        break

# 释放资源
cap.release()
cv2.destroyAllWindows()
