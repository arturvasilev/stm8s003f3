#ifndef ALTERNATOR_ISR_HANDLERS_C
#define ALTERNATOR_ISR_HANDLERS_C

#include "utils.h"
#include "../regmap.h"

// Interrupt-handlers defines
void TRAP_handler() __trap {}

// Clock controller
void CLK_handler() __interrupt(2) {}

// Port A external interrupts
void EXTI0_handler() __interrupt(3) {}
// Port B external interrupts
void EXTI1_handler() __interrupt(4) {}
// Port C external interrupts
void EXTI2_handler() __interrupt(5) {}
// Port D external interrupts
void EXTI3_handler() __interrupt(6) {}
// Port E external interrupts
void EXTI4_handler() __interrupt(7) {}

// SPI -- End of transfer
void SPI_handler() __interrupt(10) {}

// TIM2 update/overflow
void TIM2_UOVFL_handler() __interrupt(13) {}
void TIM2_CCMP_handler() __interrupt(14) {}

// UART1
// Tx complete
void UART1_TxCompl_handler() __interrupt(17) {}
// Recieve register DATA FULL
void UART1_DFULL_handler() __interrupt(18) {}

// ADC
void ADC1_handler() __interrupt(22) {
  ADC_process();

  // Очистим флаг EOC
  ADC_CSR.EOC = false;
 
  // Пнём ADC для следующей конвертации
  ADC_CR1.ADON = true;
}

void TIM4_UOVFL_handler() __interrupt(23) {
  // Очистить флаг о прерывании
  TIM4_SR.UIF = false;
}

#endif  // ALTERNATOR_ISR_HANDLERS_C
