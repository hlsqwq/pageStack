


#include "button4_4.h"

static char map[10][10];


static Item item;

BtnCtl btnCtl;



void BtnScan(BtnCtl *self)
{
    if (self == NULL)
    {
        return;
    }

	for(int i=7;i>3;i--){
		line l=self->lines[i];
		HAL_GPIO_WritePin(l.port, l.pin, GPIO_PIN_RESET);
		osDelay(1);
		for(int j=3;j>-1;j--){
			line ll=self->lines[j];
			if(HAL_GPIO_ReadPin(ll.port, ll.pin)==GPIO_PIN_RESET){

				osDelay(20);
				if(HAL_GPIO_ReadPin(ll.port, ll.pin)==GPIO_PIN_RESET){
					if(pdPASS==xQueueSend(self->q, &map[i][j], 0)){
						sprintf(item.str,"user press %c",map[i][j]);
						item.len=strlen(item.str);
						debug.ops.add(item);
						
					}
				}

				while (HAL_GPIO_ReadPin(ll.port, ll.pin)== GPIO_PIN_RESET) {
                	osDelay(5);
               	}
			}
		}
		HAL_GPIO_WritePin(l.port, l.pin, GPIO_PIN_SET);
	}

}





void BtnInit()
{

	map[7][3]='1';	
	map[6][3]='4';
	map[5][3]='7';
	map[4][3]='*';

	map[7][2]='2';	
	map[6][2]='5';
	map[5][2]='8';
	map[4][2]='0';
	
	map[7][1]='3';	
	map[6][1]='6';
	map[5][1]='9';
	map[4][1]='*';
	
	map[7][0]='A';	
	map[6][0]='B';
	map[5][0]='C';
	map[4][0]='D';
	
    // ��ɨ�躯��
    btnCtl.ops.scan = BtnScan;
	btnCtl.q=xQueueCreate(BTN_MAX_DEPTH, sizeof(char));

    // 0
    btnCtl.lines[0] = (line){b0_GPIO_Port, b0_Pin};

    // 1
    btnCtl.lines[1] = (line){b1_GPIO_Port, b1_Pin};

    // 2
    btnCtl.lines[2] = (line){b2_GPIO_Port, b2_Pin};

    // 3
    btnCtl.lines[3] = (line){b3_GPIO_Port, b3_Pin};

    // 4
    btnCtl.lines[4] = (line){b4_GPIO_Port, b4_Pin};

    // 5
    btnCtl.lines[5] = (line){b5_GPIO_Port, b5_Pin};

    // 6
    btnCtl.lines[6] = (line){b6_GPIO_Port, b6_Pin};

    // 7
    btnCtl.lines[7] = (line){b7_GPIO_Port, b7_Pin};


}








