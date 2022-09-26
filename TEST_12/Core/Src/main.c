/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include	"lcd.h"
#include	 "CarList.h"
#include	<stdio.h>
#include	<stdlib.h>
#include  <string.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define	DATA_SHOW	0
#define	PARA_SHOW	1

#define PWM_OFF		0
#define PWM_ON		1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t UartRxBuffer[RX_BUFFER_LEN];

u8 CNBR_Count_Buff[20];
u8 VNBR_Count_Buff[20];
u8 IDLE_Count_Buff[20];

u8 CNBR_Price_Buff[20];
u8 VNBR_Price_Buff[20];

float		CNBR_Price=3.50;		//CNBR每小时停车价格
float		VNBR_Price=2.00;		//VNBR每小时停车价格

CarListTypedef		CarList={0};
CarMessageTypedef	NewCar;

int LED_State=0xffff;

uint8_t PWM_State=PWM_ON;

uint8_t	LCD_STATE =DATA_SHOW; //上电初始数据界面
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void LED_Display(int led_state);
void LCD_Show(void);
void KeyScan(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
	
	__HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE);
	HAL_UART_Receive_DMA(&huart1,UartRxBuffer,RX_BUFFER_LEN);
	
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,100); //20%占空比方波
	
	LCD_Init();
	LCD_Clear(Black);
	LCD_SetBackColor(Black);
	LCD_SetTextColor(White);
	LED_Display(LED_State);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		LCD_Show();
		LED_Display(LED_State);
		if((MAX_SIZE-CarList.Length)>0)
		{
			LED_State&=0XFEFF;
		}
		else
		{
			LED_State|=0X0100;
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
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV3;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void LED_Display(int led_state)
{
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	GPIOC->ODR=led_state;
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}

void LCD_Show(void)
{
	if(LCD_STATE==DATA_SHOW)
	{
		sprintf((char*)CNBR_Count_Buff,"   CNBR: %d          ",CarList.CNBR_Count);
		sprintf((char*)VNBR_Count_Buff,"   VNBR: %d          ",CarList.VNBR_Count);
		sprintf((char*)IDLE_Count_Buff,"   IDLE: %d          ",(MAX_SIZE-CarList.Length));
		LCD_DisplayStringLine(Line1,(u8*)"        Data        ");
		LCD_DisplayStringLine(Line3,CNBR_Count_Buff);
		LCD_DisplayStringLine(Line5,VNBR_Count_Buff);
		LCD_DisplayStringLine(Line7,IDLE_Count_Buff);
	}
	else if(LCD_STATE==PARA_SHOW)
	{
		LCD_DisplayStringLine(Line1,(u8*)"        Para        ");
		sprintf((char*)CNBR_Price_Buff,"   CNBR: %.2f    ",CNBR_Price);
		sprintf((char*)VNBR_Price_Buff,"   VNBR: %.2f    ",VNBR_Price);
		LCD_DisplayStringLine(Line3,CNBR_Price_Buff);
		LCD_DisplayStringLine(Line5,VNBR_Price_Buff);
		LCD_DisplayStringLine(Line7,(u8*)"                    ");
	}
}

void KeyScan(void)
{
	if(HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)==0&&LCD_STATE==DATA_SHOW)
	{
		LCD_Clear(Black);
		LCD_STATE=PARA_SHOW;
		while(HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)==0);
	}
	else if(HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)==0&LCD_STATE==PARA_SHOW)
	{
		LCD_Clear(Black);
		LCD_STATE=DATA_SHOW;
		while(HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)==0);
	}
	
	if(HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin)==0&&LCD_STATE==PARA_SHOW)
	{
		CNBR_Price+=0.50;
		VNBR_Price+=0.50;
		while(HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin)==0);
	}
	
	if(HAL_GPIO_ReadPin(KEY3_GPIO_Port,KEY3_Pin)==0&&LCD_STATE==PARA_SHOW)
	{
		if(CNBR_Price>=0.50&&VNBR_Price>=0.50)
		{
			CNBR_Price-=0.50;
			VNBR_Price-=0.50;
		}
		else
		{
			CNBR_Price=CNBR_Price;
			VNBR_Price=VNBR_Price;
		}

		while(HAL_GPIO_ReadPin(KEY3_GPIO_Port,KEY3_Pin)==0);
	}
	if(HAL_GPIO_ReadPin(KEY4_GPIO_Port,KEY4_Pin)==0&&PWM_State==PWM_OFF)
	{
		PWM_State=PWM_ON;
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,100); //20%占空比方波
		while(HAL_GPIO_ReadPin(KEY4_GPIO_Port,KEY4_Pin)==0);
	}
	else if(HAL_GPIO_ReadPin(KEY4_GPIO_Port,KEY4_Pin)==0&&PWM_State==PWM_ON)
	{
		PWM_State=PWM_OFF;
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,0); //低电平
		while(HAL_GPIO_ReadPin(KEY4_GPIO_Port,KEY4_Pin)==0);
	}
}

void HAL_SYSTICK_Callback()
{
	static int key_time;
	key_time++;
	if(key_time==50)
	{
		key_time=0;
		KeyScan();
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

