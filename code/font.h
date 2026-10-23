

#pragma once

#include <stdint.h>
#include <stddef.h>


typedef struct{
	char id[4];
	uint8_t data[2][16];
}Chinese;



extern const Chinese ch_16x16[];
extern const uint8_t ascll_8x16[][16];


Chinese* findChinese(char ch[]);






























