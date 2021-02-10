#include <stm32f1xx.h>

void Delay(int count)
{
    volatile int currentCount;
    for(currentCount = count; currentCount > 0; currentCount--)
    {
        asm("nop");
    }
}

int main(void)
{
    //look at datasheet block diagram and see which bus the port is connected.
    //then look at reference manual to select the right bit to enable clock from rcc.
    RCC->APB2ENR |= (0b1 << 3); //enable clock to port b

    //look at reference manual for gpio registers for this part
    //configure pb12 as output
    GPIOB->CRH |=  (0b0011 << 16);
    GPIOB->CRH &= ~(0b1100 << 16);

    while(1)
    {
        //again, look at reference manual for gpio registers.
        //we can use odr register to write output
        GPIOB->ODR ^= (0b1 << 12);
        //note: we can use bsrr to perform atomic write

        Delay(100000);
    }

    return 0;
}
