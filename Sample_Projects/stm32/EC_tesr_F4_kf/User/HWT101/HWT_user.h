
#include "stm32f407xx.h"
#include "HWT101CT_sdk.h"
#include "SCServo.h"

// 云台yaw轴卡尔曼滤波器结构体
typedef struct {
    float x;         // 当前yaw角度估计
    float P;         // 协方差
    float Q;         // 过程噪声
    float R;         // 观测噪声
    float K;         // 卡尔曼增益
    float v;         // 小车速度
    float t;         // 当前边框运行时间
    uint8_t edge;    // 当前所在边框编号
    float model_yaw; // 反三角函数预测yaw
} YawKalman;
#include <math.h>
extern uint16_t FT_target_angle ;
void RotateWithHWT();
extern float debug_kf;
extern YawKalman yaw_filter;
extern uint8_t previous_edge ;
extern uint8_t current_edge ;
extern float edge_initial_angle[4];
void yaw_kalman_init(YawKalman *kf, float init_yaw, float q, float r);
