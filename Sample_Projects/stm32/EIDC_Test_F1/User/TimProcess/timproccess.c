#include "timproccess.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM1) // 判断是哪个定时器触发的中断
    {
        
    }
}