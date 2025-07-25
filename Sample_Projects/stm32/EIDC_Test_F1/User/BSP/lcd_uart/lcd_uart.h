#ifndef __LCD_UART_H
#define __LCD_UART_H

#include "stm32f1xx.h"


#define USART_TX_GPIO_PORT                  GPIOB
#define USART_TX_GPIO_PIN                   GPIO_PIN_10
#define USART_TX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */

#define USART_RX_GPIO_PORT                  GPIOA
#define USART_RX_GPIO_PIN                   GPIO_PIN_11
#define USART_RX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */

#define USART_UX                            USART3
#define USART_UX_IRQn                       USART3_IRQn
#define USART_UX_IRQHandler                 USART3_IRQHandler
#define USART_UX_CLK_ENABLE()               do{ __HAL_RCC_USART3_CLK_ENABLE(); }while(0)  /* USART3 时钟使能 */

/******************************************************************************************/

#define USART_REC_LEN               200         /* 定义最大接收字节数 200 */
#define USART_EN_RX                 1           /* 使能（1）/禁止（0）串口1接收 */
#define RXBUFFERSIZE   1                        /* 缓存大小 */

extern UART_HandleTypeDef g_uart3_handle;       /* HAL UART句柄 */

extern uint8_t  g_usart_rx_buf[USART_REC_LEN];  /* 接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 */
extern uint16_t g_usart_rx_sta;                 /* 接收状态标记 */
extern uint8_t g_rx_buffer[RXBUFFERSIZE];       /* HAL库USART接收Buffer */


void usart_init(uint32_t bound);                /* 串口初始化函数 */

#endif







