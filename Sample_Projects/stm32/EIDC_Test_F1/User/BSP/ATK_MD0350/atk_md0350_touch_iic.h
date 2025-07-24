/**
 ****************************************************************************************************
 * @file        atk_md0350_touch_iic.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MD0350ģ�鴥��IIC�ӿ���������
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

#ifndef __ATK_MD0350_TOUCH_IIC_H
#define __ATK_MD0350_TOUCH_IIC_H

#include "atk_md0350.h"

#if (ATK_MD0350_USING_TOUCH != 0)

/* ���Ŷ��� */
#define ATK_MD0350_TOUCH_IIC_SCL_GPIO_PORT          GPIOB
#define ATK_MD0350_TOUCH_IIC_SCL_GPIO_PIN           GPIO_PIN_0
#define ATK_MD0350_TOUCH_IIC_SCL_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)
#define ATK_MD0350_TOUCH_IIC_SDA_GPIO_PORT          GPIOB
#define ATK_MD0350_TOUCH_IIC_SDA_GPIO_PIN           GPIO_PIN_3
#define ATK_MD0350_TOUCH_IIC_SDA_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOB_CLK_ENABLE(); \
                                                        __HAL_RCC_AFIO_CLK_ENABLE();  \
                                                        __HAL_AFIO_REMAP_SWJ_NOJTAG();   }while(0)

/* IO���� */
#define ATK_MD0350_TOUCH_IIC_SCL(x)                 do{ x ?                                                                                                         \
                                                        HAL_GPIO_WritePin(ATK_MD0350_TOUCH_IIC_SCL_GPIO_PORT, ATK_MD0350_TOUCH_IIC_SCL_GPIO_PIN, GPIO_PIN_SET) :    \
                                                        HAL_GPIO_WritePin(ATK_MD0350_TOUCH_IIC_SCL_GPIO_PORT, ATK_MD0350_TOUCH_IIC_SCL_GPIO_PIN, GPIO_PIN_RESET);   \
                                                    }while(0)

#define ATK_MD0350_TOUCH_IIC_SDA(x)                 do{ x ?                                                                                                         \
                                                        HAL_GPIO_WritePin(ATK_MD0350_TOUCH_IIC_SDA_GPIO_PORT, ATK_MD0350_TOUCH_IIC_SDA_GPIO_PIN, GPIO_PIN_SET) :    \
                                                        HAL_GPIO_WritePin(ATK_MD0350_TOUCH_IIC_SDA_GPIO_PORT, ATK_MD0350_TOUCH_IIC_SDA_GPIO_PIN, GPIO_PIN_RESET);   \
                                                    }while(0)

#define ATK_MD0350_TOUCH_IIC_READ_SDA()             HAL_GPIO_ReadPin(ATK_MD0350_TOUCH_IIC_SDA_GPIO_PORT, ATK_MD0350_TOUCH_IIC_SDA_GPIO_PIN)

/* ������� */
#define ATK_MD0350_TOUCH_IIC_EOK    0   /* û�д��� */
#define ATK_MD0350_TOUCH_IIC_ERROR  1   /* ���� */

/* �������� */
void atk_md0350_touch_iic_init(uint8_t iic_addr);                                   /* ��ʼ��IIC�ӿ� */
uint8_t atk_md0350_touch_iic_write_reg(uint16_t reg, uint8_t *buf, uint8_t len);    /* дATK-MD0350ģ�鴥���Ĵ��� */
void atk_md0350_touch_iic_read_reg(uint16_t reg, uint8_t *buf, uint8_t len);        /* ��ATK-MD0350ģ�鴥���Ĵ��� */

#endif /* ATK_MD0350_USING_TOUCH */

#endif
