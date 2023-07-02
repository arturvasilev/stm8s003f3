#ifndef DEFINES_H
#define DEFINES_H

/* Interrupt commands */
#define enableInterrupts() \
  { __asm__("rim\n"); } /* enable interrupts */
#define disableInterrupts() \
  { __asm__("sim\n"); }           /* disable interrupts */
#define rim() enableInterrupts()  /* enable interrupts */
#define sim() disableInterrupts() /* disable interrupts */
#define nop() \
  { __asm__("nop\n"); } /* No Operation */
#define trap() \
  { __asm__("trap\n"); } /* Trap (soft IT) */
#define wfi() \
  { __asm__("wfi\n"); } /* Wait For Interrupt */
#define halt() \
  { __asm__("halt\n"); } /* Halt */
#define iret() \
  { __asm__("iret\n"); } /* Halt */

#endif  // DEFINES_H
