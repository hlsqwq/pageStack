
#pragma once



#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "tim.h"
#include "queue.h"
#include "usart.h"

#include "page.h"
#include "oled.h"
#include "sensor.h"
#include "Music.h"
#include "button4_4.h"
#include "debug.h"



void HomePageInit();



typedef struct {
	Page page;
}HomePage;

extern HomePage homePage;




