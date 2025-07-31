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

// void Rem_FT_Angle_EN(uint8_t yaw_or_pitch)
// {
//     static uint8_t flag = 0;
//     if(flag==0)
//     {
//       initial_angle_yaw = ReadPos(YAW);
//       initial_angle_pitch = ReadPos(PITCH);
//       flag = 1;
//     }

// }


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
  WheelMode(PITCH, 1); // 模弝0：佝置模�?? 模弝1：杒速； 模弝2：pwm调�?�；模弝3：步�??
  WheelMode(YAW, 1); // 模弝0：佝置模�?? 模弝1：杒速； 模弝2：pwm调�?�；模弝3：步�??
  WriteSpe(PITCH, 0, 0);//pitch
  WriteSpe(YAW, 0, 0);//yaw
  pid_init(&pid_x, 10, 0.0, 0.0);//yaw
  pid_init(&pid_y, 10, 0.0, 0.0);//pitch

  uint8_t no_find_flag = 0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      
      /* USER CODE END WHILE */

      /* USER CODE BEGIN 3 */
      __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 999); /*设置PWM占空�?????*/
      if (rx_data[0] == rx_data[1] && rx_data[1] == 0) {
          // OLED_Refresh();
          // uint8_t rxdat[4] = "rx:";
          // OLED_ShowString(0,0,rxdat, 16);
          // OLED_ShowNum(100,0,(uint16_t)(center_x),3,16);
          // OLED_ShowNum(100,25,(uint16_t)(center_y),3,16);
          // OLED_Refresh();
          //  speedServo(320, rx_data[0], &pid_x);
          //  speedServo(240, rx_data[1], &pid_y);
          //  WriteSpe(YAW, (int16_t)pid_x.output, 100);
          //  WriteSpe(PITCH, (int16_t)(-1 * pid_y.output), 50);
          //  WriteSpe(YAW, -10 * (rx_data[0] - 320), 100);
          //  WriteSpe(PITCH, 10 * (rx_data[1] - 240), 100);
          // u_flag = 0; /* 清除接收标志 */
          no_find_flag++;
          if (no_find_flag > 10) {

              printf("no find target,pos:%d,%d\n", ReadPos(YAW), ReadPos(PITCH));
              //WriteSpe(YAW, -500, 100);
              WriteSpe(YAW, 0, 0);

              WheelMode(PITCH, 0);
              WritePos(PITCH, 2250, 60, 50); // pitch
          }
    }else{
        uint8_t Kpy = 2;
        uint8_t Kpx = 2;
        if ((-Kpx * (rx_data[0] - 320)) > 1000) spex = 1000;
        else if ((-Kpx * (rx_data[0] - 320)) < -1000) spex = -1000;
        else spex = -Kpx * (rx_data[0] - 320);

        if((Kpy * (rx_data[1] - 240)) > 1000 ) spey = 1000;
        else if ((Kpy * (rx_data[1] - 240)) < -1000 )spey = -1000;
        else spey = Kpy * (rx_data[1] - 240);
        WheelMode(YAW, 1);
        WheelMode(PITCH, 1);
        WriteSpe(YAW, spex, 100);
        WriteSpe(PITCH, spey, 100);
        printf("%d,%d,%d,%d\n", spex, spey, ReadPos(YAW), ReadPos(PITCH));

        no_find_flag=0;
    }
   
    //printf("%d\n", __HAL_UART_GET_FLAG(&huart2, UART_FLAG_ORE));
    //printf("receive data: %d %d\n", rx_data[0], rx_data[1]);
    //printf("%d,%d,%d,%d,%d,%d\n",rx_buffer[0],rx_buffer[1],rx_buffer[2],rx_buffer[3], rx_buffer[4], rx_buffer[5]);
    // WriteSpe(1, -50, 50); // 正�?�向上，相机向上变大
    // WriteSpe(2, 50, 50); // 正�?�向右，相机向右变小
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
