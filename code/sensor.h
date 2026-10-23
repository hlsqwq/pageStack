#pragma once




#include "page.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#include "oled.h"
#include "tim.h"
#include "Music.h"
#include "button4_4.h"
#include "debug.h"
#include "td.h"

#include "queue.h"
#include "usart.h"




void SensorPageInit();



typedef struct {
	Page page;
}SensorPage;

extern SensorPage sensorPage;

