

#pragma once



#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "usart.h"


typedef struct Item{
	char str[64];
	int len;
}Item;


typedef struct DebugOps{
	void (*add)(Item item);
	void (*handle)(void *arg);
}DebugOps;


typedef struct Debug{
	DebugOps ops;

}Debug;



void debugInit();

extern Debug debug;











