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
typedef struct {
  uint8_t PAIS : 2;
  uint8_t PBIS : 2;
  uint8_t PCIS : 2;
  uint8_t PDIS : 2;
} EXTI_CR1_t;
enum enEXTI_CR {
  EXTI_CR_Falling_LowLevel = 0b00,
  EXTI_CR_Rising = 0b01,
  EXTI_CR_Falling = 0b10,
  EXTI_CR_RisingFalling = 0b11
};
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
  CLK_CKDIVR_CPUDIV_fmaster = 0b000,  // default
  CLK_CKDIVR_CPUDIV_fmaster_2,
  CLK_CKDIVR_CPUDIV_fmaster_4,
  CLK_CKDIVR_CPUDIV_fmaster_8,
  CLK_CKDIVR_CPUDIV_fmaster_16,
  CLK_CKDIVR_CPUDIV_fmaster_32,
  CLK_CKDIVR_CPUDIV_fmaster_64,
  CLK_CKDIVR_CPUDIV_fmaster_128
};
enum enHSIDIV {
  CLK_CKDIVR_HSIDIV_fhsi = 0b00,
  CLK_CKDIVR_HSIDIV_fhsi_2,
  CLK_CKDIVR_HSIDIV_fhsi_4,
  CLK_CKDIVR_HSIDIV_fhsi_8  // default
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
  uint8_t : 2;
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
  uint8_t : 1;
  uint8_t OCPE : 1;  // Preload enable (amust with PWM)
  uint8_t OCM : 3;
} TIMx_CCMR_t;
enum enOCM {
  TIMx_CCMR_OCM_Frozen = 0b000,
  TIMx_CCMR_OCM_PWM_mode1 = 0b110,
  TIMx_CCMR_OCM_PWM_mode2 = 0b111,
};

typedef struct {
  uint8_t CC1E : 1; // Capture/compare 1 output enable
  uint8_t CC1P : 1; // Capture/compare 1 polarity
  // CC1P = 0 -- OC1 active high
  // CC1P = 1 -- OC1 active low
  uint8_t : 2;
  uint8_t CC2E : 1;
  uint8_t CC2P : 1;
} TIMx_CCER1_t;

typedef struct {
  uint8_t CC3E : 1;
  uint8_t CC3P : 1;
} TIMx_CCER2_t;

// Timer TIM2
// TIM1 control register 1
volatile uint8_t __at(0x5250) TIM1_CR1;
// TIM1 control register 2
volatile uint8_t __at(0x5251) TIM1_CR2;
// Slave mode control register
volatile uint8_t __at(0x5252) TIM1_SMCR;
// External trigger register
volatile uint8_t __at(0x5253) TIM1_ETR;
// Interrupt enable register
volatile uint8_t __at(0x5254) TIM1_IER;
// Status register 1
volatile uint8_t __at(0x5255) TIM1_SR1;
// Status register 2
volatile uint8_t __at(0x5256) TIM1_SR2;
// Event reneration register
volatile uint8_t __at(0x5257) TIM1_EGR;
// Capture/compare mode register 1
volatile uint8_t __at(0x5258) TIM1_CCMR1;
// Capture/compare mode register 2
volatile uint8_t __at(0x5259) TIM1_CCMR2;
// Capture/compare mode register 3
volatile uint8_t __at(0x525A) TIM1_CCMR3;
// Capture/compare mode register 4
volatile uint8_t __at(0x525B) TIM1_CCMR4;
// Capture/compare enable register 1
volatile uint8_t __at(0x525C) TIM1_CERR1;
// Capture/compare enable register 2
volatile uint8_t __at(0x525D) TIM1_CERR2;
// Counter high
volatile uint8_t __at(0x525E) TIM1_CNTRH;
// Counter low
volatile uint8_t __at(0x525F) TIM1_CNTRL;
// Prescaler register high
volatile uint8_t __at(0x5260) TIM1_PSCRH;
// Prescaler register low
volatile uint8_t __at(0x5261) TIM1_PSCRL;
// Auto-reload register high
volatile uint8_t __at(0x5262) TIM1_ARRH;
// Auto-reload register low
volatile uint8_t __at(0x5263) TIM1_ARRL;
// Repetition counter register
volatile uint8_t __at(0x5264) TIM1_RCR;
// Capture/compare register 1 high
volatile uint8_t __at(0x5265) TIM1_CCR1H;
// Capture/compare register 1 low
volatile uint8_t __at(0x5266) TIM1_CCR1L;
// Capture/compare register 2 high
volatile uint8_t __at(0x5267) TIM1_CCR2H;
// Capture/compare register 2 low
volatile uint8_t __at(0x5268) TIM1_CCR2L;
// Capture/compare register 3 high
volatile uint8_t __at(0x5269) TIM1_CCR3H;
// Capture/compare register 3 low
volatile uint8_t __at(0x526A) TIM1_CCR3L;
// Capture/compare register 4 high
volatile uint8_t __at(0x526B) TIM1_CCR4H;
// Capture/compare register 4 low
volatile uint8_t __at(0x526C) TIM1_CCR4L;
// Breake register
volatile uint8_t __at(0x526D) TIM1_BKR;
// Dead-time register
volatile uint8_t __at(0x526E) TIM1_DTR;
// Output idle state register
volatile uint8_t __at(0x526F) TIM1_OISR;

