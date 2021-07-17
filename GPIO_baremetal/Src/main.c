#include "stm32f1xx.h"
// We will perform blink procedure on pin PC13
// Change bit 4 of RCC_APB2ENR to 1

int main(void) {

	// Change bit 4 of RCC_APB2ENR to 1 for initializing clock for PORT-C
	RCC->APB2ENR |= (1<<4);

	// Configure Pin13 on PORT-C
	GPIOC->CRH |= (1<<20) | (1<<21); // Output at 50MHz
	GPIOC->CRH &= ~((1<<22) | (1<<23)); // GPIO Output Mode

	while (1){
		GPIOC->BSRR = (1<<13);
		for (int i=0; i<500000; i++) {} // Delay 1 second... since clock frequency is 50MHz
		GPIOC->BSRR = (1<<29);
		for (int i=0; i<500000; i++) {} // Delay 1 second... since clock frequency is 50MHz
	}
}