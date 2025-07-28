#include "my_uart.h"
#define TX_UART USART3
#define TX_UART_HANDLE huart3
uint16_t tx_data[4]={0};
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