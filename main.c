#include "MDR32F9x.h"
#include "system_MDR32F9Qx.h"
long check = 0;
void InitPortLED(void)
{
    MDR_PORTD->FUNC &= ~((0x3FF << (10 << 1)));   /* Port */
    MDR_PORTD->ANALOG |= (1<<14|1<<13|1<<12|1<<11|1<<10);                     /* Digital */
    MDR_PORTD->PWR |= (0x155 << (10 << 1));       /* Slow */
    MDR_PORTD->RXTX &= ~(1<<14|1<<13|1<<12|1<<11|1<<10);
    MDR_PORTD->OE |= (1<<14|1<<13|1<<12|1<<11|1<<10);
}

int main()
{
    SystemInit();
    InitPortLED();
    while(1)
    {
        check++;
        if(check == 1000)
            MDR_PORTD->RXTX |= 1<<14|1<<10|1<<12;//vkl diod
        if(check == 4000)
        {
            check = 0;
            MDR_PORTD->RXTX &= (0<<11|0<<13);
        }
    }
}
