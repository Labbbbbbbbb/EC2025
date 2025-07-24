/**
 ****************************************************************************************************
 * @file        atk_md0350_fsmc.c
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

#include "atk_md0350_fsmc.h"

/* ATK-MD0350ģ��FSMC��� */
static SRAM_HandleTypeDef g_sram_handle = {0};

/**
 * @brief       ATK-MD0350ģ��FSMC�ӿڳ�ʼ��
 * @param       ��
 * @retval      ��
 */
void atk_md0350_fsmc_init(void)
{
    GPIO_InitTypeDef gpio_init_struct = {0};
    FSMC_NORSRAM_TimingTypeDef read_timing = {0};
    FSMC_NORSRAM_TimingTypeDef write_timing = {0};
    
    /* ʹ��ʱ�� */
    ATK_MD0350_FSMC_CLK_ENABLE();
    ATK_MD0350_FSMC_RS_GPIO_CLK_ENABLE();
    ATK_MD0350_FSMC_CS_GPIO_CLK_ENABLE();
    ATK_MD0350_FSMC_RD_GPIO_CLK_ENABLE();
    ATK_MD0350_FSMC_WR_GPIO_CLK_ENABLE();
    ATK_MD0350_FSMC_D0_GPIO_CLK_ENABLE();
    ATK_MD0350_FSMC_D1_GPIO_CLK_ENABLE();
    ATK_MD0350_FSMC_D2_GPIO_CLK_ENABLE();
    ATK_MD0350_FSMC_D3_GPIO_CLK_ENABLE();
    ATK_MD0350_FSMC_D4_GPIO_CLK_ENABLE();
    ATK_MD0350_FSMC_D5_GPIO_CLK_ENABLE();
    ATK_MD0350_FSMC_D6_GPIO_CLK_ENABLE();
    ATK_MD0350_FSMC_D7_GPIO_CLK_ENABLE();
    ATK_MD0350_FSMC_D8_GPIO_CLK_ENABLE();
    ATK_MD0350_FSMC_D9_GPIO_CLK_ENABLE();
    ATK_MD0350_FSMC_D10_GPIO_CLK_ENABLE();
    ATK_MD0350_FSMC_D11_GPIO_CLK_ENABLE();
    ATK_MD0350_FSMC_D12_GPIO_CLK_ENABLE();
    ATK_MD0350_FSMC_D13_GPIO_CLK_ENABLE();
    ATK_MD0350_FSMC_D14_GPIO_CLK_ENABLE();
    ATK_MD0350_FSMC_D15_GPIO_CLK_ENABLE();
    
    /* ��ʼ��RS���� */
    gpio_init_struct.Pin    = ATK_MD0350_FSMC_RS_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_AF_PP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MD0350_FSMC_RS_GPIO_PORT, &gpio_init_struct);
    
    /* ��ʼ��CS���� */
    gpio_init_struct.Pin    = ATK_MD0350_FSMC_CS_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_AF_PP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MD0350_FSMC_CS_GPIO_PORT, &gpio_init_struct);
    
    /* ��ʼ��RD���� */
    gpio_init_struct.Pin    = ATK_MD0350_FSMC_RD_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_AF_PP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MD0350_FSMC_RD_GPIO_PORT, &gpio_init_struct);
    
    /* ��ʼ��WR���� */
    gpio_init_struct.Pin    = ATK_MD0350_FSMC_WR_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_AF_PP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MD0350_FSMC_WR_GPIO_PORT, &gpio_init_struct);
    
    /* ��ʼ��D0���� */
    gpio_init_struct.Pin    = ATK_MD0350_FSMC_D0_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_AF_PP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MD0350_FSMC_D0_GPIO_PORT, &gpio_init_struct);
    
    /* ��ʼ��D1���� */
    gpio_init_struct.Pin    = ATK_MD0350_FSMC_D1_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_AF_PP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MD0350_FSMC_D1_GPIO_PORT, &gpio_init_struct);
    
    /* ��ʼ��D2���� */
    gpio_init_struct.Pin    = ATK_MD0350_FSMC_D2_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_AF_PP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MD0350_FSMC_D2_GPIO_PORT, &gpio_init_struct);
    
    /* ��ʼ��D3���� */
    gpio_init_struct.Pin    = ATK_MD0350_FSMC_D3_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_AF_PP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MD0350_FSMC_D3_GPIO_PORT, &gpio_init_struct);
    
    /* ��ʼ��D4���� */
    gpio_init_struct.Pin    = ATK_MD0350_FSMC_D4_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_AF_PP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MD0350_FSMC_D4_GPIO_PORT, &gpio_init_struct);
    
    /* ��ʼ��D5���� */
    gpio_init_struct.Pin    = ATK_MD0350_FSMC_D5_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_AF_PP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MD0350_FSMC_D5_GPIO_PORT, &gpio_init_struct);
    
    /* ��ʼ��D6���� */
    gpio_init_struct.Pin    = ATK_MD0350_FSMC_D6_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_AF_PP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MD0350_FSMC_D6_GPIO_PORT, &gpio_init_struct);
    
    /* ��ʼ��D7���� */
    gpio_init_struct.Pin    = ATK_MD0350_FSMC_D7_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_AF_PP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MD0350_FSMC_D7_GPIO_PORT, &gpio_init_struct);
    
    /* ��ʼ��D8���� */
    gpio_init_struct.Pin    = ATK_MD0350_FSMC_D8_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_AF_PP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MD0350_FSMC_D8_GPIO_PORT, &gpio_init_struct);
    
    /* ��ʼ��D9���� */
    gpio_init_struct.Pin    = ATK_MD0350_FSMC_D9_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_AF_PP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MD0350_FSMC_D9_GPIO_PORT, &gpio_init_struct);
    
    /* ��ʼ��D10���� */
    gpio_init_struct.Pin    = ATK_MD0350_FSMC_D10_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_AF_PP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MD0350_FSMC_D10_GPIO_PORT, &gpio_init_struct);
    
    /* ��ʼ��D11���� */
    gpio_init_struct.Pin    = ATK_MD0350_FSMC_D11_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_AF_PP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MD0350_FSMC_D11_GPIO_PORT, &gpio_init_struct);
    
    /* ��ʼ��D12���� */
    gpio_init_struct.Pin    = ATK_MD0350_FSMC_D12_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_AF_PP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MD0350_FSMC_D12_GPIO_PORT, &gpio_init_struct);
    
    /* ��ʼ��D13���� */
    gpio_init_struct.Pin    = ATK_MD0350_FSMC_D13_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_AF_PP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MD0350_FSMC_D13_GPIO_PORT, &gpio_init_struct);
    
    /* ��ʼ��D14���� */
    gpio_init_struct.Pin    = ATK_MD0350_FSMC_D14_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_AF_PP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MD0350_FSMC_D14_GPIO_PORT, &gpio_init_struct);
    
    /* ��ʼ��D15���� */
    gpio_init_struct.Pin    = ATK_MD0350_FSMC_D15_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_AF_PP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MD0350_FSMC_D15_GPIO_PORT, &gpio_init_struct);
    
    /* FSMC���� */
    g_sram_handle.Instance                  = FSMC_NORSRAM_DEVICE;
    g_sram_handle.Extended                  = FSMC_NORSRAM_EXTENDED_DEVICE;
    g_sram_handle.Init.NSBank               = ATK_MD0350_FSMC_BANK;
    g_sram_handle.Init.DataAddressMux       = FSMC_DATA_ADDRESS_MUX_DISABLE;
    g_sram_handle.Init.MemoryType           = FSMC_MEMORY_TYPE_SRAM;
    g_sram_handle.Init.MemoryDataWidth      = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
    g_sram_handle.Init.BurstAccessMode      = FSMC_BURST_ACCESS_MODE_DISABLE;
    g_sram_handle.Init.WaitSignalPolarity   = FSMC_WAIT_SIGNAL_POLARITY_LOW;
    g_sram_handle.Init.WrapMode             = FSMC_WRAP_MODE_DISABLE;
    g_sram_handle.Init.WaitSignalActive     = FSMC_WAIT_TIMING_BEFORE_WS;
    g_sram_handle.Init.WriteOperation       = FSMC_WRITE_OPERATION_ENABLE;
    g_sram_handle.Init.WaitSignal           = FSMC_WAIT_SIGNAL_DISABLE;
    g_sram_handle.Init.ExtendedMode         = FSMC_EXTENDED_MODE_ENABLE;
    g_sram_handle.Init.AsynchronousWait     = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
    g_sram_handle.Init.WriteBurst           = FSMC_WRITE_BURST_DISABLE;
    /* FSMC��ʱ������ */
    read_timing.AddressSetupTime            = ATK_MD0350_FSMC_READ_AST;
    read_timing.DataSetupTime               = ATK_MD0350_FSMC_READ_DST;
    read_timing.BusTurnAroundDuration       = 0;
    read_timing.AccessMode                  = FSMC_ACCESS_MODE_A;
    /* FSMCдʱ������ */
    write_timing.AddressSetupTime           = ATK_MD0350_FSMC_WRITE_AST;
    write_timing.DataSetupTime              = ATK_MD0350_FSMC_WRITE_DST;
    write_timing.BusTurnAroundDuration      = 0;
    write_timing.AccessMode                 = FSMC_ACCESS_MODE_A;
    /* ��ʼ��FSMC */
    HAL_SRAM_Init(&g_sram_handle, &read_timing, &write_timing);
}
