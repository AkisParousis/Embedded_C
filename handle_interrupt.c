#include <stdio.h>
#include <stdint.h>

#define USART3_IRQ 39

int main(void)
{
	uint32_t *pISPR1 = (uint32_t*)0XE000E204;

	*pISPR1 |= (1 << (USART3_IRQ % 32));

	uint32_t *pISER1 = (uint32_t*)0XE000E104;

	*pISER1 |= (1 << (USART3_IRQ % 32));

	for(;;);
}

void USART3_IRQHandler(void){
	printf("In USART3 ISR\n");
}
