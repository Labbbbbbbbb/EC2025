/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-16
 * @brief       TFTLCD ʵ��
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� ̽���� F407������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"


int main(void)
{
    uint8_t x = 4;
    uint8_t lcd_id[12];
    
    HAL_Init();                             /* ��ʼ��HAL�� */
    sys_stm32_clock_init(336, 8, 2, 7);     /* ����ʱ��,168Mhz */
    delay_init(168);                        /* ��ʱ��ʼ�� */
    usart_init(115200);                     /* ���ڳ�ʼ��Ϊ115200 */
    led_init();                             /* ��ʼ��LED */
    lcd_init();                             /* ��ʼ��LCD */
    //g_point_color = RED;
    //sprintf((char *)lcd_id, "LCD ID:%04X", lcddev.id);  /* ��LCD ID��ӡ��lcd_id���� */
		uint16_t data[4]={0};
    while (1)
    {
        switch (x)
        {
        case 0:
            lcd_clear(WHITE);
            break;

        case 1:
            lcd_clear(BLACK);
            break;

        case 2:
            lcd_clear(BLUE);
            break;

        case 3:
            lcd_clear(RED);
            break;

        case 4:
            lcd_clear(MAGENTA);
            break;

        case 5:
            lcd_clear(GREEN);
            break;

        case 6:
            lcd_clear(CYAN);
            break;

        case 7:
            lcd_clear(YELLOW);
            break;

        case 8:
            lcd_clear(BRRED);
            break;

        case 9:
            lcd_clear(GRAY);
            break;

        case 10:
            lcd_clear(LGRAY);
            break;

        case 11:
            lcd_clear(BROWN);
            break;
        }
				data[0]=(uart_data [0]<<8) + uart_data[1]; 
				data[1]=(uart_data [2]<<8) + uart_data[3];
				data[2]=(uart_data [4]<<8) + uart_data[5];
				data[3]=(uart_data [6]<<8) + uart_data[7];
        lcd_show_num(256,20,data[0],2,32,BLUE);
				lcd_show_num(20,60,data[1],2,32,BLUE);
				lcd_show_num(59,100,data[2],2,32,BLUE);
				lcd_show_num(59,140,data[3],2,32,BLUE);
        x++;

        if (x == 12)
            x = 0;

        LED0_TOGGLE(); /*�����˸*/
        delay_ms(500);
    }
}
