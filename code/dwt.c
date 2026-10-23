










#include "dwt.h"




void DWT_Init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}


void delay_us(uint32_t us)
{
    uint32_t start = DWT->CYCCNT;

    uint32_t ticks = us * (SystemCoreClock / 1000000);

    while((DWT->CYCCNT - start) < ticks);
}






