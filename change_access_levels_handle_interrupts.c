#include <stdio.h>
#include <stdint.h>

void generate_interrupt()
{
  /* These addresses cannot be changed during NPAL, so HardFault error will occur. */
	uint32_t *pSTIR = (uint32_t*)0xE000EF00;
	uint32_t *pISER0 = (uint32_t*)0xE000E100;

	//enable IRQ3 interrupt
	*pISER0 |= (1 << 3);

	*pSTIR = (3 & 0x1FF);

}


void change_access_level_unpriv()
{
	/* Change access level to non privileged causing a HardFaultError */

	//read
	__asm volatile ("MRS R0,CONTROL");
	//modify
	__asm volatile ("ORR R0,R0,#0X01");
	//write
	__asm volatile ("MSR CONTROL,R0");
}

void change_access_level_priv()
{
	/* Change access level to privileged causing a RTC_WKUP Interrupt */
	//write
	//read
		__asm volatile ("MRS R0,CONTROL");
		//modify
		__asm volatile ("AND R0,R0, #0X00");
		//write
		__asm volatile ("MSR CONTROL,R0");
}

int main(void)
{
	/* Cause a HardFault Error */
	printf("In thread mode : before interrupt\n");

	change_access_level_unpriv();

	generate_interrupt();

	printf("In thread mode : after interrupt\n");

	return 0;
}

void RTC_WKUP_IRQHandler(void){
	printf("RTC_WKUP Interrupt detected\n");
	while(1);
}

void HardFault_Handler(void){
	printf("Hard fault detected\n");

	/* To cause an RTC_WKUP Interrupt */
	change_access_level_priv();
}
