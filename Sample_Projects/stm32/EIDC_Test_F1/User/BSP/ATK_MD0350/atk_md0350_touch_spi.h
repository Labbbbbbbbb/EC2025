/**
 ****************************************************************************************************
 * @file        atk_md0350_touch_spi.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MD0350ģ�鴥��SPI�ӿ���������
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

#ifndef __ATK_MD0350_TOUCH_SPI_H
#define __ATK_MD0350_TOUCH_SPI_H

#include "atk_md0350.h"

#if (ATK_MD0350_USING_TOUCH != 0)

/* ���Ŷ��� */
#define ATK_MD0350_TOUCH_SPI_MI_GPIO_PORT           GPIOD
#define ATK_MD0350_TOUCH_SPI_MI_GPIO_PIN            GPIO_PIN_6
#define ATK_MD0350_TOUCH_SPI_MI_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)
#define ATK_MD0350_TOUCH_SPI_MO_GPIO_PORT           GPIOB
#define ATK_MD0350_TOUCH_SPI_MO_GPIO_PIN            GPIO_PIN_3
#define ATK_MD0350_TOUCH_SPI_MO_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOB_CLK_ENABLE(); \
                                                        __HAL_RCC_AFIO_CLK_ENABLE();  \
                                                        __HAL_AFIO_REMAP_SWJ_NOJTAG();   }while(0)
#define ATK_MD0350_TOUCH_SPI_TCS_GPIO_PORT          GPIOC
#define ATK_MD0350_TOUCH_SPI_TCS_GPIO_PIN           GPIO_PIN_5
#define ATK_MD0350_TOUCH_SPI_TCS_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)
#define ATK_MD0350_TOUCH_SPI_CLK_GPIO_PORT          GPIOB
#define ATK_MD0350_TOUCH_SPI_CLK_GPIO_PIN           GPIO_PIN_0
#define ATK_MD0350_TOUCH_SPI_CLK_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)

/* IO���� */
#define ATK_MD0350_TOUCH_SPI_READ_MI()              HAL_GPIO_ReadPin(ATK_MD0350_TOUCH_SPI_MI_GPIO_PORT, ATK_MD0350_TOUCH_SPI_MI_GPIO_PIN)
#define ATK_MD0350_TOUCH_SPI_MO(x)                  do{ x ?                                                                                                         \
                                                        HAL_GPIO_WritePin(ATK_MD0350_TOUCH_SPI_MO_GPIO_PORT, ATK_MD0350_TOUCH_SPI_MO_GPIO_PIN, GPIO_PIN_SET) :      \
                                                        HAL_GPIO_WritePin(ATK_MD0350_TOUCH_SPI_MO_GPIO_PORT, ATK_MD0350_TOUCH_SPI_MO_GPIO_PIN, GPIO_PIN_RESET);     \
                                                    }while(0)
#define ATK_MD0350_TOUCH_SPI_TCS(x)                 do{ x ?                                                                                                         \
                                                        HAL_GPIO_WritePin(ATK_MD0350_TOUCH_SPI_TCS_GPIO_PORT, ATK_MD0350_TOUCH_SPI_TCS_GPIO_PIN, GPIO_PIN_SET) :    \
                                                        HAL_GPIO_WritePin(ATK_MD0350_TOUCH_SPI_TCS_GPIO_PORT, ATK_MD0350_TOUCH_SPI_TCS_GPIO_PIN, GPIO_PIN_RESET);   \
                                                    }while(0)
#define ATK_MD0350_TOUCH_SPI_CLK(x)                 do{ x ?                                                                                                         \
                                                        HAL_GPIO_WritePin(ATK_MD0350_TOUCH_SPI_CLK_GPIO_PORT, ATK_MD0350_TOUCH_SPI_CLK_GPIO_PIN, GPIO_PIN_SET) :    \
                                                        HAL_GPIO_WritePin(ATK_MD0350_TOUCH_SPI_CLK_GPIO_PORT, ATK_MD0350_TOUCH_SPI_CLK_GPIO_PIN, GPIO_PIN_RESET);   \
                                                    }while(0)

/* �������� */
void atk_md0350_touch_spi_init(void);               /* ATK-MD0350ģ�鴥��SPI�ӿڳ�ʼ�� */
uint16_t atk_md0350_touch_spi_read(uint8_t cmd);    /* ATK-MD0350ģ�鴥��SPI�ӿڶ����� */

#endif /* ATK_MD0350_USING_TOUCH */

#endif