// Timer TIM2
// TIM2 control register 1
volatile struct {
  uint8_t CEN : 1;  // 1 -- counter enabled
  uint8_t UDIS : 1;
  uint8_t URS : 1;
  uint8_t OPM : 1;  // 0 -- counter is not stopped at update (defaut)
  uint8_t : 3;
  uint8_t ARPE : 1; // 1 -- Auto-reload buffer through preload register
} TIMx_CR1_t __at(0x5300) TIM2_CR1;
// TIM2 interrupt enable register
volatile uint8_t __at(0x5303) TIM2_IER;
// TIM2 status register 1
volatile uint8_t __at(0x5304) TIM2_SR1;
// TIM2 status register 2
volatile uint8_t __at(0x5305) TIM2_SR2;
// TIM2 event generation register
volatile struct {
  uint8_t UG : 1;  // Update generation
  uint8_t CC1G : 1; // Capture/compare 1 generation
  uint8_t CC2G : 1; // Capture/compare 2 generation
  uint8_t CC3G : 1; // Capture/compare 3 generation
} __at(0x5306) TIM2_EGR;
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
volatile struct {
  uint8_t PSC : 4;  // Power of 2
} __at(0x530E) TIM2_PSCR;
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
volatile TIMx_CR1_t __at(0x5340) TIM4_CR1;
// TIM4 interrupt enable register
volatile struct {
  uint8_t UIE : 1;  // Update interrupt enable
} __at(0x5343) TIM4_IER;
// TIM4 status register
volatile struct {
  uint8_t UIF : 1;  // Update interrupt flag
  uint8_t : 5;
  uint8_t TIF : 1;  // Trigger interrupt flag
} __at(0x5344) TIM4_SR;
// TIM4 event generation register
volatile uint8_t __at(0x5345) TIM4_EGR;
// TIM4 counter
volatile uint8_t __at(0x5345) TIM4_CNTR;
// TIM4 prescaler register
volatile struct {
  uint8_t PSC : 3;  // Prescaler -- power of 2
} __at(0x5345) TIM4_PSCR;
// TIM4 auto-reload register
volatile uint8_t __at(0x5348) TIM4_ARR;

// ADC registers
// ADC data buffer registers
volatile struct {
  uint8_t high;
  uint8_t low;
} __at(0x53E0) ADC_DBxR[(0x53F4 - 0x53E0) / 2];
// ADC control/status register
volatile struct {
  uint8_t CH : 4; // Number of AINx -channel
  uint8_t AWDIE : 1;  // Analog watchdog interrupt enable
  uint8_t EOCIE : 1;  // Interrupt enable for EOC
  uint8_t AWD : 1;  // Analog watchdog flag
  uint8_t EOC : 1;  // End of conversion flag
} __at(0x5400) ADC_CSR;
// ADC configuration register 1
volatile struct {
  uint8_t ADON : 1; // ADC on/off
  uint8_t CONT : 1; // Continious conversion
  uint8_t : 2;
  uint8_t SPSEL : 3;  // Prescaler selection
} __at(0x5401) ADC_CR1;
enum enSPSEL {
  ADC_CR1_SPSEL_fmaster_2 = 0b000,
  ADC_CR1_SPSEL_fmaster_3,
  ADC_CR1_SPSEL_fmaster_4,
  ADC_CR1_SPSEL_fmaster_6,
  ADC_CR1_SPSEL_fmaster_8,
  ADC_CR1_SPSEL_fmaster_10,
  ADC_CR1_SPSEL_fmaster_12,
  ADC_CR1_SPSEL_fmaster_18
};
// ADC configuration register 2
volatile struct {
  uint8_t : 1;
  uint8_t SCAN : 1; // Scan mode enabled
  uint8_t : 1;
  // 0 -- left alignment (ADC_DRH read first, then ADC_DRL)
  // 1 -- right alignment (ADC_DRL read first, then ADC_DRH)
  uint8_t ALIGN : 1; 
  uint8_t EXTSEL : 2; // External event selection
  uint8_t EXTTRIG : 1;  // External trigger enable
} __at(0x5402) ADC_CR2;
// ADC configuration register 3
volatile struct {
  uint8_t : 6;
  uint8_t OVR : 1;  // Overrung flag
  uint8_t DBUF : 1; // Data buffer enable
} __at(0x5403) ADC_CR3;
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
