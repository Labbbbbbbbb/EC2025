/**
 ****************************************************************************************************
 * @file        atk_md0350_fsmc.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MD0350ģ��FSMC�ӿ���������
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

#ifndef __ATK_MD0350_FSMC_H
#define __ATK_MD0350_FSMC_H

#include "sys.h"

/* ATK-MD0350ģ��FSMC�ӿڶ��� */
#define ATK_MD0350_FSMC_BANK                    FSMC_NORSRAM_BANK4                          /* ATK-MD0350ģ������FSMC��Bank */
#define ATK_MD0350_FSMC_BANK_ADDR               (0x60000000)
#define ATK_MD0350_FSMC_REG_SEL                 (19)
#define ATK_MD0350_FSMC_READ_AST                0x00                                        /* ��ʱ��ĵ�ַ����ʱ�䣬��λ��HCLK */
#define ATK_MD0350_FSMC_READ_DST                0x0F                                        /* ��ʱ������ݽ���ʱ�䣬��λ��HCLK */
#define ATK_MD0350_FSMC_WRITE_AST               0x03                                        /* дʱ��ĵ�ַ����ʱ�䣬��λ��HCLK */
#define ATK_MD0350_FSMC_WRITE_DST               0x01                                        /* дʱ������ݽ���ʱ�䣬��λ��HCLK */
#define ATK_MD0350_FSMC_CLK_ENABLE()            do{ __HAL_RCC_FSMC_CLK_ENABLE(); }while(0)  /* ATK-MD0350ģ������FSMCʱ��ʹ�� */

/* ATK-MD0350ģ��FSMC�ӿڶ�д������ݵ�ַ */
#define ATK_MD0350_FSMC_CMD_ADDR                (ATK_MD0350_FSMC_BANK_ADDR | (((1U << ATK_MD0350_FSMC_REG_SEL) - 1) << 1))
#define ATK_MD0350_FSMC_DAT_ADDR                (ATK_MD0350_FSMC_BANK_ADDR | ((1U << ATK_MD0350_FSMC_REG_SEL) << 1))

/* ATK-MD0350ģ��FSMC�ӿڶ�д������� */
#define ATK_MD0350_FSMC_CMD_REG                 (*(volatile uint16_t *)ATK_MD0350_FSMC_CMD_ADDR)
#define ATK_MD0350_FSMC_DAT_REG                 (*(volatile uint16_t *)ATK_MD0350_FSMC_DAT_ADDR)

