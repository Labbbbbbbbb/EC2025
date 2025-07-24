/**
 ****************************************************************************************************
 * @file        demo.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MD0350ģ�����ʵ�飨FSMC��
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
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

#include "demo.h"
#include "./BSP/ATK_MD0350/atk_md0350.h"
#include "./BSP/led/led.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include <math.h>

#define PI (float)(3.1415926)

static float cube[8][3] = {
    {-16, -16, -16},
    {-16, +16, -16},
    {+16, +16, -16},
    {+16, -16, -16},
    {-16, -16, +16},
    {-16, +16, +16},
    {+16, +16, +16},
    {+16, -16, +16}
};

static uint8_t line_id[24] = {
    1, 2, 2, 3,
    3, 4, 4, 1,
    5, 6, 6, 7,
    7, 8, 8, 5,
    8, 4, 7, 3,
    6, 2, 5, 1
};

/**
 * @brief       �������˷�
 * @param       a      : ����a
 *              b[3][3]: ����b
 * @retval      ������
 */
static float *demo_matconv(float *a, float b[3][3])
{
    float res[3];
    uint8_t res_index;
    uint8_t a_index;
    
    for (res_index=0; res_index<3; res_index++)
    {
        res[res_index] = b[res_index][0] * a[0] + b[res_index][1] * a[1] + b[res_index][2] * a[2];
    }
    
    for (a_index=0; a_index<3; a_index++)
    {
        a[a_index] = res[a_index];
    }
    
    return a;
}

/**
 * @brief       ��ת����
 * @param       point: ��Ҫ��ת������
 *              x    : X����ת��
 *              y    : Y����ת��
 *              z    : Z����ת��
 * @retval      ������
 */
static void demo_rotate(float *point, float x, float y, float z)
{
    float rx[3][3];
    float ry[3][3];
    float rz[3][3];
    
    x /= PI;
    y /= PI;
    z /= PI;
    
    rx[0][0] = cos(x);
    rx[0][1] = 0;
    rx[0][2] = sin(x);
    rx[1][0] = 0;
    rx[1][1] = 1;
    rx[1][2] = 0;
    rx[2][0] = -sin(x);
    rx[2][1] = 0;
    rx[2][2] = cos(x);
    
    ry[0][0] = 1;
    ry[0][1] = 0;
    ry[0][2] = 0;
    ry[1][0] = 0;
    ry[1][1] = cos(y);
    ry[1][2] = -sin(y);
    ry[2][0] = 0;
    ry[2][1] = sin(y);
    ry[2][2] = cos(y);
    
    rz[0][0] = cos(z);
    rz[0][1] = -sin(z);
    rz[0][2] = 0;
    rz[1][0] = sin(z);
    rz[1][1] = cos(z);
    rz[1][2] = 0;
    rz[2][0] = 0;
    rz[2][1] = 0;
    rz[2][2] = 1;
    
    demo_matconv(demo_matconv(demo_matconv(point, rz), ry), rx);
}

/**
 * @brief       ��ʾ������3D��ת
 * @param       ��
 * @retval      ��
 */
static void demo_show_cube(void)
{
    uint8_t point_index;
    uint8_t line_index;
    static uint16_t x = 160;
    static uint16_t y = 281;
    
    static atk_md0350_touch_point_t point[1];
    
    /* ATK-MD0350ģ�鴥��ɨ�� */
    if (tp_type ? (atk_md0350_ctp_touch_scan(point, sizeof(point) / sizeof(point[0])) == sizeof(point) / sizeof(point[0])) : (atk_md0350_rtp_touch_scan(&point[0].x, &point[0].y) == 0))
    {
        if ((point[0].x > 28) && (point[0].x < (atk_md0350_get_lcd_width() - 28)) && (point[0].y > 110) && (point[0].y < (atk_md0350_get_lcd_height() - 28)))
        {
            x = point[0].x;
            y = point[0].y;
        }
    }
    
    for (point_index=0; point_index<8; point_index++)
    {
        demo_rotate(cube[point_index], 0.5f, 0.3f, 0.2f);
    }
    
    for (line_index=0; line_index<24; line_index+=2)
    {
        /* ATK-MD0350ģ��LCD���߶� */
        atk_md0350_draw_line(   x + cube[line_id[line_index] - 1][0],
                                y + cube[line_id[line_index] - 1][1],
                                x + cube[line_id[line_index + 1] - 1][0],
                                y + cube[line_id[line_index + 1] - 1][1],
                                ATK_MD0350_BLUE);
    }
    
    delay_ms(100);
    
    atk_md0350_fill(x - 28, y - 28, x + 28, y + 28, ATK_MD0350_WHITE);
}

/**
 * @brief       ������ʾ��ں���
 * @param       ��
 * @retval      ��
 */
void demo_run(void)
{
    uint8_t ret;
    
    /* ��ʼ��ATK-MD0350ģ�� */
    ret = atk_md0350_init();
    if (ret != 0)
    {
        printf("ATK-MD0350 init failed!\r\n");
        while (1)
        {
            LED0_TOGGLE();
            delay_ms(200);
        }
    }
    
    /* ATK-MD0350ģ��LCD���� */
    atk_md0350_clear(ATK_MD0350_WHITE);
    /* ATK-MD0350ģ��LCD��ʾ�ַ��� */
    atk_md0350_show_string(10, 10, ATK_MD0350_LCD_WIDTH, 32, "STM32", ATK_MD0350_LCD_FONT_32, ATK_MD0350_RED);
    atk_md0350_show_string(10, 42, ATK_MD0350_LCD_WIDTH, 24, "ATK-MD0350", ATK_MD0350_LCD_FONT_24, ATK_MD0350_RED);
    atk_md0350_show_string(10, 66, ATK_MD0350_LCD_WIDTH, 16, "ATOM@ALIENTEK", ATK_MD0350_LCD_FONT_16, ATK_MD0350_RED);
    
    while (1)
    {
        /* ��ʾ������3D��ת */
        demo_show_cube();
    }
}
