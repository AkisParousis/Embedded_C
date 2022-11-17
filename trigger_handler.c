#include <stdio.h>
#include <stdint.h>

// 2. Implement the fault handlers.
void HardFault_Handler(void){
	printf("HardFaultException\n");
}

void MemManage_Handler(void){
	printf("MemManageFaultException\n");
}

void BusFault_Handler(void){
	printf("BusFaultException\n");
}

void UsageFault_Handler(void){
	// 4. Analyze fault.
  uint32_t *pUFSR = (uint32_t *)0xE000ED2A;
	printf("UsageFault Exception\n");
	printf("%x\n", *pUFSR);

}

int main(void)
{
    // 1. Enable all exceptions via System Handler Control and State Register.
	uint32_t *pSHCSR = (uint32_t*)0xE000ED24;
	/* We must enable
	 * 18th bit -> UsgFaultException
	 * 17th bit -> BusFaultException
	 * 16th bit -> MemFaultException
	 */
	*pSHCSR |= 458752;

	/* 3. Force processor to run undefined instruction.
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

	for(;;);
}
