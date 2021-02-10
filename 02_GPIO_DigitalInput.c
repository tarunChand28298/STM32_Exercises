#include <stm32f1xx.h>

int main(void)
{
    //look at datasheet block diagram and see which bus the port is connected.
    //then look at reference manual to select the right bit to enable clock from rcc.
    RCC->APB2ENR |= (0b1 << 3); //enable clock to port b
    RCC->APB2ENR |= (0b1 << 4); //enable clock to port c


    //look at reference manual for gpio registers for this part
    //configure pb12 as input
    GPIOB->CRH |=  (0b1000 << 16);
    GPIOB->CRH &= ~(0b0111 << 16);

    //configure pc13 as output
    GPIOC->CRH |=  (0b0011 << 20);
    GPIOC->CRH &= ~(0b1100 << 20);

    while(1)
    {
        //again, look at reference manual for gpio registers.
        //we can use idr register to read the value for input
        if((GPIOB->IDR) & (0b1 << 12)) GPIOC->ODR |= (0b1 << 13);
        else GPIOC->ODR &= ~(0b1 << 13);
    }

    return 0;
}
