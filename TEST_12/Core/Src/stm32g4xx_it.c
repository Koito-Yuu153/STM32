/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32g4xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "stm32g4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include	<string.h>
#include	<stdlib.h>
#include 	<stdio.h>
#include	 "CarList.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
extern uint8_t UartRxBuffer[RX_BUFFER_LEN];
extern	CarListTypedef		CarList;
extern	CarMessageTypedef	NewCar;

extern float	CNBR_Price;		//CNBR每小时停车价格
extern float	VNBR_Price;		//VNBR每小时停车价格
float		Price=0.00;
char Price_Buffer[20];

ParkMessageTypedef	ParkPrice={0.00,0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void HAL_UART_IDLE_Callback(UART_HandleTypeDef *huart);
void CmdProcess(uint8_t cmd);
uint8_t CheckCmd(uint8_t* cmd_buffer,uint8_t cmd_length,CarListTypedef* carlist,CarMessageTypedef* newcar);
ParkMessageTypedef PirceCalculate(CarListTypedef* carlist,CarMessageTypedef* newcar,uint8_t i);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern UART_HandleTypeDef huart1;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */
	HAL_SYSTICK_IRQHandler();
  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32G4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32g4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles USART1 global interrupt / USART1 wake-up interrupt through EXTI line 25.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */
	if(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_IDLE)!=RESET)
	{
		HAL_UART_IDLE_Callback(&huart1);
	}
  /* USER CODE END USART1_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/*处理接收到指令
1、判断指令格式，正确就执行下一步，不正确就串口发送ERROR警告
2、指令格式正确，遍历CarList,查找是否存在车牌号和停车类型相同的成员，如果有，则认为是出库，如果没有则为入库
3、根据指令判断结果对CarList执行增添或删除操作
*/
void HAL_UART_IDLE_Callback(UART_HandleTypeDef *huart)
{
	
	__HAL_UART_CLEAR_IDLEFLAG(huart); //清除中断标志位
	HAL_UART_DMAStop(huart);  //停止DMA接收
	uint8_t	data_length=RX_BUFFER_LEN - __HAL_DMA_GET_COUNTER(huart->hdmarx); //获取接收数据的长度

	if(data_length!=0)
		CmdProcess(CheckCmd(UartRxBuffer,data_length,&CarList,&NewCar));
	
	HAL_UART_Receive_DMA(huart,UartRxBuffer,RX_BUFFER_LEN);  //重新开始DMA接收
}



uint8_t CheckCmd(uint8_t* cmd_buffer,uint8_t cmd_length,CarListTypedef* carlist,CarMessageTypedef* newcar)
{
	uint8_t	cmd_check=ERROR;
	
	if(cmd_length==22&&(memcmp(cmd_buffer,"VNBR:",5)==0||memcmp(cmd_buffer,"CNBR:",5)==0)&&cmd_buffer[9]==':') //初步检查
	{
		for(uint8_t i=10;i<22;i++)
		{
			if(cmd_buffer[i]>='0'&&cmd_buffer[i]<='9')
			{
				
			}
			else
			{
				return ERROR;
			}
		}
		//执行到这里说明代码格式正确,将数据写入结构体以便进一步检查
		sscanf((char*)cmd_buffer,"%4s:%4s:%2d%2d%2d%2d%2d%2d",newcar->Type,newcar->CarNum,(int*)&newcar->Year,
					(int*)&newcar->Month,(int*)&newcar->Date,(int*)&newcar->Hours,(int*)&newcar->Minutes,(int*)&newcar->Seconds);
		//初步检查时间逻辑
		if(newcar->Hours<=23 && newcar->Minutes<=59 && newcar->Date<=31)
		{
			cmd_check=Car_List_Search(carlist,newcar);
			return cmd_check;
		}
		return ERROR;
	}
	
	return ERROR;
	
}

void CmdProcess(uint8_t cmd)
{
	if(cmd !=ERROR)
	{
		if(cmd==CAR_IN) //车辆入库
		{
			Car_List_Add(&CarList,&NewCar);
			sprintf(Price_Buffer,"CAR_IN %s %s %2d",NewCar.Type,NewCar.CarNum,NewCar.Hours);
			HAL_UART_Transmit(&huart1,(uint8_t*)Price_Buffer,17,10);
		}
		else
		{
			ParkPrice=PirceCalculate(&CarList,&NewCar,cmd);
			sprintf(Price_Buffer,"%s:%s:%2d:%.2f ",NewCar.Type,NewCar.CarNum,ParkPrice.ParkHours,ParkPrice.Price);
			Car_List_Delete(&CarList,&NewCar);
			HAL_UART_Transmit(&huart1,(uint8_t*)Price_Buffer,20,10);
		}
	}
	else
	{
		HAL_UART_Transmit(&huart1,(uint8_t*)"ERROR\n",6,10);
	}
}

ParkMessageTypedef PirceCalculate(CarListTypedef* carlist,CarMessageTypedef* newcar,uint8_t i)
{
	ParkMessageTypedef price={0.00,0};
	price.ParkHours=(newcar->Hours)-(carlist->CarList[i].Hours);
	
	if(memcmp(newcar->Type,"CNBR",4)==0)
	{
		if(price.ParkHours>1)
			price.Price=price.ParkHours*CNBR_Price;
		else
			price.Price=CNBR_Price;
	}
	else
	{
		if(price.ParkHours>1)
			price.Price=price.ParkHours*VNBR_Price;
		else
			price.Price=VNBR_Price;
	}
	
	return price;
}


/* USER CODE END 1 */

