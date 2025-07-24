/**
 ****************************************************************************************************
 * @file        led.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2023-04-23
 * @brief       LED��������
 * @license     Copyright (c) 2020-2032, �������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 * 
 * ʵ��ƽ̨:����ԭ�� M100Z-M3��Сϵͳ��STM32F103��
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 * 
 ****************************************************************************************************
 */

#include "led.h"

/**
 * @brief   ��ʼ��LED
 * @param   ��
 * @retval  ��
 */
void led_init(void)
{
    GPIO_InitTypeDef gpio_init_struct = {0};
    
    /* ʹ��GPIO�˿�ʱ�� */
    LED0_GPIO_CLK_ENABLE();
    LED1_GPIO_CLK_ENABLE();
    
    /* ����LED0�������� */
    gpio_init_struct.Pin = LED0_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull = GPIO_PULLDOWN;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED0_GPIO_PORT, &gpio_init_struct);
    
    /* ����LED1�������� */
    gpio_init_struct.Pin = LED1_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull = GPIO_PULLDOWN;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED1_GPIO_PORT, &gpio_init_struct);
    
    /* �ر�LED0��LED1 */
    LED0(1);
    LED1(1);
}
