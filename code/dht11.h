







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
#include "debug.h"


#define DHT_MAX_DEPTH 20



typedef struct DhtCtl DhtCtl;

typedef struct DhtOps
{
    void (*scan)();
}DhtOps;

struct DhtCtl
{
	QueueHandle_t q;
    DhtOps ops;
};

void DhtInit();



extern DhtCtl dhtCtl;





















