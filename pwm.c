#include"LPC11XX.H"
#define KEY1_DOWN() ((LPC_GPIO3->DATA&(1<<0))!=(1<<0))
#define KEY2_DOWN() ((LPC_GPIO3->DATA&(1<<1))!=(1<<1))
void CT32B1_Init(uint32_t interval)
{
	LPC_SYSCON->SYSAHBCLKCTRL |=(1<<16);
	LPC_IOCON ->R_PIO1_2&=~(0x07);
	LPC_IOCON ->R_PIO1_2|=0x03;
	LPC_SYSCON ->SYSAHBCLKCTRL &=~(1<<16);
	LPC_SYSCON ->SYSAHBCLKCTRL |=(1<<10);
	LPC_TMR32B1->TCR=0x02;
	LPC_TMR32B1->PR=0;
	LPC_TMR32B1->MCR=0x02<<9;
	LPC_TMR32B1->PWMC=0x02;
	LPC_TMR32B1->MR1=interval/2;
	LPC_TMR32B1->MR3=interval;
	LPC_TMR32B1->TCR =0x01;
}
void PIOINT1_IRQHandler()
{
	if((LPC_GPIO1->DATA&(1<<0))==(1<<0))
	{
		while(KEY1_DOWN ());
		(LPC_TMR32B1->MR1)-=10;
		LPC_GPIO0->IC=(1<<0);
	}
	if((LPC_GPIO1->DATA&(1<<1))==(1<<1))
	{
		while(KEY2_DOWN ());
		(LPC_TMR32B1->MR1)+=10;
		LPC_GPIO0->IC=(1<<1);
}
int main()
{
	CT32B1_Init(SystemCoreClock /1000);
  LPC_IOCON ->PIO3_0&~(0x07);
	LPC_IOCON ->DIR&=~(1<<0);
	LPC_IOCON ->PIO3_1&~(0x07);
	LPC_IOCON ->DIR&=~(1<<1);
	NVIC_EnableIRQ (EINT0_IRQn );
	while(1);
	}
}