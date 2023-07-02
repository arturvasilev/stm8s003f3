#ifndef REGMAP_H
#define REGMAP_H

#error "Проверить регистровую карту с документацией!"

#include <stdint.h>
#include <stdbool.h>

volatile uint8_t __at(0x4000) EEPROM[128];

typedef struct {
  uint8_t ODR;
  uint8_t IDR;
  uint8_t DDR;
  uint8_t CR1;
  uint8_t CR2;
} GPIO_t;

volatile GPIO_t __at(0x5000) GPIOA;
volatile GPIO_t __at(0x5005) GPIOB;
volatile GPIO_t __at(0x500A) GPIOC;
volatile GPIO_t __at(0x500F) GPIOD;
volatile GPIO_t __at(0x5014) GPIOE;
volatile GPIO_t __at(0x5019) GPIOF;

// External interrupt control register 1
enum enEXTI_CR {
  Falling_LowLevel = 0b00,
  Rising = 0b01,
  Falling = 0b10,
  RisingFalling = 0b11
};
typedef struct {
  uint8_t PAIS : 2;
  uint8_t PBIS : 2;
  uint8_t PCIS : 2;
  uint8_t PDIS : 2;
} EXTI_CR1_t;
volatile EXTI_CR1_t __at(0x50A0) EXTI_CR1;
// External interrupt control register 2
typedef struct {
  uint8_t PEIS : 2;
  uint8_t TLIS : 1;  // Top level interrupt sensoboloty
} EXTI_CR2_t;
volatile EXTI_CR2_t __at(0x50A1) EXTI_CR2;

// Internal clock control register
volatile uint8_t __at(0x50C0) CLK_ICKR;
// External clock control register
volatile uint8_t __at(0x50C1) CLK_ECKR;

// Clock master status register
volatile uint8_t __at(0x50C3) CLK_CMSR;
// Clock master switch register
volatile uint8_t __at(0x50C4) CLK_SWR;
// Clock switch control register
volatile uint8_t __at(0x50C5) CLK_SWCR;
// Clock divider register
volatile uint8_t __at(0x50C6) CLK_CKDIVR;
// Peripheral clock gating register 1
volatile struct {
  uint8_t I2C : 1;
  uint8_t SPI : 1;
  uint8_t UART1234 : 2;
  uint8_t TIM46 : 1;
  uint8_t TIM25 : 1;
  uint8_t TIM3 : 1;
  uint8_t TIM1 : 1;
} __at(0x50C7) CLK_PCKENR1;
// Clock security system register
volatile uint8_t __at(0x50C8) CLK_CSSR;
// Cofigurable clock control register
volatile uint8_t __at(0x50C9) CLK_CCOR;
// Peripheral clock gating register 2
volatile struct {
  uint8_t __Reserved : 2;
  uint8_t AWU : 1;
  uint8_t ADC : 1;
} __at(0x50CA) CLK_PCKENR2;
// HSI clock calibration trimming register
volatile uint8_t __at(0x50CC) CLK_HSITRIMR;

// TIM4 control register 1
volatile uint8_t __at(0x5340) TIM4_CR1;
// TIM4 interrupt enable register
volatile uint8_t __at(0x5343) TIM4_IER;
// TIM4 status register
volatile uint8_t __at(0x5344) TIM4_SR;
// TIM4 event generation register
volatile uint8_t __at(0x5345) TIM4_EGR;
// TIM4 counter
volatile uint8_t __at(0x5345) TIM4_CNTR;
// TIM4 prescaler register
volatile uint8_t __at(0x5345) TIM4_PSCR;
// TIM4 auto-reload register
volatile uint8_t __at(0x5348) TIM4_ARR;

#endif  // REGMAP_H
