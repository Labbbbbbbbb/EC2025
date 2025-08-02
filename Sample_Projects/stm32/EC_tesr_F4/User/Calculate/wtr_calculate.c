/*
 * @Author: szf
 * @Date: 2023-02-22 12:04:21
 * @LastEditTime: 2025-05-27 00:46:34
 * @LastEditors: ZYT
 * @brief 运动学逆解算及PID计算函数
 * @FilePath: \pantilt_freertos__\User\Calculate\wtr_calculate.c
 */

#include "wtr_calculate.h"

#include <math.h>
//#include "Chassis_UserConfig.h"


/**
 * @brief: PID控制-增量式PID
 * @auther: Chen YiTong 3083697520
 * @param {__IO PID_t} *pid
 * @return {*}
 */

void PID_Calc(__IO PID_t *pid)
{
    pid->cur_error = pid->ref - pid->fdb;
    pid->output += pid->KP * (pid->cur_error - pid->error[1]) + pid->KI * pid->cur_error + pid->KD * (pid->cur_error - 2 * pid->error[1] + pid->error[0]);
    pid->error[0] = pid->error[1];
    pid->error[1] = pid->ref - pid->fdb;
    /*设定输出上限*/
    if (pid->output > pid->outputMax) pid->output = pid->outputMax;
    if (pid->output < -pid->outputMax) pid->output = -pid->outputMax;
}


/**
 * @brief: PID控制-位置式PID
 * @auther: zyt
 * @param {__IO PID_t} *pid
 * @return {*}
 */

void PID_Calc_P(__IO PID_t *pid)
{
    pid->cur_error = pid->ref - pid->fdb;
    pid->integral += pid->cur_error;
    pid->output = pid->KP * pid->cur_error + pid->KI * pid->integral + pid->KD * (pid->cur_error - pid->error[1]);
    pid->error[0] = pid->error[1];  //这句已经没有用了
    pid->error[1] = pid->ref - pid->fdb;
    /*设定输出上限*/
    if (pid->output > pid->outputMax) pid->output = pid->outputMax;
    if (pid->output < -pid->outputMax) pid->output = -pid->outputMax;
}


/**
 * @brief: PID算法-P控制
 * @auther: Chen YiTong 3083697520
 * @param {__IO PID_t} *pid
 * @return {*}
 */
void P_Calc(__IO PID_t *pid)
{
    pid->cur_error = pid->ref - pid->fdb;
    pid->output    = pid->KP * pid->cur_error;
    /*设定输出上限*/
    if (pid->output > pid->outputMax) pid->output = pid->outputMax;
    if (pid->output < -pid->outputMax) pid->output = -pid->outputMax;

    if (fabs(pid->output) < pid->outputMin)
        pid->output = 0;
}



/**
 * @brief: 速度伺服函数
 * @auther: zyt
 * @param {float} ref 目标值
 * @param {DJI_t} *motor 电机结构体
 * @return {*}
 */
void speedServo(float ref, float fdb,PID_t* pid)
{
    pid->ref = ref;
    pid->fdb = fdb;
    PID_Calc(pid);
}
void pid_init(PID_t *pid,float kp,float ki,float kd)
{
    pid->KP = kp;
    pid->KI = ki;
    pid->KD = kd;
    pid->outputMax = 300;
}