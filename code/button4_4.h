
#pragma once

#include "main.h"
#include "gpio.h"
#include "cmsis_os.h"
#include <stdint.h>
#include "debug.h"


#define BTN_MAX_DEPTH 20


typedef struct line
{
    GPIO_TypeDef *port;
    uint16_t pin;
} line;

typedef struct BtnCtl BtnCtl;

typedef struct Ops
{
    void (*scan)(BtnCtl *self);
} Ops;

struct BtnCtl
{
    line lines[8];
	QueueHandle_t q;
    Ops ops;
};

void BtnInit();
void BtnScan(BtnCtl *self);



extern BtnCtl btnCtl;



