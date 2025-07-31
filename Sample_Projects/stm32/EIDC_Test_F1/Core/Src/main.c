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
//#include "HWT101CT_sdk.h"
#include "io_retargetToUart.h"
#include "oled.h"
#include "my_uart.h"
#include "wtr_calculate.h"
//#include "User_SMS_STS.h"
#include "SCServo.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define PITCH 1
#define YAW 2
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
int16_t spex;
int16_t spey;
uint16_t initial_angle_yaw;
uint16_t initial_angle_pitch;
PID_t pid_x, pid_y;




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
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_SPI1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  SPI_PIN_Init();
  OLED_Init();
  HAL_UART_Receive_IT(&huart2, rx_buffer, sizeof(rx_buffer));
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
  WheelMode(PITCH, 1); // 模弝0：佝置模�??? 模弝1：杒速； 模弝2：pwm调�?�；模弝3：步�???
  WheelMode(YAW, 1); // 模弝0：佝置模�??? 模弝1：杒速； 模弝2：pwm调�?�；模弝3：步�???
  WriteSpe(PITCH, 0, 0);//pitch
  WriteSpe(YAW, 0, 0);//yaw
  pid_init(&pid_x, 1.5, 0.1, 0.3);//yaw - 增加响应速度
  pid_init(&pid_y, 2, 0.0, 0.0);//pitch

  uint8_t no_find_flag = 0;//标记目标丢失的时刻
  uint8_t find_flag = 0;//标记目标是否被锁定（几乎到达中心）
  uint8_t init_flag=0;//标记在目标未找到时是否处于初始化状态
  
  // 死区参数
  uint8_t dead_zone_x = 8;  // X轴死区
  uint8_t dead_zone_y = 6;  // Y轴死区

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      // WriteSpe(1, -50, 50); // 正�?�向上，相机向上变大
      // WriteSpe(2, 50, 50); // 正�?�向右，相机向右变小
      /* USER CODE END WHILE */

      /* USER CODE BEGIN 3 */
      // 如果已经锁定目标，检查是否需要重新追踪
      if(find_flag == 1) {
          int16_t error_x = rx_data[0] - 302;
          int16_t error_y = rx_data[1] - 240;
          
          // 如果目标移出死区，重新开始追踪
          if (abs(error_x) > dead_zone_x * 2 || abs(error_y) > dead_zone_y * 2) {
              find_flag = 0;  // 重置锁定状态
              printf("Target moved, resume tracking\n");
          } else {
              // 保持锁定状态，不执行追踪
              printf("Target locked, maintaining position\n");
          }
      }
      
      if(find_flag==0)//未锁定
      {
        if (rx_data[0] == rx_data[1] && rx_data[1] == 0) {//丢失目标
            if(init_flag==0)
            {
              no_find_flag++;
              if (no_find_flag > 20) {

                printf("no find target,pos:%d,%d\n", ReadPos(YAW), ReadPos(PITCH));
                WheelMode(YAW, 1);
                WriteSpe(YAW, -100, 100);
              // WriteSpe(YAW, 0, 0);

                WheelMode(PITCH, 0);
                WritePos(PITCH, 2250, 60, 50); // pitch
                
                }
            }else{
              //运动中丢失目标，使用卡尔曼滤波的估计坐标补上

              }
          }else{    //视野内有目标但尚未追上
            // 旧的固定条件判断 - 可以删除或作为备用
            // 计算速度控制
            // 这里可以根据实际需要调整PID参数和速度计算逻辑
            // 例如：
        // uint8_t Kpy = 2;
        // uint8_t Kpx = 2;
        // if ((-Kpx * (rx_data[0] - 320)) > 1000) spex = 1000;
        // else if ((-Kpx * (rx_data[0] - 320)) < -1000) spex = -1000;
        // else spex = -Kpx * (rx_data[0] - 320);

        // if((Kpy * (rx_data[1] - 240)) > 1000 ) spey = 1000;
        // else if ((Kpy * (rx_data[1] - 240)) < -1000 )spey = -1000;
        // else spey = Kpy * (rx_data[1] - 240);
            WheelMode(YAW, 1);
            WheelMode(PITCH, 1);
            no_find_flag = 0;
            init_flag    = 1; // 设置初始化标志，标志已经找到过，不再处于初始的寻找状态，此后就算丢失也认为是运动中的暂时丢失
            // 计算误差
            int16_t error_x = rx_data[0] - 302;
            int16_t error_y = rx_data[1] - 240;
            
            // 死区控制 - 当误差足够小时停止运动
            if (abs(error_x) <= dead_zone_x && abs(error_y) <= dead_zone_y) {
                // 目标在死区内，停止运动并锁定位置
                WheelMode(YAW, 0);
                //WheelMode(PITCH, 0);
                uint16_t pos_yaw = ReadPos(YAW);
                //uint16_t pos_pitch = ReadPos(PITCH);
                WritePos(YAW, pos_yaw, 60, 50);
                //WritePos(PITCH, pos_pitch, 60, 50);
                __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 100); /*降低PWM占空比*/
                find_flag = 1;
                printf("Target locked at center: error_x=%d, error_y=%d\n", error_x, error_y);
            } else {
                // 目标不在死区内，使用PID控制
                speedServo(302, rx_data[0], &pid_x);
                speedServo(240, rx_data[1], &pid_y);
                WheelMode(YAW, 1);

                // 限制输出速度，避免过冲
                int16_t output_x = (int16_t)pid_x.output;
                int16_t output_y = (int16_t)(-1 * pid_y.output);
                
                // 根据误差大小动态调整最大速度
                int16_t max_speed_x = (abs(error_x) > 50) ? 100 : 60;  // 远距离快速，近距离慢速
                int16_t max_speed_y = (abs(error_y) > 30) ? 50 : 30;
                
                // 速度限制
                if (output_x > max_speed_x) output_x = max_speed_x;
                if (output_x < -max_speed_x) output_x = -max_speed_x;
                if (output_y > max_speed_y) output_y = max_speed_y;
                if (output_y < -max_speed_y) output_y = -max_speed_y;
                
                WriteSpe(YAW, output_x, 100);
                WriteSpe(PITCH, output_y, 50);
                
                printf("Tracking: error_x=%d, error_y=%d, output_x=%d, output_y=%d\n", 
                       error_x, error_y, output_x, output_y);
            }

            // 旧的固定条件判断 - 可以删除或作为备用
            if(abs(rx_data[0]-302)<5)
            {
              WheelMode(YAW, 0);
              uint16_t pos = ReadPos(YAW);
               WritePos(YAW, pos, 60, 50); // yaw
               __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 100); /*设置PWM占空比*/
               find_flag = rx_data[0];
               
               // HAL_Delay(10000);
            }
        }
      }

        // 每10次循环输出一次状态信息，避免过多打印影响性能
        static uint8_t print_counter = 0;
        if(++print_counter >= 10) {
            printf("PID_out:%f, flag:%d, rx:[%f,%f]\n", 
                   pid_x.output, find_flag, (float)rx_data[0], (float)rx_data[1]);
            print_counter = 0;
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
  while (1)
  {
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
