


#include "home.h"



HomePage homePage;


char ch=' ';



static void onEnter(){
	homePage.page.dirty=1;
	oledClean();
}


static void onUpdate(){
	ch=' ';
	QueueSetMemberHandle_t qq=xQueueSelectFromSet(qs,0);
	if(homePage.page.pageState!=running || qq==NULL || qq!=btnCtl.q){
		return;
	}
	//todo 队列集的结果需要判断来源
	xQueueReceive(qq, &ch, 0);
	switch(ch){
	case '0':
	case '1':
	case '2':
	case '3':
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
		pageStack.ops.push(&sensorPage);
		break;
	}
}

static void onDraw(){
	if(homePage.page.pageState!=running){
		return;
	}
	oledDisplay(0, 0, "hello world");
	oledDisplay(6, 0, "按任意键继续");
}


static void onExit(){
	homePage.page.dirty=0;
}


void HomePageInit(){
	strcpy(homePage.page.name,"home");
	homePage.page.tid=NULL;
	homePage.page.dirty=0;
	homePage.page.pageState=suspend;
	homePage.page.ops.onEnter=onEnter;
	homePage.page.ops.onUpdate=onUpdate;
	homePage.page.ops.onDraw=onDraw;
	homePage.page.ops.onExit=onExit;
}



