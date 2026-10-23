#pragma once



#include "main.h"
#include <string.h>
#include <stdio.h> 
#include <stdbool.h>
#include <stddef.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "usart.h"
#include "button4_4.h"
#include "dht11.h"

#define PAGE_STACK_MAX_DEPTH 10



typedef struct PageStack PageStack;
typedef struct Page Page;


typedef enum{
	running,
	suspend
}PageState;

typedef struct{
	void (*onEnter)();
	void (*onUpdate)();
	void (*onDraw)();
	void (*onExit)();
}PageOps;


struct Page{
	char name[32];
	volatile int dirty;
	TaskHandle_t tid;
	PageState pageState;
	PageOps ops;
};

typedef struct{
	void (*push)(Page *page);
	void (*pop)();
}PageStackOps;


struct PageStack{
	int top;
	Page *list[PAGE_STACK_MAX_DEPTH];
	PageStackOps ops;
};




void UiTask(void *arg);
void UpdateTask(void *arg);

void uiInit();

extern PageStack pageStack;
extern QueueSetHandle_t qs;


