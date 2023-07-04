#ifndef ISR_HANDLERS_H
#define ISR_HANDLERS_H

// Interrupt-handlers defines
void TRAP_handler() __trap;

// Clock controller
void CLK_handler() __interrupt(2);

// Port A external interrupts
void EXTI0_handler() __interrupt(3);
// Port B external interrupts
void EXTI1_handler() __interrupt(4);
// Port C external interrupts
void EXTI2_handler() __interrupt(5);
// Port D external interrupts
void EXTI3_handler() __interrupt(6);
// Port E external interrupts
void EXTI4_handler() __interrupt(7);

// SPI -- End of transfer
void SPI_handler() __interrupt(10);

// TIM2 update/overflow
void TIM2_UOVFL_handler() __interrupt(13);
void TIM2_CCMP_handler() __interrupt(14);

// UART1
// Tx complete
void UART1_TxCompl_handler() __interrupt(17);
// Recieve register DATA FULL
void UART1_DFULL_handler() __interrupt(18);

// ADC
void ADC1_handler() __interrupt(22);

#endif  // ISR_HANDLERS_H
