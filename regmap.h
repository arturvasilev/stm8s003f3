#ifndef REGMAP_H
#define REGMAP_H

#include <stdint.h>
#include <stdbool.h>

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
volatile struct {
  uint8_t SWBSY : 1;  // 1 -- clock switch ongoing
  uint8_t SWEN : 1;   // 1 -- enable clock switch execution
} __at(0x50C5) CLK_SWCR;
// Clock divider register
volatile struct {
  uint8_t CPUDIV : 3;
  uint8_t HSIDIV : 2;
} __at(0x50C6) CLK_CKDIVR;
enum enCPUDIV {
  fmaster = 0b000,  // default
  fmaster_2,
  fmaster_4,
  fmaster_8,
  fmaster_16,
  fmaster_32,
  fmaster_64,
  fmaster_128
};
enum enHSIDIV {
  fhsi = 0b00,
  fhsi_2,
  fhsi_4,
  fhsi_8  // default
};
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

// Window watchdog
// WWDG control register
volatile uint8_t __at(0x50D1) WWDG_CR;
// WWDR window register
volatile uint8_t __at(0x50D2) WWDG_WR;

// SPI interface
// SPI control register 1
volatile uint8_t __at(0x5200) SPI_CR1;
// SPI control register 2
volatile uint8_t __at(0x5201) SPI_CR2;
// SPU interrupt control register
volatile uint8_t __at(0x5202) SPI_ICR;
// SPI status register
volatile uint8_t __at(0x5203) SPI_SR;
// SPI data register
volatile uint8_t __at(0x5204) SPI_DR;
// SPI CRC polynomial register
volatile uint8_t __at(0x5205) SPI_CRCPR;
// SPI Rx CRC register
volatile uint8_t __at(0x5206) SPI_RXCRCR;
// SPI Tx CRC register
volatile uint8_t __at(0x5207) SPI_TXCRCR;

// UART1 interface
// UART1 status register
volatile uint8_t __at(0x5230) UART1_SR;
// UART1 data register
volatile uint8_t __at(0x5231) UART1_DR;
// UART1 baud rate register 1
volatile uint8_t __at(0x5232) UART1_BRR1;
// UART1 baud rate register 2
volatile uint8_t __at(0x5233) UART1_BRR2;
// UART1 control register 1
volatile uint8_t __at(0x5234) UART1_CR1;
// UART1 control register 2
volatile uint8_t __at(0x5235) UART1_CR2;
// UART1 control register 3
volatile uint8_t __at(0x5236) UART1_CR3;
// UART1 control register 4
volatile uint8_t __at(0x5237) UART1_CR4;
// UART1 control register 5
volatile uint8_t __at(0x5238) UART1_CR6;
// UART1 guard time register
volatile uint8_t __at(0x5239) UART1_GTR;
// UART1 prescaler register
volatile uint8_t __at(0x523A) UART1_PSCR;

// Timers
typedef struct {
  uint8_t CCS : 2;
  uint8_t __reserved : 1;
  uint8_t OCPE : 1;  // Preload enable (amust with PWM)
  uint8_t OCM : 3;
} TIMx_CCMR_t;
enum enTIMx_CCMR_OCM {
  Frozen = 0b000,
  PWM_mode1 = 0b110,
  PWM_mode2 = 0b111,
};

typedef struct {
  uint8_t CC1E : 1; // Capture/compare 1 output enable
  uint8_t CC1P : 1; // Capture/compare 1 polarity
  // CC1P = 0 -- OC1 active high
  // CC1P = 1 -- OC1 active low
  uint8_t __Reserved : 2;
  uint8_t CC2E : 1;
  uint8_t CC2P : 1;
} TIMx_CCER1_t;

typedef struct {
  uint8_t CC3E : 1;
  uint8_t CC3P : 1;
} TIMx_CCER2_t;