/* ���Ŷ��� */
#define ATK_MD0350_FSMC_RS_GPIO_PORT            GPIOE
#define ATK_MD0350_FSMC_RS_GPIO_PIN             GPIO_PIN_3
#define ATK_MD0350_FSMC_RS_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)
#define ATK_MD0350_FSMC_CS_GPIO_PORT            GPIOD
#define ATK_MD0350_FSMC_CS_GPIO_PIN             GPIO_PIN_7
#define ATK_MD0350_FSMC_CS_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)
#define ATK_MD0350_FSMC_RD_GPIO_PORT            GPIOD
#define ATK_MD0350_FSMC_RD_GPIO_PIN             GPIO_PIN_4
#define ATK_MD0350_FSMC_RD_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)
#define ATK_MD0350_FSMC_WR_GPIO_PORT            GPIOD
#define ATK_MD0350_FSMC_WR_GPIO_PIN             GPIO_PIN_5
#define ATK_MD0350_FSMC_WR_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)
#define ATK_MD0350_FSMC_D0_GPIO_PORT            GPIOD
#define ATK_MD0350_FSMC_D0_GPIO_PIN             GPIO_PIN_14
#define ATK_MD0350_FSMC_D0_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)
#define ATK_MD0350_FSMC_D1_GPIO_PORT            GPIOD
#define ATK_MD0350_FSMC_D1_GPIO_PIN             GPIO_PIN_15
#define ATK_MD0350_FSMC_D1_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)
#define ATK_MD0350_FSMC_D2_GPIO_PORT            GPIOD
#define ATK_MD0350_FSMC_D2_GPIO_PIN             GPIO_PIN_0
#define ATK_MD0350_FSMC_D2_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)
#define ATK_MD0350_FSMC_D3_GPIO_PORT            GPIOD
#define ATK_MD0350_FSMC_D3_GPIO_PIN             GPIO_PIN_1
#define ATK_MD0350_FSMC_D3_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)
#define ATK_MD0350_FSMC_D4_GPIO_PORT            GPIOE
#define ATK_MD0350_FSMC_D4_GPIO_PIN             GPIO_PIN_7
#define ATK_MD0350_FSMC_D4_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)
#define ATK_MD0350_FSMC_D5_GPIO_PORT            GPIOE
#define ATK_MD0350_FSMC_D5_GPIO_PIN             GPIO_PIN_8
#define ATK_MD0350_FSMC_D5_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)
#define ATK_MD0350_FSMC_D6_GPIO_PORT            GPIOE
#define ATK_MD0350_FSMC_D6_GPIO_PIN             GPIO_PIN_9
#define ATK_MD0350_FSMC_D6_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)
#define ATK_MD0350_FSMC_D7_GPIO_PORT            GPIOE
#define ATK_MD0350_FSMC_D7_GPIO_PIN             GPIO_PIN_10
#define ATK_MD0350_FSMC_D7_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)
#define ATK_MD0350_FSMC_D8_GPIO_PORT            GPIOE
#define ATK_MD0350_FSMC_D8_GPIO_PIN             GPIO_PIN_11
#define ATK_MD0350_FSMC_D8_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)
#define ATK_MD0350_FSMC_D9_GPIO_PORT            GPIOE
#define ATK_MD0350_FSMC_D9_GPIO_PIN             GPIO_PIN_12
#define ATK_MD0350_FSMC_D9_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)
#define ATK_MD0350_FSMC_D10_GPIO_PORT           GPIOE
#define ATK_MD0350_FSMC_D10_GPIO_PIN            GPIO_PIN_13
#define ATK_MD0350_FSMC_D10_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)
#define ATK_MD0350_FSMC_D11_GPIO_PORT           GPIOE
#define ATK_MD0350_FSMC_D11_GPIO_PIN            GPIO_PIN_14
#define ATK_MD0350_FSMC_D11_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)
#define ATK_MD0350_FSMC_D12_GPIO_PORT           GPIOE
#define ATK_MD0350_FSMC_D12_GPIO_PIN            GPIO_PIN_15
#define ATK_MD0350_FSMC_D12_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)
#define ATK_MD0350_FSMC_D13_GPIO_PORT           GPIOD
#define ATK_MD0350_FSMC_D13_GPIO_PIN            GPIO_PIN_8
#define ATK_MD0350_FSMC_D13_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)
#define ATK_MD0350_FSMC_D14_GPIO_PORT           GPIOD
#define ATK_MD0350_FSMC_D14_GPIO_PIN            GPIO_PIN_9
#define ATK_MD0350_FSMC_D14_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)
#define ATK_MD0350_FSMC_D15_GPIO_PORT           GPIOD
#define ATK_MD0350_FSMC_D15_GPIO_PIN            GPIO_PIN_10
#define ATK_MD0350_FSMC_D15_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)

/* �������� */
void atk_md0350_fsmc_init(void);                                                            /* ATK-MD0350ģ��FSMC�ӿڳ�ʼ�� */
static inline void atk_md0350_fsmc_write_cmd(volatile uint16_t cmd)                         /* ATK-MD0350ģ��ͨ��FSMC�ӿ�д���� */
{
    ATK_MD0350_FSMC_CMD_REG = cmd;
}
static inline void atk_md0350_fsmc_write_dat(volatile uint16_t dat)                         /* ATK-MD0350ģ��ͨ��FSMC�ӿ�д���� */
{
    ATK_MD0350_FSMC_DAT_REG = dat;
}
static inline void atk_md0350_fsmc_write_reg(volatile uint16_t reg, volatile uint16_t dat)  /* ATK-MD0350ģ��ͨ��FSMC�ӿ�д�Ĵ��� */
{
    ATK_MD0350_FSMC_CMD_REG = reg;
    ATK_MD0350_FSMC_DAT_REG = dat;
}
static inline uint16_t atk_md0350_fsmc_read_dat(void)                              /* ATK-MD0350ģ��ͨ��FSMC�ӿڶ����� */
{
    uint16_t dat;
    
    __nop();
    __nop();
    dat = ATK_MD0350_FSMC_DAT_REG;
    
    return dat;
}

#endif
