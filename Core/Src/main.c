/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "can.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
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
#include <memory.h>
#include "Breathe_Light.h"
#include "Buzzer.h"
#include "Steer_Engine.h"
#include "Interrupt_trigger.h"
#include "Serial_port.h"
#include "Direct_Memory_Access.h"
#include "Remote_control.h"
#include "Flash.h"
#include "Motor_CAN.h"
#include "ist8310driver.h"
#include "BMI088driver.h"
#include "PID.h"
#include "AHRS.h"
#include "SPI_DMA.h"
#include "IMU_Slove.h"
#include "Board_communication.h"

enum colour{ RED = 1,GREEN = 2,BLUE = 3};
enum MODE{ DOWN = -1,UP = 1};
int state = UP;                      // 0:增加亮度 1:减小亮度
extern uint8_t OK;

uint8_t steer_mode = 0;


#define MOTOR_RAD_TO_ANGLE 57.29577951308238f // 360*2PI




/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  RC_ctrl_t local_rc_control;       // 遥控器数据
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
  MX_DMA_Init();
  MX_TIM5_Init();
  MX_TIM4_Init();
  MX_TIM8_Init();
  MX_TIM1_Init();
  MX_USART3_UART_Init();
  MX_USART6_UART_Init();
  MX_USART1_UART_Init();
  MX_CAN1_Init();
  MX_CAN2_Init();
  MX_I2C3_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  //灯500ms闪烁
    HAL_TIM_Base_Start_IT(&htim1);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);

//    HAL_TIM_Base_Start_IT(&htim8);
//    HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_1);
//    HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_2);
//    HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_3);

//  HAL_TIM_PeriodElapsedCallback(&htim1);

    //  启动时钟
    HAL_TIM_Base_Start(&htim5);
    HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_3);

    HAL_TIM_Base_Start(&htim4);
    HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);

    // 接收信号
    __HAL_UART_ENABLE_IT(&huart1,UART_FLAG_RXNE);

    remote_control_init();//遥控器初始化
    local_rc_control = *get_remote_control_point();//获取遥控器数据
    // CAN信号滤波
    CAN_filter_init();
    // IST8310初始化
    ist8310_init();
    // BMI088初始化
    BMI088_init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

while (1){

// 双板can通信
#define CAN_COMMUNICATION
#ifdef CAN_COMMUNICATION
    CAN_TX();
#endif //CAN_COMMUNICATION

// IMU
//#define IMU_SLOVE
#ifdef IMU_SLOVE
    static uint8_t flag = 0;
    if(flag == 0) {
        flag++;
        IMU_Init();
    }
    IMU_Slove();
#endif //IMU_SLOVE

// 加速度计、陀螺仪
//#define BMI088
#ifdef BMI088
    BMI088_read(BMI088_Real_Data.gyro,BMI088_Real_Data.accel,&BMI088_Real_Data.temp);
#endif //BMI088

// 磁力计
//#define IST8310
#ifdef IST8310
    uint16_t res = ist8310_init();
    if(res == 0x00){
        htim5.Instance->CCR2=100;
    }
#endif //IST8310


// 3508电机控制
//#define MOTOR_3508
#ifdef MOTOR_3508
//    CAN_cmd_motor(300,0,0,0);
// PDI控制
    Send();
    HAL_Delay(2);
#endif //MOTOR_3508

// FLASH 读写操作
//#define FLASH_OPERATE
#ifdef FLASH_OPERATE
    uint32_t buf[3];
    // 读取3*4个字节 即3组uint32_t数据
    Flash_Read(0x08020000,buf,3);
    uint8_t R, G, B;
    uint8_t index = 0;
    // 读取aRGB数据
    R = (buf[index] & 0xF00) >> 8;
    G = (buf[index] & 0x0F0) >> 4;
    B = (buf[index] & 0x00F) >> 0;
    if(R == 1) htim5.Instance->CCR3=100;
    else htim5.Instance->CCR3=0;
    if(G == 1) htim5.Instance->CCR2=100;
    else htim5.Instance->CCR2=0;
    if(B == 1) htim5.Instance->CCR1=100;
    else htim5.Instance->CCR1=0;

    static uint8_t Flash_init = 0;
    if(Flash_init == 1) {
        htim5.Instance->CCR1 = 0;
        htim5.Instance->CCR2 = 0;
        htim5.Instance->CCR3 = 0;

        uint32_t test[3] = {0x100,0x010,0x001};
        uint32_t *data = test;
        if(Flash_Erase(0x08020000, 1) == 1){
            htim5.Instance->CCR3=0;
            htim5.Instance->CCR2=100;
            htim5.Instance->CCR1=0;
        }
        if(Flash_Write(0x08020000, data, 3) == 1){
            htim5.Instance->CCR3=0;
            htim5.Instance->CCR2=100;
            htim5.Instance->CCR1=0;
        } else{
            htim5.Instance->CCR3=100;
            htim5.Instance->CCR2=0;
            htim5.Instance->CCR1=0;
        }
        Flash_init = 0;
    }
#endif //FLASH_OPERATE

//遥控器部分
//#define REMOTE
#ifdef REMOTE
    local_rc_control = *get_remote_control_point();//更新遥控器数据
    if(local_rc_control.rc.ch[4] != 0)
    {
        htim5.Instance->CCR1 = 100;
        htim5.Instance->CCR2 = 0;
        htim5.Instance->CCR3 = 0;
    } else{
        htim5.Instance->CCR1 = 0;
        htim5.Instance->CCR2 = 0;
        htim5.Instance->CCR3 = 100;
    }
#endif //REMOTE

// 串口通信
//#define SERIAL
#ifdef SERIAL
    // 分别对两个C板进行烧录,将UART2接口连接
    char buf[]="Robomaster";
    Serial(buf,0x100);
#endif //SERIAL

// 中断触发
//#define INTERRUPT
#ifdef INTERRUPT
    Interrupt(0x010,0x011);
#endif//INTERRUPT

// 舵机代码
//#define STEER_ENGINE
#ifdef STEER_ENGINE
//      Steer_init();
//      if(steer_mode == 0){
//          Steer_move(530);
//          steer_mode = 1;
//      } else if(steer_mode == 1){
//          Steer_move(1780);
//          steer_mode = 0;
//      }
    int Steer_PWM_180 = 1800; //180度
    int Steer_PWM_90 = 500; //50度
    htim1.Instance->CCR1 = Steer_PWM_180;
    HAL_Delay(100);
#endif// STEER_ENGINE

// 蜂鸣器代码
//#define BUZZER
#ifdef BUZZER
    static GPIO_PinState Read_PIN;
    Read_PIN = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);
    if(Read_PIN == GPIO_PIN_SET){
        htim5.Instance->CCR3 = 100;
        htim5.Instance->CCR2 = 0;
        htim5.Instance->CCR1 = 0;
    }
    if(Read_PIN == GPIO_PIN_RESET && OK == 0){
        OK = 1;
        htim5.Instance->CCR3 = 0;
        htim5.Instance->CCR2 = 100;
        htim5.Instance->CCR1 = 0;
    }

    if(OK == 1){
        HAL_Delay(500);
        wind_rise();
    }

#endif //BUZZER

// 呼吸灯代码
//#define TRICOLOR_BREATHE
#ifdef TRICOLOR_BREATHE
      // 选择颜色
      uint16_t Select = 0x010;
      if(state == UP) {
          state = DOWN;
          breathe_light(UP,Select);

      } else{
          state = UP;
          breathe_light(DOWN,Select);
      }
#endif //TRICOLOR_BREATHE

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 6;
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
