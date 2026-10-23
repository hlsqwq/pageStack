











#include "dht11.h"



DhtCtl dhtCtl;



#define read() HAL_GPIO_ReadPin(dht11_GPIO_Port,dht11_Pin)
#define on() HAL_GPIO_WritePin(dht11_GPIO_Port, dht11_Pin, GPIO_PIN_SET)
#define off() HAL_GPIO_WritePin(dht11_GPIO_Port,dht11_Pin,GPIO_PIN_RESET)



void setGpioMode(uint32_t mode,uint32_t pull){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/*Configure GPIO pin : dht11_Pin */
	GPIO_InitStruct.Pin = dht11_Pin;
	GPIO_InitStruct.Mode = mode;
	GPIO_InitStruct.Pull = pull;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(dht11_GPIO_Port, &GPIO_InitStruct);
}



uint8_t dht11ReadByte(){
	uint8_t val=0;
	for(int i=0;i<8;i++){


		//等待低电平
		uint8_t flag=0;
		while(read()==GPIO_PIN_SET){
			if(++flag>100){
				return 0;
			}
			delay_us(1);
		}
		
		//等待高电平
		flag=0;
		while(read()==GPIO_PIN_RESET){
			if(++flag>100){
				return 0;
			}
			delay_us(1);
		}
			
		delay_us(40);
		

		if(read()){
			val|=(1<<(7-i));
		}
	}
	return val;	
}




/**
返回数据data
0 温度
1 湿度
**/
HAL_StatusTypeDef dht11ReadData(uint8_t data[]){
	setGpioMode(GPIO_MODE_OUTPUT_PP, GPIO_NOPULL);
	off();
	HAL_Delay(20);
	on();
	delay_us(30);
	setGpioMode(GPIO_MODE_INPUT, GPIO_NOPULL);

	
	uint8_t flag=0;
	while(read()){
		if(++flag>100){
			return HAL_ERROR;
		}
		delay_us(1);
	}

	flag=0;
	while(!read()){
		if(++flag>100){
			return HAL_ERROR;
		}
		delay_us(1);
	}

	
	uint8_t humidity_dig=dht11ReadByte();
	uint8_t humidity_deci=dht11ReadByte();
	uint8_t temperture_dig=dht11ReadByte();
	uint8_t temperture_deci=dht11ReadByte();
	uint8_t check=dht11ReadByte();

	if(check!=humidity_deci+humidity_dig
		+temperture_deci+temperture_dig){
		return HAL_ERROR;
	}

	data[0]=temperture_dig;
	data[1]=humidity_dig;
		
	return HAL_OK;
}


static uint8_t td[2];
static Item item;
static void scan(){
	while(1){
		if(HAL_OK==dht11ReadData(td)){
			BaseType_t ret = xQueueSend(dhtCtl.q, td, 0);
			sprintf(item.str,"temperture_dig:%d,humidity_dig:%d,q:%d",td[0],td[1],(int)ret);
			item.len=strlen(item.str);
			debug.ops.add(item);
		}else{
			sprintf(item.str,"dht11 error");
			item.len=strlen(item.str);
			debug.ops.add(item);
		}
		osDelay(3000);
	}
}




void DhtInit(){
	dhtCtl.q=xQueueCreate(DHT_MAX_DEPTH, sizeof(uint8_t)*2);
	dhtCtl.ops.scan=scan;
}









