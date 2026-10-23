



#include "oled.h"



#define OLED_ADDRESS 0x3c
#define TIMEOUT 100
#define DATA_LEN_MAX 256-1
#define OLED_WR_CMD 0x00
#define OLED_WR_DATA 0x40



ErrorStatus oledSendByte(uint8_t cmd,uint8_t data){
	uint8_t buf[]={cmd,data};

	if(HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDRESS<<1, buf, 2, TIMEOUT)==HAL_OK){
		return SUCCESS;
	}
	return ERROR;
}

ErrorStatus oledSendStr(uint8_t cmd,uint8_t data[],int len){
	if(len>DATA_LEN_MAX){
		return ERROR;
	}
	uint8_t buf[DATA_LEN_MAX+1]={0};
	buf[0]=cmd;
	memcpy(&buf[1],data,len);

	if(HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDRESS<<1, buf, len+1, TIMEOUT)==HAL_OK){
		return SUCCESS;
	}
	return ERROR;
}


void oledSetPos(uint8_t y, uint8_t x) {
    uint8_t pos_buf[3] = {
        0xB0 + y,                 // 页地址命令（0xB0~0xB7）
        0x10 | (x & 0xF0) >> 4,          // 列地址高4位（0x10~0x17）
        (x & 0x0F)                           // 列地址低4位（0x00~0x0F）
    };
    oledSendStr(OLED_WR_CMD, pos_buf,3);
}


void oledFill(uint8_t fill_data){
	uint8_t buf[128]={0};
	memset(buf,fill_data,128);

	for(int i=0;i<8;i++){
		oledSetPos(i, 0);
		oledSendStr(OLED_WR_DATA, buf,128);
	}
}

void oledFillFull(){
	oledFill(0xFF);
}



void oledClean(){
	oledFill(0x00);
}


void oledInit(void) {
    HAL_Delay(100);

    oledSendByte(OLED_WR_CMD, 0xAE);       // 关闭显示
    oledSendByte(OLED_WR_CMD, 0xD5);       // 设置时钟分频
    oledSendByte(OLED_WR_CMD, 0x80);
    oledSendByte(OLED_WR_CMD, 0xA8);       // 设置多路复用
    oledSendByte(OLED_WR_CMD, 0x3F);
    oledSendByte(OLED_WR_CMD, 0xD3);       // 设置显示偏移
    oledSendByte(OLED_WR_CMD, 0x00);
    oledSendByte(OLED_WR_CMD, 0x40);       // 设置起始行
    oledSendByte(OLED_WR_CMD, 0xA1);       // 段重映射（左右镜像）
    oledSendByte(OLED_WR_CMD, 0xC8);       // COM 输出模式（上下镜像）
    oledSendByte(OLED_WR_CMD, 0xDA);       // COM 引脚硬件配置
    oledSendByte(OLED_WR_CMD, 0x12);
    oledSendByte(OLED_WR_CMD, 0x81);       // 设置对比度
    oledSendByte(OLED_WR_CMD, 0xCF);
    oledSendByte(OLED_WR_CMD, 0xD9);       // 设置预充电周期
    oledSendByte(OLED_WR_CMD, 0xF1);
    oledSendByte(OLED_WR_CMD, 0xDB);       // 设置 VCOMH 电压
    oledSendByte(OLED_WR_CMD, 0x30);
    oledSendByte(OLED_WR_CMD, 0xA4);       // 启用全局显示（禁用全亮/全灭）
    oledSendByte(OLED_WR_CMD, 0xA6);       // 设置正常显示（0xAF 亮，0xA7 反显）
    oledSendByte(OLED_WR_CMD, 0x8D);       // 启用电荷泵
    oledSendByte(OLED_WR_CMD, 0x14);
    oledSendByte(OLED_WR_CMD, 0xAF);       // 开启显示

    oledClean();                           // 清屏
}





void oledChinese_16x16(uint8_t y,uint8_t x,uint8_t ch[2][16]){
	for(int i=0;i<2;i++){
		oledSetPos(y+i, x);
		oledSendStr(OLED_WR_DATA, ch[i],16);
	}
}


void oledAscll_8x16(uint8_t y,uint8_t x,const uint8_t ch[16]){
	oledSetPos(y, x);
	for(int i=0;i<16;i++){
		if(i==8)
			oledSetPos(y+1, x);
		oledSendByte(OLED_WR_DATA, ch[i]);
	}
}





int is_chinese(char c)
{
    return (c >= 0xE0 && c <= 0xEF);
}



void oledDisplay(uint8_t y,uint8_t x,char data[]){
	int len=strlen(data);
	
	for(int i=0;i<len;i++){
		if(is_chinese(data[i])){
			if(x+16>128){
				y+=2;
				x=0;
			}
			char tmp[]={data[i],data[i+1],data[i+2],'\0'};
			Chinese* ch=findChinese(tmp);
			if(ch!=NULL){
				oledChinese_16x16(y, x, ch->data);
			}
			i+=2;
			x+=16;
		}else{
			if(x+8>128){
				y+=2;
				x=0;
			}
			oledAscll_8x16(y, x, ascll_8x16[data[i]-' ']);
			x+=8;
		}
	}
}




















