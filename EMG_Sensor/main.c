#include "msp.h"
#include <stdint.h>
#include <stdbool.h>

double nADC;

void ADC14_init(void);
void voltmeas(void);

/**
 * main.c
 */

void main(void)
{

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    ADC14_init();
    __enable_irq();                                 // enable all interrupts
    while(1)
    {
        voltmeas();
    }

}


void ADC14_init(void)
{
    P5 -> SEL0 |= 0x20;                     //configure pin 5.5 for A0 input
    P5 -> SEL1 |= 0x20;
    ADC14 -> CTL0 &=~ 0x00000002;       //disable ADC14ENC during configuration
    ADC14 -> CTL0 |= 0x04400110;        //S/H pulse mode, SMCLK, 16 sample clocks
    ADC14 -> CTL1 = 0x00000030;         //14 bit resolution
    ADC14 -> CTL1 |= 0x00000000;        //selecting ADC14CSTARTADDx mem0 REGISTER
    ADC14 -> MCTL[0] = 0x00000000;      //ADC14INCHx = 0 for mem[0]
    ADC14 -> CTL0 |= 0x00000002;        //enable ADC14ENC, starts for the ADC after configuration
}

void voltmeas(void)
{
    float result;

    ADC14->CTL0 |= 1;                   //start conversation
    while(!ADC14->IFGR0 & BIT0);        //wait for conversation to complete
    result = ADC14->MEM[0];             //get the position from the ADC
    nADC = (result*3.3)/16384;          //convert to voltage

}
