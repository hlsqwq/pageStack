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
#include "dht11.h"

#include "queue.h"
#include "usart.h"




void TdPageInit();



typedef struct {
	Page page;
}TdPage;

extern TdPage tdPage;


