/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
// #include "HWT101CT_sdk.h"
#include "io_retargetToUart.h"
#include "HWT101CT_sdk.h"
#include "my_uart.h"
#include "wtr_calculate.h"

#include "SCServo.h"
#include "HWT_user.h"
#include "stdio.h"
#include "stdlib.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define PITCH 1
#define YAW   2
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint16_t initial_angle_yaw;
uint16_t initial_angle_pitch;
PID_t pid_x, pid_y;
uint8_t mode;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART6_UART_Init();
  MX_USART3_UART_Init();
  MX_TIM3_Init();
  MX_UART5_Init();
  /* USER CODE BEGIN 2 */

  HAL_UART_Receive_IT(&huart3, rx_buffer, sizeof(rx_buffer));
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_Base_Start_IT(&htim3);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  
  WriteSpe(PITCH, 0, 0);           // pitch
  WriteSpe(YAW, 0, 0);             // yaw
  pid_init(&pid_x, 1, 0.08, 0.00); // yaw - 增加响应速度
  pid_init(&pid_y, 1, 0.05, 0.0);   // pitch
  

  uint8_t no_find_flag = 1; // 标记目标丢失的时�???????
  uint8_t find_flag    = 0; // 标记目标是否被锁定（几乎到达中心�???????
  uint8_t init_flag    = 0; // 标记在目标未找到时是否处于初始化状�??
  uint16_t yaw_pos_target = 0; // 目标位置
  uint16_t pitch_pos_target = 0; // 目标位置
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
 
    while (1) {
        // WriteSpe(1, -50, 50); // 正向上，相机向上变大
        // WriteSpe(2, 50, 50); // 正向右，相机向右变小
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    if(mode==1)
    {
      WheelMode(YAW, 0); 
      WritePosEx(YAW, 2220+512, 500, 100); // 45°,具体的待�??
      __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 100); /*设置PWM占空�??*/
    }
    if(mode==3)
    {
      if(find_flag==0)
      {
       if(rx_data[0]==rx_data[1]&&rx_data[0]==0) // 目标丢失
       {
           no_find_flag ++;
           if(no_find_flag > 10) // 连续10次丢失目�??
           {
               WheelMode(PITCH, 0); 
               WheelMode(YAW, 1);  
               WritePosEx(PITCH, 2250, 50,100);
               WriteSpe(YAW, -400, 100);
               printf("nofound\n");
           }else
               printf("nofoundonce\n");

    }else{
      no_find_flag = 0;
      WheelMode(YAW, 1);
      WheelMode(PITCH, 1);
      //int16_t error_yaw = rx_data[0] - 298; // 298为中心点;
      speedServo(rx_data[0], 298, &pid_x); // pitch
      speedServo(rx_data[1], 250, &pid_y); // pitch
      WriteSpe(YAW, -1 * pid_x.output, 100); // 发�?�目标位�??
      WriteSpe(PITCH,  pid_y.output, 100); // 发�?�目标位�??
      if(abs(rx_data[0]-298)<8&&abs(rx_data[1]-250)<8) // 如果目标在中心点附近
       {
          yaw_pos_target = ReadPos(YAW);
          pitch_pos_target = ReadPos(PITCH);
          //WriteSpe(YAW, 0, 0);
          //find_flag = 1;          // 目标被锁定ri
          __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 100); /*设置PWM占空�??*/
          WheelMode(YAW, 0);
          WheelMode(PITCH, 0);
          WritePosEx(YAW, yaw_pos_target, 500, 100); // HAL_Delay(10000); // 延时100ms
          WritePosEx(PITCH, pitch_pos_target, 500, 100); // HAL_Delay(10000); // 延时100ms
       } // 如果目标未被锁定
      }
    }
      else if(find_flag==1)
      {
           //WriteSpe(YAW, 0, 0); // 停止
           WheelMode(YAW, 0);
           WheelMode(PITCH, 0);
           WritePosEx(YAW, yaw_pos_target, 500, 100); // HAL_Delay(10000); // 延时100ms
           WritePosEx(PITCH, pitch_pos_target, 500, 100);
      }
       printf("%d,%d,%f,%f\n", rx_data[0],rx_data[1], pid_x.output,pid_y.output);
    
  }
  
}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == MODE1_Pin) // 引脚判断
    {
        mode = 1;
    }
    if (GPIO_Pin == MODE2_Pin) // 引脚判断
    {
        mode = 2;
    }
    if (GPIO_Pin == MODE3_Pin) // 引脚判断
    {
        mode = 3;
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) {
    }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