// Timer TIM2
// TIM2 control register 1
volatile struct {
  uint8_t CEN : 1;  // 1 -- counter enabled
  uint8_t UDIS : 1;
  uint8_t URS : 1;
  uint8_t OPM : 1;  // 0 -- counter is not stopped at update (defaut)
  uint8_t __Reserved : 3;
  uint8_t ARPE : 1; // 1 -- Auto-reload buffer through preload register
} __at(0x5300) TIM2_CR1;
// TIM2 interrupt enable register
volatile uint8_t __at(0x5303) TIM2_IER;
// TIM2 status register 1
volatile uint8_t __at(0x5304) TIM2_SR1;
// TIM2 status register 2
volatile uint8_t __at(0x5305) TIM2_SR2;
// TIM2 event generation register
volatile uint8_t __at(0x5306) TIM2_EGR;
// TIM2 capture/compare mode register 1
volatile TIMx_CCMR_t __at(0x5307) TIM2_CCMR1;
// TIM2 capture/compare mode register 2
volatile TIMx_CCMR_t __at(0x5308) TIM2_CCMR2;
// TIM2 capture/compare mode register 3
volatile TIMx_CCMR_t __at(0x5309) TIM2_CCMR3;
// TIM2 capture/compare enable register 1
volatile TIMx_CCER1_t __at(0x530A) TIM2_CCER1;
// TIM2 capture/compare enable register 2
volatile TIMx_CCER2_t __at(0x530B) TIM2_CCER2;
// TIM2 counter high
volatile uint8_t __at(0x530C) TIM2_CNTRH;
// TIM2 counter low
volatile uint8_t __at(0x530D) TIM2_CNTRL;
// TIM2 prescaler register
volatile uint8_t __at(0x530E) TIM2_PSCR;
// TIM2 auto-reload register high
volatile uint8_t __at(0x530F) TIM2_ARRH;
// TIM2 auto-reload register low
volatile uint8_t __at(0x5310) TIM2_ARRL;
// TIM2 capture/compare register 1 high
volatile uint8_t __at(0x5311) TIM2_CCR1H;
// TIM2 capture/compare register 1 low
volatile uint8_t __at(0x5312) TIM2_CCR1L;
// TIM2 capture/compare register 2 high
volatile uint8_t __at(0x5313) TIM2_CCR2H;
// TIM2 capture/compare register 2 low
volatile uint8_t __at(0x5314) TIM2_CCR2L;
// TIM2 capture/compare register 3 high
volatile uint8_t __at(0x5315) TIM2_CCR3H;
// TIM2 capture/compare register 3 low
volatile uint8_t __at(0x5316) TIM2_CCR3L;

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

// ADC registers
// ADC data buffer registers
volatile uint8_t __at(0x53E0) ADC_DBxR[0x53F3 - 0x53E0];
// ADC control/status register
volatile uint8_t __at(0x5400) ADC_CSR;
// ADC configuration register 1
volatile uint8_t __at(0x5401) ADC_CR1;
// ADC configuration register 2
volatile uint8_t __at(0x5402) ADC_CR2;
// ADC configuration register 3
volatile uint8_t __at(0x5403) ADC_CR3;
// ADC data register high
volatile uint8_t __at(0x5404) ADC_DRH;
// ADC data register low
volatile uint8_t __at(0x5405) ADC_DRL;
// ADC Schmitt trigger disable register high
volatile uint8_t __at(0x5406) ADC_TDRH;
// ADC Schmitt trigger disable register low
volatile uint8_t __at(0x5407) ADC_TDRL;
// ADC high threshold register high
volatile uint8_t __at(0x5408) ADC_HTRH;
// ADC high threshold register low
volatile uint8_t __at(0x5409) ADC_HTRL;
// ADC low threshold register high
volatile uint8_t __at(0x540A) ADC_LTRH;
// ADC low threshold register low
volatile uint8_t __at(0x540B) ADC_LTRL;
// ADC analog watchdog status register high
volatile uint8_t __at(0x540C) ADC_AWSRH;
// ADC analog watchdog status register low
volatile uint8_t __at(0x540D) ADC_AWSRL;
// ADC analog watchdog control register high
volatile uint8_t __at(0x540E) ADC_AWCRH;
// ADC analog watchdog control register low
volatile uint8_t __at(0x540F) ADC_AWCRL;

#endif  // REGMAP_H
