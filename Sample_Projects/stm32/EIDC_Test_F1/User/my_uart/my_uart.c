#include "my_uart.h"
#include "io_retargetToUart.h"
#define TX_UART USART3
#define TX_UART_HANDLE huart3
uint16_t tx_data[4]={0};
uint16_t rx_data[2]={0};
uint8_t rx_buffer[1]={0}; // 接收缓冲区
uint8_t u_flag      = 0;
float center_x      = 0;
float center_y      = 0;
void U_Transmit(uint16_t *data)
{
    uint8_t i;
    uint8_t tx_buffer[12]={0};
    tx_buffer[0] = 0xAA; /* 起始字节 */
    tx_buffer[1] = 0xBB; /* 起始字节 */
    tx_buffer[2] = 0xCC; /* 起始字节 */
    tx_buffer[3] = 0xDD; /* 起始字节 */
    for (i=4; i<=11; i++)
    {
        if(i%2==0)  tx_buffer[i] = data[(i-4)/2]>>8;        /* 高字节 */
        else        tx_buffer[i] = data[(i-5)/2];    /* 低字节 */
    }
    
    HAL_UART_Transmit(&TX_UART_HANDLE, tx_buffer, sizeof(tx_buffer), 1000); /* 发送数据 */
}

void U_Receive(uint8_t *buffer)
{
    if(buffer[0]==0xAA && buffer[1]==0xFF && buffer[6]==0xFF && buffer[7]==0xAA) /* 检查起始字节 */
    {
        rx_data[0]=((buffer[2]<<8)|buffer[3]); /* 解析数据 */
        rx_data[1]=((buffer[4]<<8)|buffer[5]); /* 解析数据 */
        //HAL_UART_Transmit(&huart3, (uint8_t *)buffer, sizeof(buffer), 100); /* 回传数据 */
    }    

    HAL_UART_Receive_IT(&huart2, buffer, sizeof(buffer)); /* 发送数据 */
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

    if (huart->Instance == USART2) {
        static uint8_t rxstate = 0;
        if (rxstate == 0) {
            if (rx_buffer[0] == 0xAA) /* ½ÓÊÕÎ´Íê³É */
            {
                rxstate++;
            }
        } else if (rxstate == 1) {
            if (rx_buffer[0] == 0xBB) /* ½ÓÊÕÎ´Íê³É */
            {
                rxstate++;
            } else {
                rxstate = 0;
            }
        } else if (rxstate == 2) {
            if (rx_buffer[0] == 0xCC) /* ½ÓÊÕÎ´Íê³É */
            {
                rxstate++;
            } else {
                rxstate = 0;
            }
        } else if (rxstate == 3) {
            if (rx_buffer[0] == 0xDD) /* ½ÓÊÕÎ´Íê³É */
            {
                rxstate++;
            } else {
                rxstate = 0;
            }
        } else if (rxstate >= 4 && rxstate <= 7) // ÖÐ¼ä8Î»Êý¾Ý
        {
            if ((rxstate - 4) % 2 == 0)
                rx_data[(rxstate - 4)/2] = rx_buffer[0] << 8; // Å¼ÊýÎ»ÊÇ¸ßÎ»Êý¾Ý
            else
                rx_data[(rxstate - 4)/2] += rx_buffer[0];
            rxstate++;
            if (rxstate == 8) 
            {
                u_flag = 1; // æ»»Ê¼
                rxstate = 0;
            }
        }
        //HAL_UART_Transmit(&huart3, rx_buffer, sizeof(rx_buffer), 100); /* 回传数据 */
        HAL_UART_Receive_IT(&huart2, (uint8_t *)rx_buffer, 1);
    }
    

} /**/

//1. HAL_UART_Receive_IT(&huart2, buffer, 1);
//2. 