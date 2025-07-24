/**
 ****************************************************************************************************
 * @file        atk_md0350_touch.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MD0350ģ�鴥����������
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

#ifndef __ATK_MD0350_TOUCH_H
#define __ATK_MD0350_TOUCH_H

#include "atk_md0350.h"

#if (ATK_MD0350_USING_TOUCH != 0)

#define ATK_MD0350_TOUCH_READ_TIMES             5   /* ���嵥�λ�ȡ�����ȡ�Ĵ��� */
#define ATK_MD0350_TOUCH_READ_DISCARD           1   /* ���嵥�λ�ȡ���궪���������Сֵ������ */
#define ATK_MD0350_TOUCH_READ_RANGE             50  /* ���嵥�λ�ȡ�����ȡ������ֵ֮���������ֵ */

/* ���Ŷ��� */
#define ATK_MD0350_TOUCH_PEN_GPIO_PORT          GPIOB
#define ATK_MD0350_TOUCH_PEN_GPIO_PIN           GPIO_PIN_1
#define ATK_MD0350_TOUCH_PEN_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)

#define ATK_MD0350_TOUCH_TCS_GPIO_PORT          GPIOC
#define ATK_MD0350_TOUCH_TCS_GPIO_PIN           GPIO_PIN_5
#define ATK_MD0350_TOUCH_TCS_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)

/* IO���� */
#define ATK_MD0350_TOUCH_READ_PEN()             HAL_GPIO_ReadPin(ATK_MD0350_TOUCH_PEN_GPIO_PORT, ATK_MD0350_TOUCH_PEN_GPIO_PIN)

#define ATK_MD0350_TOUCH_PEN(x)                 do{ x ?                                                                                                 \
                                                    HAL_GPIO_WritePin(ATK_MD0350_TOUCH_PEN_GPIO_PORT, ATK_MD0350_TOUCH_PEN_GPIO_PIN, GPIO_PIN_SET) :    \
                                                    HAL_GPIO_WritePin(ATK_MD0350_TOUCH_PEN_GPIO_PORT, ATK_MD0350_TOUCH_PEN_GPIO_PIN, GPIO_PIN_RESET);   \
                                                }while(0)

#define ATK_MD0350_TOUCH_TCS(x)                 do{ x ?                                                                                                 \
                                                    HAL_GPIO_WritePin(ATK_MD0350_TOUCH_TCS_GPIO_PORT, ATK_MD0350_TOUCH_TCS_GPIO_PIN, GPIO_PIN_SET) :    \
                                                    HAL_GPIO_WritePin(ATK_MD0350_TOUCH_TCS_GPIO_PORT, ATK_MD0350_TOUCH_TCS_GPIO_PIN, GPIO_PIN_RESET);   \
                                                }while(0)

/* ATK-MD0350����IICͨѶ��ַö�� */
typedef enum
{
    ATK_MD0350_TOUCH_IIC_ADDR_14 = 0x14,    /* 0x14 */
    ATK_MD0350_TOUCH_IIC_ADDR_5D = 0x5D,    /* 0x5D */
} atk_md0350_touch_iic_addr_t;

/* �������������ݽṹ */
typedef struct
{
    uint16_t x;     /* ������X���� */
    uint16_t y;     /* ������Y���� */
    uint16_t size;  /* �������С */
} atk_md0350_touch_point_t;

/* ������� */
#define ATK_MD0350_TOUCH_EOK                    0   /* û�д��� */
#define ATK_MD0350_TOUCH_ERROR                  1
#define ATK_MD0350_TOUCH_EMPTY                  2   /* �� */

/* �������� */
uint8_t atk_md0350_touch_init(void);                                                /* ATK-MD0350ģ�鴥����ʼ�� */
uint8_t atk_md0350_rtp_touch_scan(uint16_t *x, uint16_t *y);                        /* ATK-MD0350ģ����败��ɨ�� */
uint8_t atk_md0350_ctp_touch_scan(atk_md0350_touch_point_t *point, uint8_t cnt);    /* ATK-MD0350ģ����ݴ���ɨ�� */
#endif /* ATK_MD0350_USING_TOUCH */

#endif
