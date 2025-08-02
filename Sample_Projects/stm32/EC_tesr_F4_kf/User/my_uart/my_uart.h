#ifndef MY_UART
#define MY_UART

#include "main.h"
#include "usart.h"


extern uint16_t tx_data[4];
extern uint8_t rx_buffer[1]; // 接收缓冲区

extern uint16_t rx_data[2];
extern uint8_t u_flag;
extern float center_x ;
extern float center_y ;

void U_Transmit(uint16_t *data);



#endif // !MY_UART


