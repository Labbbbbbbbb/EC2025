资源：

```
https://wit-motion.yuque.com/wumwnr/docs/lvqyh7?singleDoc#%20%E3%80%8AHWT101CT-TTL/232%E4%BA%A7%E5%93%81%E8%B5%84%E6%96%99%E3%80%8B

https://wit-motion.yuque.com/wumwnr/bf4d0f/ggixbq?#%20%E3%80%8A%E9%93%9D%E5%A3%B3%E5%80%BE%E8%A7%92%E4%BB%AA%E5%92%8C%E6%83%AF%E5%AF%BC%E4%BC%A0%E6%84%9F%E5%99%A8TTL/232-%E4%BD%BF%E7%94%A8%E4%B8%B2%E5%8F%A3%E5%B7%A5%E5%85%B7--%E5%BF%AB%E9%80%9F%E4%B8%8A%E6%89%8B%E3%80%8B
```



同级目录下的`HWT101`文件夹是丢进`User`的驱动包，但是里面得把串口改成对应的串口号，目前在`f1`测试的`baudrate`是115200，不知道为啥当时在`f405`是230400

对应在main中需要写



```
#include "HWT101CT_sdk.h"
#include "io_retargetToUart.h"

......

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
  /* USER CODE BEGIN 2 */
  __HAL_UART_ENABLE_IT(&huart1,UART_IT_RXNE);
  HAL_UART_Receive_IT(&huart1,&s,1); 
  HW101_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    HAL_Delay(1);
    ProcessData();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    printf("fangle:%f,%f,%f\n",fAngle[2], fAcc[2], fGyro[2]);
  }
  /* USER CODE END 3 */
}
```

