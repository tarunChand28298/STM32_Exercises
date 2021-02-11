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
    RCC->APB2ENR |= (0b1 << 3); //enable clock to port b
    RCC->APB2ENR |= (0b1 << 0); //enable clock to alternate function
    RCC->APB1ENR |= (0b1 << 2); //enable clock to timer 4

    //Set pb6 for t4c1 alternate function output:
    GPIOB->CRL |=  (0b1011 << 24);
    GPIOB->CRL &= ~(0b0100 << 24);

    TIM4->CCER |= (0b11 << 0); //enable channel 1 for timer 4 (oc1 active low)
    TIM4->CR1 |= (0b1 << 7); //Auto-reload preload enable (register is buffered)
    //I don't even know what this above line does.

    TIM4->CCMR1 |= (0b111 << 4); //set t4c1 to pwm2 mode
    
    //look at prescalar, autoreload register and frequency relation for this calculation.
    TIM4->PSC = 288;
    TIM4->ARR = 250;

    TIM4->EGR |= (0b1 << 0); //Update generation (I don't know what this does)
    TIM4->CR1 |= (0b1 << 0); //enable counter for timer 4

    //user timx->ccr1 register to set the value of pwm level
    TIM4->CCR1 = 0;
    while(1)
    {
        TIM4->CCR1++;
        if(TIM4->CCR1 > 250) TIM4->CCR1 = 0;
        Delay(5000);
    }

    return 0;
}
