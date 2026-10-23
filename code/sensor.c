







#include "sensor.h"



SensorPage sensorPage;


static char ch=' ';



static void onEnter(){
	ch=' ';
	sensorPage.page.dirty=1;
	oledClean();
}


static void beeTask(void *arg)
{	
	while(1){
		MUSIC_Analysis();
	}
}



static volatile int isRunning=0;
static volatile TaskHandle_t bee=NULL;

static void onUpdate(){

	ch=' ';
	QueueSetMemberHandle_t qq=xQueueSelectFromSet(qs,0);
	if(sensorPage.page.pageState!=running || qq==NULL || qq!=btnCtl.q){
		return;
	}
	
	xQueueReceive(qq, &ch, 0);
	switch(ch){
	case '0':
		pageStack.ops.pop();
		break;
	case '1':
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		break;
	case '2':
		if(bee==NULL){
			BaseType_t ret = xTaskCreate(beeTask, "beeTask", 512, NULL, osPriorityNormal, &bee);
			isRunning=1;
		}else{
			if(isRunning){
				vTaskSuspend(bee);
				BSP_Buzzer_SetFrequency(0);
				isRunning=0;
			}else{
				vTaskResume(bee);
				isRunning=1;
			}
		}
		sensorPage.page.dirty=1;
		break;
	case '3':
		pageStack.ops.push(&tdPage);
		break;
	case '4':		
	case '5':		
	case '6':		
	case '7':		
	case '8':		
	case '9':		
	case 'A':		
	case 'B':		
	case 'C':		
	case 'D':		
	case '*':		
	case '#':
		break;
	}
}

static void onDraw(){
	if(sensorPage.page.pageState!=running){
		return;
	}
	oledDisplay(0, 0, "1.灯");
	if(bee==NULL){
		oledDisplay(2, 0, "2.play music");	

	}else{
		if(isRunning){
			oledDisplay(2, 0, "2.suspend music");	
		}else{
			oledDisplay(2, 0, "2.play music");	
		}
	}
	oledDisplay(4, 0, "3.温湿度");		
	oledDisplay(6, 0, "按0返回");
}


static void onExit(){
	sensorPage.page.dirty=0;


}


void SensorPageInit(){
	strcpy(sensorPage.page.name,"sensor");
	sensorPage.page.tid=NULL;
	sensorPage.page.pageState=suspend;
	sensorPage.page.dirty=0;

	
	sensorPage.page.ops.onEnter=onEnter;
	sensorPage.page.ops.onUpdate=onUpdate;
	sensorPage.page.ops.onDraw=onDraw;
	sensorPage.page.ops.onExit=onExit;

}




