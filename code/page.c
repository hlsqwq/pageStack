
#include "page.h"





PageStack pageStack;
QueueSetHandle_t qs;




void pageStackPush(Page *page){
	if(page==NULL || page->ops.onEnter==NULL){
		return;
	}
	if(pageStack.top==0){
		pageStack.list[pageStack.top++]=page;
		page->pageState=running;
		page->ops.onEnter();
		return;
	}
	Page *p=pageStack.list[pageStack.top-1];
	if(pageStack.top<PAGE_STACK_MAX_DEPTH){
		
		if(p->pageState==running){
			p->ops.onExit();
			p->pageState=suspend;
		}

		pageStack.list[pageStack.top++]=page;
		page->pageState=running;
		page->ops.onEnter();
	}

}

void pageStackPop(){
	if(pageStack.top>1){
		Page *p=pageStack.list[pageStack.top-1];
		if(p!=NULL && p->ops.onExit!=NULL){
			p->pageState=suspend;
			p->ops.onExit();
		}
		pageStack.top--;

		Page *pp=pageStack.list[pageStack.top-1];
		if(pp!=NULL && pp->pageState==suspend){
			pp->ops.onEnter();
			pp->pageState=running;
		}
	}
}




void UiTask(void *arg)
{
    while (1) {
        Page *current = NULL;

        if (pageStack.top > 0) {
            current = pageStack.list[pageStack.top - 1];
        }

        if (current != NULL) {

            if (current->ops.onDraw != NULL && current->dirty==1) {
				current->ops.onDraw();
				current->dirty=0;
            }
        }

        osDelay(500);
    }
}

void UpdateTask(void *arg)
{
    while (1) {
        Page *current = NULL;

        if (pageStack.top > 0) {
            current = pageStack.list[pageStack.top - 1];
        }

        if (current != NULL) {
            if (current->ops.onUpdate != NULL) {
                current->ops.onUpdate();
            }
        }

        osDelay(10);
    }
}




void uiInit(){
	pageStack.top=0;
	qs=xQueueCreateSet(BTN_MAX_DEPTH+DHT_MAX_DEPTH);
	xQueueAddToSet(btnCtl.q, qs);
	xQueueAddToSet(dhtCtl.q, qs);
	pageStack.ops.push=pageStackPush;
	pageStack.ops.pop=pageStackPop;
}










