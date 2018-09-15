/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */     
#include "iwdg.h"
#include "usart.h"
#include "CarControl.h"
/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId defaultTaskHandle;
osThreadId controlTaskHandle;
osMessageQId MessageQueueHandle;

/* USER CODE BEGIN Variables */
static osPoolId MessagePool;
#define RX_SIZE 32
static uint8_t Rx1[RX_SIZE];
static uint8_t Rx3[RX_SIZE];
/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void StartDefaultTask(void const * argument);
void StartControlTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */
struct MessageStr {
	uint8_t  *p;
	uint16_t  size;
};
static void ExecCarCmd(uint8_t *, uint16_t);
/* USER CODE END FunctionPrototypes */

/* Hook prototypes */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 64);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of controlTask */
  osThreadDef(controlTask, StartControlTask, osPriorityBelowNormal, 0, 64);
  controlTaskHandle = osThreadCreate(osThread(controlTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the queue(s) */
  /* definition and creation of MessageQueue */
  osMessageQDef(MessageQueue, 16, uint32_t);
  MessageQueueHandle = osMessageCreate(osMessageQ(MessageQueue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{

  /* USER CODE BEGIN StartDefaultTask */
	osPoolDef(Message, 16, sizeof(struct MessageStr));
	MessagePool = osPoolCreate(osPool(Message));

	HAL_UART_Receive_DMA(&huart1, Rx1, RX_SIZE);
	__HAL_UART_CLEAR_IDLEFLAG(&huart1);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);

	HAL_UART_Receive_DMA(&huart3, Rx3, RX_SIZE);
	__HAL_UART_CLEAR_IDLEFLAG(&huart3);
	__HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);

	const struct _CarControl *CarController = GetCarController();
	/* Infinite loop */
	for (;;) {
		HAL_IWDG_Refresh(&hiwdg);
		if (!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) && !HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1))
		{
			CarController->Stop();
			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		}
		else
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
		osDelay(250);
	}
  /* USER CODE END StartDefaultTask */
}

/* StartControlTask function */
void StartControlTask(void const * argument)
{
  /* USER CODE BEGIN StartControlTask */
	osEvent event;
	struct MessageStr *Message = NULL;
	/* Infinite loop */
	for (;;) {
		event = osMessageGet(MessageQueueHandle, osWaitForever);
		if (event.status == osEventMessage) {
			Message = event.value.p;
			ExecCarCmd(Message->p, Message->size);
			osPoolFree(MessagePool, Message);
		}
	}
  /* USER CODE END StartControlTask */
}

/* USER CODE BEGIN Application */
static void ExecCarCmd(uint8_t *p, uint16_t size)
{
	const struct _CarControl *CarController = GetCarController();
	switch (p[0]) {
	case '1':
		CarController->ForLeft();
		break;
	case '2':
		CarController->Forward();
		break;
	case '3':
		CarController->ForRight();
		break;
	case '4':
		CarController->Left();
		break;
	case '5':
		CarController->Stop();
		break;
	case '6':
		CarController->Right();
		break;
	case '7':
		CarController->BackLeft();
		break;
	case '8':
		CarController->Backward();
		break;
	case '9':
		CarController->BackRight();
		break;
	default:
		CarController->Stop();
		break;
	}
}

void UART_IDLE_Handler(UART_HandleTypeDef *huart)
{
	struct MessageStr *Message = NULL;

	if(__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE))
	{
		__HAL_UART_CLEAR_IDLEFLAG(huart);
		__HAL_UART_DISABLE_IT(huart, UART_IT_IDLE);
		HAL_UART_DMAStop(huart);

		Message = osPoolCAlloc(MessagePool);
		if(Message != NULL)
		{
			Message->p    = huart->pRxBuffPtr;
			Message->size = huart->RxXferSize - __HAL_DMA_GET_COUNTER(huart->hdmarx);
			osMessagePut(MessageQueueHandle, (uint32_t)Message, osWaitForever);
		}

		HAL_UART_Receive_DMA(huart, huart->pRxBuffPtr, huart->RxXferSize);
		__HAL_UART_CLEAR_IDLEFLAG(huart);
		__HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
	}
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
