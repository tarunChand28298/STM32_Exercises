#include <stm32f1xx.h>

//name of handler should be [Acronym]_IRQ_IRQHandler.
//see [Vector table for connectivity line devices] for acronym.
void EXTI4_IRQ_IRQHandler()
{
    EXTI->PR |= (0b1 << 12); //clear the interrupt

    //do whatever
    GPIOC->ODR ^= (0b1 << 13);
}

int main(void)
{
    RCC->APB2ENR |= (0b1 << 3); //Enable clock to port b
    RCC->APB2ENR |= (0b1 << 0); //Enable clock to alternate function
    
    //Look at reference manual for which exti to set to which value.
    //exti1 for pin 0-3, exti2 for 4-7, exti3 for 8-11 and exti4 for 12-15
    //Configure pb12 for interrupt:
    AFIO->EXTICR[3] |=  (0b0001 << 0);
    AFIO->EXTICR[3] &= ~(0b1110 << 0);

    EXTI->RTSR |= (0b1 << 12); //rtsr for rising edge and ftsr for falling edge
    EXTI->IMR |= (0b1 << 12); //enable interrupt mask on exti side
    NVIC_EnableIRQ(10); //Enable interrupt from nvic side.
    //look at [Vector table for connectivity line devices] to pass in this param


    //setting pc13 as output (see 01 for setting gpio pin as output)
    RCC->APB2ENR |= (0b1 << 4);
    GPIOC->CRH |=  (0b0011 << 20);
    GPIOC->CRH &= ~(0b1100 << 20);
    
    while(1);
    return 0;
}
