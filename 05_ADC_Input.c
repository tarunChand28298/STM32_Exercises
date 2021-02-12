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
    //set adc prescaler to divide by 6
    //Prescaler for the ADC clock must not exceed 14 MHz
    RCC->CFGR |=  (0b10 << 14);
    RCC->CFGR &= ~(0b01 << 14);

    RCC->APB2ENR |= (0b1 << 2); //enable clock to port a
    RCC->APB2ENR |= (0b1 << 9); //enable clock to adc1
    RCC->APB2ENR |= (0b1 << 0); //enable clock to alternate function

    //configure pa1 as input push pull.
    //see datasheet pin definition table.
    //example: ADC12_IN1 means adc1 and adc2 input channel 1 connected to pa1
    GPIOA->CRL |=  (0b1000 << 4);
    GPIOA->CRL &= ~(0b0111 << 4);

    //we can enable interrupt for end of conversion, by writing 1 to 5th bit of adc->cr1 register
    //I am not going to do it because I want to focus only on adc and not interrupts, so I will poll instead

    //set sample rate for adc1 channel 1 to 13.5 cycles:
    ADC1->SMPR2 |=  (0b010 << 5);
    ADC1->SMPR2 &= ~(0b101 << 5);

    //set number of conversions to 1:
    ADC1->SQR1 |=  (0b0000 << 20);
    ADC1->SQR1 &= ~(0b1111 << 20);

    //set first conversion to channel 1 (write the channel number in the first place):
    ADC1->SQR3 |=  (0b0001 << 0);
    ADC1->SQR3 &= ~(0b1110 << 0);
    //subsequent channels will follow
    //suppose second conversion we want to be channel 3, then we will write 0011 in the next place
    
    //turn the adc on:
    ADC1->CR2 |= (0b1 << 0);
    Delay(1000000); // delay amount should be at least (tSTAB)
    ADC1->CR2 |= (0b1 << 0);
    //read the following excerpt from the reference manual! read it carefully, I wasted 2 days because of this:
    //When the ADON bit is set for the first time, it wakes up the ADC from Power Down mode.
    //Conversion starts when ADON bit is set for a second time by software after ADC power-up time (tSTAB).

    while(1)
    {
        ADC1->CR2 |= (0b1 << 22); //start conversion.
        while (!(ADC1->SR & (0b1 << 1))); //wait for end of conversion flag to be set in the status register

        volatile int valueRead = ADC1->DR; //read the value from data register
    }

    return 0;
}
