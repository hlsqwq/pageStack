


#include "debug.h"

static QueueHandle_t q;

Debug debug;

static Item item;



void add(Item item){
	if(item.len>63){
		return;
	}
	if(q==NULL){
		return;
	}
	item.str[63]='\0';
	xQueueSend(q, &item, 10);
}

void handle(void *arg){
	(void)arg;
	while(1){
		if(xQueueReceive(q, &item, portMAX_DELAY)==pdPASS){
			HAL_UART_Transmit_IT(&huart1, (uint8_t *)item.str, (uint16_t)item.len);
		}
	}
}



void debugInit(){
	q=xQueueCreate(10, sizeof(Item));
	if (q == NULL) {
        Error_Handler();
    }
	debug.ops.add=add;
	debug.ops.handle=handle;
}

