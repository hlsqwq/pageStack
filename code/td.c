







#include "td.h"



TdPage tdPage;


static char ch=' ';


static uint8_t t_and_h[2];

static void onEnter(){
	oledClean();
	ch=' ';
	tdPage.page.dirty=1;
	t_and_h[0]=0;
	t_and_h[1]=0;

	
}







static void onUpdate(){
	ch=' ';



	QueueSetMemberHandle_t qq=xQueueSelectFromSet(qs,0);
	if(tdPage.page.pageState!=running || qq==NULL){
		return;
	}
	if(qq==dhtCtl.q){
		xQueueReceive(qq, t_and_h, 0);
		tdPage.page.dirty=1;
	}else if(qq==btnCtl.q){
		xQueueReceive(qq, &ch, 0);
	}

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
		pageStack.ops.pop();
		break;
	}

	
}

static void onDraw(){
	if(tdPage.page.pageState!=running){
		return;
	}
	static char s1[20];
	static char s2[20];
	sprintf(s1,"温度：%d",t_and_h[0]);
	sprintf(s2,"湿度：%d",t_and_h[1]);
	oledDisplay(0, 0, s1);
	oledDisplay(2, 0, s2);	

	oledDisplay(6, 0, "按0返回");
}


static void onExit(){
	tdPage.page.dirty=0;


}


void TdPageInit(){
	strcpy(tdPage.page.name,"td");
	tdPage.page.tid=NULL;
	tdPage.page.pageState=suspend;
	tdPage.page.dirty=0;

	
	tdPage.page.ops.onEnter=onEnter;
	tdPage.page.ops.onUpdate=onUpdate;
	tdPage.page.ops.onDraw=onDraw;
	tdPage.page.ops.onExit=onExit;


}





