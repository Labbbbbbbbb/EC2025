/*
 * @Author: ZYT
 * @Date: 2025-07-28 15:39:06
 * @LastEditors: ZYT
 * @LastEditTime: 2025-07-28 17:14:46
 * @FilePath: \EC_tesr_F4\User\oled\My_spi.h
 * @Brief: 
 * 
 * Copyright (c) 2025 by zyt, All Rights Reserved. 
 */
//
// Created by lynliam on 23-7-3.
//

#ifndef STM32F407_DEMO2_MY_SPI_H
#define STM32F407_DEMO2_MY_SPI_H

#include "main.h"
#include "gpio.h"
#include "bsp_delay.h"
//f4
#define  SPI_WP_PIN          GPIO_PIN_6
#define  SPI_WP_PORT         GPIOC

#define  SPI_CS_PIN          oled_CS_Pin
#define  SPI_CS_PORT         oled_CS_GPIO_Port

#define  SPI_CLK_PIN         oled_D0_Pin
#define  SPI_CLK_PORT        oled_D0_GPIO_Port

#define  SPI_MOSI_PIN        oled_D1_Pin
#define  SPI_MOSI_PORT       oled_D1_GPIO_Port

#define  SPI_MISO_PIN        GPIO_PIN_4
#define  SPI_MISO_PORT       GPIOB

#define  SPI_BASE			 SPI1
//f1
// #define  SPI_WP_PIN          GPIO_PIN_0
// #define  SPI_WP_PORT         GPIOB

// #define  SPI_CS_PIN          GPIO_PIN_0
// #define  SPI_CS_PORT         GPIOB

// #define  SPI_CLK_PIN         GPIO_PIN_5
// #define  SPI_CLK_PORT        GPIOA

// #define  SPI_MOSI_PIN        GPIO_PIN_7
// #define  SPI_MOSI_PORT       GPIOA

// #define  SPI_MISO_PIN        GPIO_PIN_4     //MISO在oled里面用不到，无所谓
// #define  SPI_MISO_PORT       GPIOB

// #define  SPI_BASE			 SPI1

#define  SPI_FLASH_CS_LOW()    HAL_GPIO_WritePin(SPI_CS_PORT, SPI_CS_PIN,GPIO_PIN_RESET)  /* Select SPI MEM1: ChipSelect pin low  */
#define  SPI_FLASH_CS_HIGH()   HAL_GPIO_WritePin(SPI_CS_PORT, SPI_CS_PIN, GPIO_PIN_SET)    /* Deselect SPI MEM1: ChipSelect pin high */

#define  SPI_FLASH_WP_LOW()    HAL_GPIO_WritePin(SPI_CS_PORT, SPI_CS_PIN,GPIO_PIN_RESET)  //PC4
#define  SPI_FLASH_WP_HIGH()   HAL_GPIO_WritePin(SPI_CS_PORT, SPI_CS_PIN, GPIO_PIN_SET)    //PC4

//void SendandReceiveByte(uint8_t byte);
void SPI_PIN_Init(void);
void SPI_SendByte(uint8_t byte);

#endif //STM32F407_DEMO2_MY_SPI_H