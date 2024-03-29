#include <stdio.h>
#include <stdint.h>

// 2. Implement the fault handlers
void HardFault_Handler(void){
	printf("HardFaultException\n");
}

void MemManage_Handler(void){
	printf("MemManageFaultException\n");
}

void BusFault_Handler(void){
	printf("BusFaultException\n");
}

__attribute__ ((naked)) void UsageFault_Handler(void){
	__asm("MRS r0, MSP");
	__asm("B UsageFault_Handler_c");
}

void UsageFault_Handler_c(uint32_t *pBaseStackPointer){

	uint32_t *pUFSR = (uint32_t *)0xE000ED2A;
	printf("UsageFault Exception\n");
	printf("%lx\n", *pUFSR);
	
	if (*pUFSR ==200){
		printf("Division by zero Fault\n");
	}
	
	printf("Value of R0 is: %lx\n", pBaseStackPointer[0]);
	printf("Value of R1 is: %lx\n", pBaseStackPointer[1]);
	printf("Value of R2 is: %lx\n", pBaseStackPointer[2]);
	printf("Value of R3 is: %lx\n", pBaseStackPointer[3]);
	printf("Value of R12 is: %lx\n", pBaseStackPointer[4]);
	printf("Value of LR is: %lx\n", pBaseStackPointer[5]);
	printf("Value of PC is: %lx\n", pBaseStackPointer[6]);
	printf("Value of XPSR is: %lx\n", pBaseStackPointer[7]);

}

float divide(int x, int y){
	return x/y;
}

int main(void)
{
    // 1a. Enable all exceptions via System Handler Control and State Register
	uint32_t *pSHCSR = (uint32_t*)0xE000ED24;
	/* We must enable
	 * 18th bit -> UsgFaultException
	 * 17th bit -> BusFaultException
	 * 16th bit -> MemFaultException
	 */
	*pSHCSR |= 458752;

	// 1b. Enable DIV_0_TRP
	uint32_t *pCCR = (uint32_t *)0xE000ED14;
	// DIV_0_TRP, bit[4] Controls the trap on divide by 0
	*pCCR |= (1<<4);
		
	// Depending the error you want to trigger, comment the other. E.g. to run 3b, comment 3a instructions. 
	
	/* 3a. Force processor to run undefined instruction.
	 * According to ARMV7 Architecture 0xFFFFFFFF is not an opcode.
	 * We assign this opcode value to an instruction.
	 * M4 processor can fetch instructions from both sram and flash.
	 * We assign the SRAM address to a function pointer in order
	 * for the processor to execute it.
	 */
	uint16_t *pSRAM = (uint16_t*)0x20010005;
	*pSRAM = 0xFFFF;

	void (*sram_address)(void) = (void*)0x20010005;
	sram_address();

	// 3b. Attempt to divide by zero
	divide(5,0);

	for(;;);
}
