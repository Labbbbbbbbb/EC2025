/*
 * @Author: szf
 * @Date: 2023-02-22 12:06:17
 * @LastEditors: ZYT
 * @LastEditTime: 2025-05-26 01:41:27
 * @FilePath: \pantilt_freertos__\User\Calculate\wtr_calculate.h
 * @brief大疆电机PID以及底盘逆解
 *
 * Copyright (c) 2023 by ChenYiTong, All Rights Reserved.
 */

#pragma once

#include "main.h"


typedef struct {
    float KP;        // PID参数P
    float KI;        // PID参数I
    float KD;        // PID参数D
    float fdb;       // PID反馈值
    float ref;       // PID目标值
    float cur_error; // 当前误差
    float error[2];  // 前两次误差
    float output;    // 输出值
    float outputMax; // 最大输出值的绝对值
    float outputMin; // 最小输出值的绝对值用于防抖

    float integral; // add by zyt
} PID_t;

void speedServo(float ref, float fdb, PID_t *pid);
void pid_init(PID_t *pid, float kp, float ki, float kd);
