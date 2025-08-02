#include "HWT_user.h"
#include "my_uart.h"
float hwt_initial_angle = 0;
float hwt_last_angle    = 0;
float hwt_current_angle = 0;
uint16_t FT_target_angle = 0;
uint8_t previous_edge    = 0;
uint8_t current_edge    = 0;
float edge_initial_angle[4];
uint32_t edge_start_time[4];
uint32_t tick_tim2_ms;
float debug_kf = 0.0f; // 用于调试卡尔曼滤波器的变量
void RotateWithHWT()
{
    static uint8_t init = 0;
    if(init==0) 
    {
        hwt_initial_angle=fAngle[2];
        hwt_current_angle = fAngle[2];
        FT_target_angle = ReadPos(2); // 读取初始角度
        init = 1;
    }
    hwt_last_angle = hwt_current_angle;
    hwt_current_angle = fAngle[2];
    /*calculate_the_turn_of_hwt*/
    float turn = hwt_current_angle - hwt_last_angle; //fangle变小说明向左，和FT一样
    if(turn < -180) turn += 360; // 说明经历了180到-180的转变
    if(turn > 180) turn -= 360; // 说明经历了-180到180的转变

    uint16_t FT_turn = (int16_t)(turn / 360 * 4095);
    FT_target_angle -= FT_turn; // 计算出FT的目标角度//负号表示补偿车身运动
    if(FT_target_angle >=65535/2 ) FT_target_angle = -(65535-FT_target_angle); // 确保角度在0-65535范围内
    if(FT_target_angle < 65535/2)  // 
        WritePos(2, FT_target_angle, 10000, 1000); // FT目标角度
}


/****Kalman_define****/



// 反三角函数模型（根据实际赛题场地调整参数）
float predict_yaw(float v, float t, uint8_t edge)
{
    float x = 0.0f;
    switch (edge) {
        case 0:
            x = -1 * (t * v - 0.5f) / 0.5f;
            break; // 右边框
        case 1:
            x = -1 * (t * v + 0.5f) / 0.5f;
            break; // 左边框
        case 2:
            x = (t * v - 0.5f) / 0.5f;
            break; // 上边框
        case 3:
            x = (t * v + 0.5f) / 0.5f;
            break; // 下边框
        default:
            x = 0.0f;
            break;
    }
    if (x > 0.999f) x = 0.999f;
    if (x < -0.999f) x = -0.999f;
    return atan(1 / x) * 180.0f / 3.1415926f; // 理论yaw角度（度）
}

// 初始化卡尔曼滤波器
void yaw_kalman_init(YawKalman *kf, float init_yaw, float q, float r)
{
    kf->x         = init_yaw;
    kf->P         = 100.0f;
    kf->Q         = q; // 过程噪声（如0.1f）
    kf->R         = r; // 观测噪声（如10.0f）
    kf->K         = 0.0f;
    kf->v         = 0.6f;
    kf->t         = 0.0f;
    kf->edge      = 0;
    kf->model_yaw = init_yaw;
}

// 卡尔曼滤波主流程
float yaw_kalman_update(YawKalman *kf, float v, float t, uint8_t edge, float cam_yaw)
{
    // 1. 预测步骤
    kf->v         = v;
    kf->t         = t;
    kf->edge      = edge;
    kf->model_yaw = predict_yaw(v, t, edge); // 预测量
    kf->P += kf->Q;

    // 2. 观测步骤
    // cam_yaw为摄像头测得的yaw角度

    // 3. 卡尔曼增益
    kf->K = kf->P / (kf->P + kf->R);

    // 4. 状态融合
    // 预测量和观测量加权融合
    float y = cam_yaw - kf->model_yaw; // 创新
    kf->x   = kf->model_yaw + kf->K * y;

    // 5. 协方差更新
    kf->P = (1.0f - kf->K) * kf->P;

    return kf->x; // 输出最优yaw角度
}

void edge_state_update(void)
{
    // 根据摄像头yaw角度判断当前边框

    if(previous_edge==0)
    {
        if(hwt_current_angle-hwt_initial_angle <-80 && hwt_current_angle-hwt_initial_angle-360 < -80)
        {
            edge_start_time[1]= tick_tim2_ms;; // 记录边框开始时间
            edge_initial_angle[0] = hwt_current_angle;
            current_edge= 1; // 左边框
        }      
    }else if(previous_edge==1)
    {
        if(hwt_current_angle-edge_initial_angle[1] <-80 && hwt_current_angle-edge_initial_angle[1]-360 < -80)
        {
            edge_initial_angle[1] = hwt_current_angle;
            edge_start_time[2]    = tick_tim2_ms; // 记录边框开始时间

            current_edge= 2; // 上边框
        }
    }else if(previous_edge==2)
    {
        if(hwt_current_angle-edge_initial_angle[2] <-80 && hwt_current_angle-edge_initial_angle[2] -360<-80)
        {
            edge_initial_angle[2] = hwt_current_angle;
            edge_start_time[3]    = tick_tim2_ms;; // 记录边框开始时间

            current_edge= 3; // 下边框
        }
    }else if(previous_edge==3)
    {   
        if(hwt_current_angle-edge_initial_angle[3] <-80 && hwt_current_angle-edge_initial_angle[3] -360<-80)
        {
            edge_initial_angle[3] = hwt_current_angle;
            edge_start_time[0]    = tick_tim2_ms;; // 记录边框开始时间

            current_edge= 0; // 右边框
        }
    }
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        tick_tim2_ms++;
    }
    if (htim->Instance == TIM3)
        if (htim->Instance == TIM3) // for kalman
        {
                // 每10ms调用一次

            float v       = 0.6f;                                                         // 小车速度
            float t       = (edge_start_time[current_edge] - tick_tim2_ms) * 1.0 / 1000; // 当前边框运行时间(s)
            edge_state_update();
            uint8_t edge = current_edge; // 当前边框编号
            previous_edge = edge;
            float cam_yaw = 0;

            if(rx_data[0]!=0)
                cam_yaw = (-5 * (rx_data[0] - 302));                                    // 摄像头测得yaw角度
            
            float best_yaw = yaw_kalman_update(&yaw_filter, v, t, edge, cam_yaw);
            debug_kf = best_yaw; // 用于调试卡尔曼滤波器的变量
            FT_target_angle += (int16_t)(best_yaw / 360 * 4095); // 更新FT目标角度
        }
}