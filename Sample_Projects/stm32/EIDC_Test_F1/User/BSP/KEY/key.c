/**
 ****************************************************************************************************
 * @file        key.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2023-04-23
 * @brief       ������������
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

#include "key.h"
#include "delay.h"

/**
 * @brief   ��ʼ������
 * @param   ��
 * @retval  ��
 */
void key_init(void)
{
    GPIO_InitTypeDef gpio_init_struct = {0};
    
    /* ʹ��GPIO�˿�ʱ�� */
    WKUP_GPIO_CLK_ENABLE();
    KEY0_GPIO_CLK_ENABLE();
    
    /* ����WKUP�������� */
    gpio_init_struct.Pin = WKUP_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_INPUT;
    gpio_init_struct.Pull = GPIO_PULLDOWN;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(WKUP_GPIO_PORT, &gpio_init_struct);
    
    /* ����KEY0�������� */
    gpio_init_struct.Pin = KEY0_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_INPUT;
    gpio_init_struct.Pull = GPIO_PULLDOWN;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(KEY0_GPIO_PORT, &gpio_init_struct);
}

/**
 * @brief   ɨ�谴��
 * @note    ������Ӧ�������ȼ���WKUP > KEY0
 * @param   mode: ɨ��ģʽ
 * @arg     0: ��֧��������
 * @arg     1: ֧��������
 * @retval  ������ֵ
 * @arg     NONE_PRES: û�а�������
 * @arg     WKUP_PRES: WKUP��������
 * @arg     KEY0_PRES: KEY0��������
 */
uint8_t key_scan(uint8_t mode)
{
    static uint8_t key_release = 1;
    uint8_t key_value = NONE_PRES;
    
    if (mode != 0)
    {
        key_release = 1;
    }
    
    if ((key_release == 1) && ((WKUP == 1) || (KEY0 == 1)))
    {
        delay_ms(10);
        key_release = 0;
        
        if (KEY0 == 1)
        {
            key_value = KEY0_PRES;
        }
        
        if (WKUP == 1)
        {
            key_value = WKUP_PRES;
        }
    }
    else if ((WKUP == 0) && (KEY0 == 0))
    {
        key_release = 1;
    }
    
    return key_value;
}
