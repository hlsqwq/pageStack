/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
#include "tim.h"
#include "Music.h"
#include "button4_4.h"
#include "debug.h"
#include "page.h"
#include "home.h"
#include "sensor.h"
#include "dht11.h"
#include "dwt.h"


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
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

void ledTask(void *arg){
		Item item;
		strcpy(item.str,"hello world");
		item.len=strlen(item.str);

	while(1){

		debug.ops.add(item);
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		osDelay(1000);
	}
}


void btnTask(void *arg)
{
	while(1){
		btnCtl.ops.scan(&btnCtl);
		osDelay(10);
	}

}

void beeTask(void *arg)
{	
	while(1){
		MUSIC_Analysis();
	}
}


void handleTask(void *arg)
{
	TaskHandle_t bee=NULL;
	int isRunning=0;

	
	while (1){
		char ch;
		xQueueReceive(btnCtl.q,&ch,portMAX_DELAY);
		switch(ch){
			case '0':
				HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
				break;
			case '1':
				if(bee==NULL){
					oledClean();
					oledDisplay(0, 0, "start bee");
					xTaskCreate(beeTask, "beeTask", 512, NULL, osPriorityNormal, &bee);
					isRunning=1;
				}
				break;
				
			case '2':
				if(bee!=NULL){
					oledClean();
					oledDisplay(0, 0, "del bee");
					vTaskDelete(bee);
					BSP_Buzzer_SetFrequency(0);
					bee=NULL;
					isRunning=0;
				}
				break;
				
			case '3':
				if(bee!=NULL){
					if(isRunning){
						oledClean();
						oledDisplay(0, 0, "suspend bee");
						vTaskSuspend(bee);
						BSP_Buzzer_SetFrequency(0);
						isRunning=0;
					}else{
						oledClean();
						oledDisplay(0, 0, "start bee");
						vTaskResume(bee);
						isRunning=1;
					}
				}
				break;
		}
		osDelay(10);
	}

}





/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  
	oledInit();
	BtnInit();
  	debugInit();
	DWT_Init();
	DhtInit();
	
	uiInit();
	HomePageInit();
  	SensorPageInit();
  	TdPageInit();

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

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */

  //xTaskCreate(handleTask, "ledTask", 512, NULL, osPriorityNormal, NULL);


  xTaskCreate(btnTask, "btnTask", 512, NULL, osPriorityNormal, NULL);
  xTaskCreate(debug.ops.handle, "debugTask", 512, NULL, osPriorityNormal, NULL);
  xTaskCreate(UiTask, "UiTask", 512, NULL, osPriorityNormal, NULL);
  xTaskCreate(UpdateTask, "updateTask", 512, NULL, osPriorityNormal, NULL);
	xTaskCreate(dhtCtl.ops.scan, "dhtTask", 1024, NULL, osPriorityNormal, NULL);
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */

  pageStack.ops.push(&homePage);
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

