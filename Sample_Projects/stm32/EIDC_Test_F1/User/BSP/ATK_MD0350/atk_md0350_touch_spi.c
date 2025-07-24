/**
 ****************************************************************************************************
 * @file        atk_md0350_touch_spi.c
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

#include "atk_md0350_touch_spi.h"
#include "delay.h"

#if (ATK_MD0350_USING_TOUCH != 0)

/**
 * @brief       ATK-MD0350ģ�鴥��SPI�ӿ�д����
 * @param       ��д�������
 * @retval      ��
 */
static void atk_md0350_touch_spi_write(uint8_t dat)
{
    uint8_t dat_index;
    
    for (dat_index=0; dat_index<8; dat_index++)
    {
        if ((dat & 0x80) != 0)
        {
            ATK_MD0350_TOUCH_SPI_MO(1);
        }
        else
        {
            ATK_MD0350_TOUCH_SPI_MO(0);
        }
        
        dat <<= 1;
        ATK_MD0350_TOUCH_SPI_CLK(0);
        delay_us(1);
        ATK_MD0350_TOUCH_SPI_CLK(1);
    }
}

/**
 * @brief       ATK-MD0350ģ�鴥��SPI�ӿڳ�ʼ��
 * @param       ��
 * @retval      ��
 */
void atk_md0350_touch_spi_init(void)
{
    GPIO_InitTypeDef gpio_init_struct = {0};
    
    /* ʹ��ʱ�� */
    ATK_MD0350_TOUCH_SPI_MI_GPIO_CLK_ENABLE();
    ATK_MD0350_TOUCH_SPI_MO_GPIO_CLK_ENABLE();
    ATK_MD0350_TOUCH_SPI_TCS_GPIO_CLK_ENABLE();
    ATK_MD0350_TOUCH_SPI_CLK_GPIO_CLK_ENABLE();
    
    /* ��ʼ��MI���� */
    gpio_init_struct.Pin    = ATK_MD0350_TOUCH_SPI_MI_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_INPUT;
    gpio_init_struct.Pull   = GPIO_PULLUP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MD0350_TOUCH_SPI_MI_GPIO_PORT, &gpio_init_struct);
    
    /* ��ʼ��MO���� */
    gpio_init_struct.Pin    = ATK_MD0350_TOUCH_SPI_MO_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull   = GPIO_PULLUP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MD0350_TOUCH_SPI_MO_GPIO_PORT, &gpio_init_struct);
    
    /* ��ʼ��TCS���� */
    gpio_init_struct.Pin    = ATK_MD0350_TOUCH_SPI_TCS_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull   = GPIO_PULLUP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MD0350_TOUCH_SPI_TCS_GPIO_PORT, &gpio_init_struct);
    
    /* ��ʼ��CLK���� */
    gpio_init_struct.Pin    = ATK_MD0350_TOUCH_SPI_CLK_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull   = GPIO_PULLUP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MD0350_TOUCH_SPI_CLK_GPIO_PORT, &gpio_init_struct);
}

/**
 * @brief       ATK-MD0350ģ�鴥��SPI�ӿڶ�����
 * @param       ��
 * @retval      ����������
 */
uint16_t atk_md0350_touch_spi_read(uint8_t cmd)
{
    uint16_t dat = 0;
    uint8_t dat_index;
    
    ATK_MD0350_TOUCH_SPI_CLK(0);
    ATK_MD0350_TOUCH_SPI_MO(0);
    ATK_MD0350_TOUCH_SPI_TCS(0);
    atk_md0350_touch_spi_write(cmd);
    delay_us(6);
    ATK_MD0350_TOUCH_SPI_CLK(0);
    delay_us(1);
    ATK_MD0350_TOUCH_SPI_CLK(1);
    delay_us(1);
    ATK_MD0350_TOUCH_SPI_CLK(0);
    
    for (dat_index=0; dat_index<16; dat_index++)
    {
        dat <<= 1;
        ATK_MD0350_TOUCH_SPI_CLK(0);
        delay_us(1);
        ATK_MD0350_TOUCH_SPI_CLK(1);
        if (ATK_MD0350_TOUCH_SPI_READ_MI() != 0)
        {
            dat++;
        }
    }
    
    dat >>= 4;
    ATK_MD0350_TOUCH_SPI_TCS(1);
    
    return dat;
}

#endif /* ATK_MD0350_USING_TOUCH */
