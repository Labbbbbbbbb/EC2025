# Untitled - By: zyt - Sun Apr 6 2025

import sensor, image, time

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)


sensor.set_auto_gain(False)
sensor.skip_frames(20)
sensor.set_auto_exposure(False, exposure_us=1400)
sensor.set_auto_whitebal(False)

red_threshold = (30, 100, 15, 127, -40, 127)  # 红色激光笔的颜色阈值
green_threshold = (9, 86, 10, 71, -2, 66)    # 绿色十字的颜色阈值

def find_blob_center(image_,threshold):        #返回光点中心坐标，img为图片对象
    blobs = image_.find_blobs([threshold])
    if blobs:
        b = blobs[0]
        cx = b.cx()
        cy = b.cy()
        return cx, cy
    return None, None
# 移动平均滤波窗口大小
window_size = 3
corner_history = []
avg_corner1_x=0
avg_corner1_y=0
avg_corner2_x=0
avg_corner2_y=0
avg_corner3_x=0
avg_corner3_y=0
avg_corner4_x=0
avg_corner4_y=0
def find_rect_corner(image_)
    for r in img.find_rects(threshold = 10000):
        # 判断矩形边长是否符合要求
        if r.w() > 20 and r.h() > 20:
            # 在屏幕上框出矩形
            img.draw_rectangle(r.rect(), color = (255, 0, 0), thickness = 2)
            # 获取矩形角点位置
            corner = r.corners()

            # 添加角点到历史列表
            corner_history.append(corner)

            # 保持历史列表大小不超过窗口大小
            if len(corner_history) > window_size:
                corner_history.pop(0)

            # 计算移动平均角点位置
            avg_corners = [
                (sum([corner[i][0] for corner in corner_history]) / len(corner_history),
                 sum([corner[i][1] for corner in corner_history]a) / len(corner_history))
                for i in range(4)]

            # 在屏幕上圈出移动平均的角点
            for avg_corner in avg_corners:
                img.draw_circle(int(avg_corner[0]), int(avg_corner[1]), 5, color=(0, 255, 0), thickness=2, fill=False)

            # 打印移动平均角点坐标
            avg_corner1 = avg_corners[0]
            avg_corner2 = avg_corners[1]
            avg_corner3 = avg_corners[2]
            avg_corner4 = avg_corners[3]

            avg_corner1_x = int(avg_corner1[0])
            avg_corner1_y = int(avg_corner1[1])

            avg_corner2_x = int(avg_corner2[0])
            avg_corner2_y = int(avg_corner2[1])

            avg_corner3_x = int(avg_corner3[0])
            avg_corner3_y = int(avg_corner3[1])

            avg_corner4_x = int(avg_corner4[0])
            avg_corner4_y = int(avg_corner4[1])



clock = time.clock()

while(True):
    clock.tick()
    img = sensor.snapshot().lens_corr(strength=1.6, zoom=1.0)

    # 检测红色激光笔的位置
    laser_x, laser_y = find_blob_center(green_threshold)

    if laser_x is not None and laser_y is not None:
            # 绘制绿色十字来表示激光笔的位置
        img.draw_cross(laser_x, laser_y, color=(0, 255, 128))
    find_rect_corner(img)
    print("Moving Avg Corner 1 (x, y):", avg_corner1_x, avg_corner1_y)
    #img = img.to_rgb565()  #节约内存,但是此处没必要因为本来就是
    #print(clock.fps())



