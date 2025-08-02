import cv2  # OpenCV库，用于图像处理
import numpy as np  # NumPy库，用于数值计算
import time  # 时间库
import struct
import serial
import serial.tools.list_ports

# FPS计算相关变量
fps_counter = 0
fps_start_time = time.time()
fps_display_interval = 30  # 每30帧显示一次FPS

serial_port = serial.Serial("/dev/ttyAMA0", 115200, timeout=0.5)
# serial_port_state = serial_port.is_open

# 初始化摄像头
cap = None
while cap is None or not cap.isOpened():
    for device_id in [0, 1, 2]:
        print(f"尝试打开摄像头设备 {device_id}...")
        cap = cv2.VideoCapture(device_id)
        if cap.isOpened():
            print(f"成功打开摄像头设备 {device_id}")
            break
        else:
            print(f"摄像头设备 {device_id} 打开失败")
            cap.release()
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
            
            # 计算外边框矩形的准确中心坐标（基于轮廓顶点）
            moments = cv2.moments(approx)
            if moments['m00'] != 0:
                center_x = moments['m10'] / moments['m00']
                center_y = moments['m01'] / moments['m00']
                outer_center = (center_x, center_y)
            else:
                # 备用方法：直接计算顶点平均值
                outer_center = (np.mean(approx[:, 0, 0]), np.mean(approx[:, 0, 1]))
            
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
                        'center': outer_center,  # 使用外边框的真实中心
                        'hierarchy_idx': i
                    })
    
    # 第二步：多层边框辅助验证
    valid_targets = []
    
    for candidate in candidate_rectangles:
        idx = candidate['hierarchy_idx']
        
        # 检查是否有子轮廓（内边框）
        has_inner_border = False
        inner_border_count = 0
        valid_inner_rectangles = []
        
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
                        # 检查内边框的矩形特征
                        child_rect = cv2.minAreaRect(child_approx)
                        child_w, child_h = child_rect[1]
                        if child_w < child_h:
                            child_w, child_h = child_h, child_w
                        
                        child_aspect_ratio = child_w / child_h
                        
                        # 计算内边框的矩形度
                        child_hull = cv2.convexHull(child_approx)
                        child_hull_area = cv2.contourArea(child_hull)
                        child_solidity = child_area / child_hull_area if child_hull_area > 0 else 0
                        
                        # 只有当内边框也是规整的矩形时才认为有效
                        if (1.1 <= child_aspect_ratio <= 2.5 and  # 内边框长宽比合理
                            child_solidity > 0.85 and             # 内边框矩形度高
                            child_area > 200):                    # 内边框面积足够大
                            has_inner_border = True
                            inner_border_count += 1
                            valid_inner_rectangles.append(child_approx)
                            break
                
                child_idx = hierarchy[0][child_idx][0]  # 下一个兄弟轮廓
        
        # 评分系统
        score = 0
        
        # 矩形特征评分（主要）
        if 1.2 <= candidate['aspect_ratio'] <= 2.0:  # 理想长宽比
            score += 20
        elif 1.1 <= candidate['aspect_ratio'] <= 2.3:  # 可接受长宽比
            score += 10
            
        # 面积阈值判断
        if candidate['area'] < 4000:  # 面积小于500像素的目标不予考虑
            continue
            
        if candidate['solidity'] > 0.9:  # 高矩形度
            score += 25
        elif candidate['solidity'] > 0.85:  # 中等矩形度
            score += 15
            
        if candidate['area'] > 7000:  # 面积加分
            score += 20
        elif candidate['area'] > 5000:
            score += 10
            
        # 多层边框评分（辅助）
        if has_inner_border:
            score += 25  # 有内边框大幅加分
            
        # 内部区域颜色检测（所有候选对象的必要条件）
        inner_region_valid = False  # 内部区域验证标志
        
        try:
            # 创建外轮廓mask来检测内部区域
            outer_mask = np.zeros(gray.shape, dtype=np.uint8)
            cv2.fillPoly(outer_mask, [candidate['contour']], 255)
            
            # 检查外轮廓内部区域的整体灰度
            if cv2.countNonZero(outer_mask) > 0:
                # 灰度图检测内部亮度
                inner_mean_gray = cv2.mean(gray, mask=outer_mask)[0]
                
                # 内部区域验证条件（必须满足）
                if inner_mean_gray > 120:  # 内部区域较亮（主要是白色）
                    inner_region_valid = True
                    score += 15  # 内部颜色符合要求加分
                    #print(f"内部区域验证通过: 整体亮度={inner_mean_gray:.1f}")
                else:
                    pass
                    
        except Exception as e:
        
            pass  # 忽略计算错误
        
        # 如果有内边框，进行额外的精细检测
        if has_inner_border and valid_inner_rectangles and inner_region_valid:
            try:
                # 使用第一个有效的内轮廓进行精细检测
                inner_approx = valid_inner_rectangles[0]
                inner_mask = np.zeros(gray.shape, dtype=np.uint8)
                cv2.fillPoly(inner_mask, [inner_approx], 255)
                #cv2.imshow('innermask',inner_mask)
                
                # 计算边框区域
                border_mask = cv2.bitwise_and(outer_mask, cv2.bitwise_not(inner_mask))
                
                # 精细的内部区域检测（只检测内边框以内）
                if cv2.countNonZero(inner_mask) > 0:
                    inner_fine_gray = cv2.mean(gray, mask=inner_mask)[0]
                    
                    # 彩色图检测红色圆线
                    frame_hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
                    
                    # 红色HSV范围
                    lower_red1 = np.array([0, 50, 50])
                    upper_red1 = np.array([10, 255, 255])
                    lower_red2 = np.array([170, 50, 50])
                    upper_red2 = np.array([180, 255, 255])
                    
                    # 创建红色mask
                    red_mask1 = cv2.inRange(frame_hsv, lower_red1, upper_red1)
                    red_mask2 = cv2.inRange(frame_hsv, lower_red2, upper_red2)
                    red_mask = cv2.bitwise_or(red_mask1, red_mask2)
                    
                    # 只检测内部区域的红色
                    inner_red_mask = cv2.bitwise_and(red_mask, inner_mask)
                    
                    # 计算红色比例
                    inner_total_pixels = cv2.countNonZero(inner_mask)
                    inner_red_pixels = cv2.countNonZero(inner_red_mask)
                    red_ratio = inner_red_pixels / inner_total_pixels if inner_total_pixels > 0 else 0
                    
                    # 精细检测红色比例
                    if red_ratio <= 0.15:
                        score += 10  # 红色比例合理额外加分
                        #print(f"精细检测通过: 内边框亮度={inner_fine_gray:.1f}, 红色比例={red_ratio:.3f}")
                
                # 边框颜色差异检测
                if cv2.countNonZero(border_mask) > 0 and cv2.countNonZero(inner_mask) > 0:
                    border_mean = cv2.mean(gray, mask=border_mask)[0]
                    inner_mean = cv2.mean(gray, mask=inner_mask)[0]
                    color_diff = abs(border_mean - inner_mean)
                    
                    if color_diff > 30:  # 明显色差
                        score += 20
                    elif color_diff > 15:  # 轻微色差
                        score += 10
                        
            except Exception as e:
                #print(f"精细检测错误: {e}")
                pass
        
        # 根据评分决定是否为有效目标
        # 重要：必须同时满足评分和内部整体亮度条件
        if score >= 50 and inner_region_valid:  # 所有对象都必须通过内部亮度检测
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
    #cv2.imshow('thresh', thresh)
    #cv2.imshow('cleaned', cleaned)
    #cv2.imshow('edges', edges)
    #cv2.imshow('Detection', frame)
    
    if valid_targets:
        best_target = valid_targets[0]
        center_x, center_y = best_target['center']
        #print(f"最高得分目标中心坐标: ({center_x:.1f}, {center_y:.1f}), 得分: {best_target['score']:.0f}")
        xh = int(center_x)>>8  #x高位
        xl = int(center_x)&0xFF  #x低位
        yh = int(center_y) >> 8  # y高位
        yl = int(center_y) & 0xFF  # y低位
        print(xh*256+xl,yh*256+yl)
    else :
        xh=0
        xl=0
        yh=0
        yl=0
        
    pack_data = struct.pack(
        ">BB4BBB",
        0xAA,
        0xBB,
        0xCC,
        0xDD,
        xh,
        xl,
        yh,     
        yl
            
    )
    serial_port.write(pack_data)
    #print(f"发送数据: {xh}, {xl}, {yh}, {yl}")

    # FPS计算和显示
    fps_counter += 1
    if fps_counter >= fps_display_interval:
        current_time = time.time()
        elapsed_time = current_time - fps_start_time
        fps = fps_counter / elapsed_time
        print(f"FPS: {fps:.2f}")
        
        # 重置计数器
        fps_counter = 0
        fps_start_time = current_time

    key = cv2.waitKey(1)
    if key == 32 or key == 27:  # 空格或ESC退出
        break

# 释放资源
cap.release()
cv2.destroyAllWindows()
