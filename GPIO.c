#include "lib/include.h"
volatile uint16_t ui32Loop;
unsigned long Led;
extern void Delay(void)
{
    unsigned long volatile time;
    time = 800000;
    while(time)
    {
     time--;
    }
}
extern void Configurar_GPIO(void)
{
    SYSCTL->RCGCGPIO |=(1<<5); 
    GPIOF->LOCK= 0x4C4F434B;   // 2) unlock GPIO Port F
    GPIOF->CR = 0x1F;           // allow changes to PF4-0
    GPIOF->AMSEL = 0x00;        // 3) disable analog on PF
    GPIOF->PCTL = 0x00000000;   // 4) PCTL GPIO on PF4-0
    GPIOF->DIR = 0x0E;          // 5) PF4,PF0 in, PF3-1 out
    GPIOF->AFSEL = 0x00;        // 6) disable alt funct on PF7-0
    GPIOF->PUR = 0x11;          // enable pull-up on PF0 and PF4
    GPIOF->DEN = 0x1F;          // 7) enable digital I/O on PF4-0
              //operacione con bitwise
                        // N      J       F     B
    SYSCTL->RCGCGPIO |= (1<<12)|(1<<8)|(1<<5)|(1<<1);
    SYSCTL->RCGCGPIO  = SYSCTL->RCGCGPIO | 0x0004;//0x1122 | 0x0004 = 0x1126
    SYSCTL->RCGCGPIO  = 0x1126;
    while((SYSCTL->PRGPIO&0x00000100) == 0){;} // allow time for clock to stabilize
    ui32Loop = SYSCTL->RCGCGPIO;
    //Salidas
    GPIOB_AHB->DIR |= (1<<4);
    GPIOF_AHB->DIR |= (1<<4) | (1<<0);
        GPION->DIR |= (1<<1) | (1<<0);
    //Entradas
    GPIOJ_AHB->DIR |= (0<<1)|(0<<0);
    GPIOJ_AHB->PUR  |= (1<<1)|(1<<0);//Estado alto
    //Funciones alternativas
    GPIOB_AHB->AFSEL |= 0x00;
    GPIOJ_AHB->AFSEL |= 0x00;
        GPION->AFSEL |= 0x00;
    GPIOF_AHB->AFSEL |= 0x00;
    //Tabla con GPIOCTL
    GPIOB_AHB->PCTL |= 0x00;
    GPIOJ_AHB->PCTL |= 0x00;
        GPION->PCTL |= 0x00;
    GPIOF_AHB->PCTL |= 0x00;
    //Habilitarlos como digitales
    GPIOB_AHB->DEN |= (1<<4);
    GPIOJ_AHB->DEN |= (1<<1) | (1<<0);
        GPION->DEN |= (1<<1) | (1<<0);
    GPIOF_AHB->DEN |= (1<<4) | (1<<0);
    //GPION->DIR  = 0x03; 
    //GPION->DEN  = 0x03;
}
extern void blink(uint8_t LED, uint16_t periodo)
{
    
}
