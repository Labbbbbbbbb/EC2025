/*
 * @Author: ZYT
 * @Date: 2025-07-28 15:39:06
 * @LastEditors: ZYT
 * @LastEditTime: 2025-07-28 15:42:15
 * @FilePath: \EC_tesr_F4\User\HWT101\bsp_delay.h
 * @Brief: 
 * 
 * Copyright (c) 2025 by zyt, All Rights Reserved. 
 */
//
// Created by Quan2 on 2023/4/23.
//

#ifndef STM32F407_DEMO2_BSP_DELAY_H
#define STM32F407_DEMO2_BSP_DELAY_H

#ifndef     __delay_H
#define     __delay_H

#include "stm32f4xx.h"  
#include "tim.h"
void Delay_Init(void);
void Delay_us(uint32_t nus);
void Delay_ms(uint32_t nms);
int delay_ms(uint32_t nms);
void delay_us(uint32_t nus);
#endif

#endif //STM32F407_DEMO2_BSP_DELAY_H
