#include "defines.h"
#include "isr_alternator_handlers.h"

struct EEPROM_t {
  uint16_t PWM_max;
};
volatile struct EEPROM_t __at(0x4000) EEPROM;
// assert не работает, поэтому проверять руками
// static_assert(sizeof(EEPROM) <= 128);

struct EEPROM_t EEPROM_RAM;

void process();

void EEPROM_load() {
  EEPROM_RAM = EEPROM;
}

void EEPROM_save() {
  EEPROM = EEPROM_RAM;
}

void init();
void PWM_set(uint16_t pwm);
uint16_t PWM_get();

void main(void) {
  disableInterrupts();

  init();

  enableInterrupts();

  for (;;) {
    PWM_set(PWM_get() + 1);
    wfi();
  }
}

void PWM_set(uint16_t pwm) {
  pwm = pwm > EEPROM_RAM.PWM_max ? EEPROM_RAM.PWM_max : pwm;

  TIM2_CCR2H = pwm >> 8;
  TIM2_CCR2L = pwm & 0xff;
}

uint16_t PWM_get() {
  uint16_t ret = TIM2_CCR2H;
  ret <<= 8;
  ret |= TIM2_CCR2L;
  return ret;
}

void clock_init() {
  // Disable clock for unnecessary peripherals
  CLK_PCKENR1.I2C = false;
  CLK_PCKENR1.TIM46 = false;
  CLK_PCKENR1.TIM3 = false;
  CLK_PCKENR1.TIM1 = false;

  // Повысим частоту от HSI
  CLK_CKDIVR.HSIDIV = CLK_CKDIVR_HSIDIV_fhsi_4;
  // И понизим для CPU
  CLK_CKDIVR.CPUDIV = CLK_CKDIVR_CPUDIV_fmaster_2;

  // Подождём переключения clock
  while (CLK_SWCR.SWBSY) nop();
}

void GPIO_init() {
  // PD4 для вывода PWM TIM2_CH2
  {
    static const uint8_t mask = 0b1 << 4;
    // PD4 as output
    GPIOD.DDR |= mask;
    // Push-pull
    GPIOD.CR1 |= mask;
    // Output speed up to 10MHz
    GPIOD.CR2 |= mask;
  }

  // Для ADC остальные порты (PC4, PD2) здесь не трогаем
}

void TIM2_init() {
  // Вывод PWM с частотой 500кГц
  // 16-bit ШИМ регулирует выходную мощность
  // Вывод -- на PD4 (TIM2_CH2)

  // Preload enable
  TIM2_CCMR2.OCPE = true;
  // Режим работы канала
  TIM2_CCMR2.OCM = TIMx_CCMR_OCM_PWM_mode1;
  // Compare output enable
  TIM2_CCER1.CC2E = true;

  // Установим частоту
  // Сейчас f_hsi = 4MHz
  // Нам надо 500кГц -- это /8
  TIM2_PSCR = 3;  // 2^3 = 8

  // Включаем сконфигурированный таймер
  TIM2_CR1.CEN = true;
}

void ADC_init() {
  // PC4=AIN2 -- Uin
  // PD2=AIN3 -- Uout
  ADC.channels[0] = 2;
  ADC.channels[1] = 3;
  ADC.vals[0] = ADC.vals[1] = 0;

  // Disable Shmidtt triggers -- high means "disable"
  // Lowers static power consumption on I/O port
  ADC_TDRL |= 0b1 << 3 || 0b1 << 2;

  // Будем использовать continious buffered режим
  // После каждого EOC -- останавливаемся, меняем канал,
  // и запускаем дальше
  // Считывание 10 измерений за раз

  // fADC = fmaster / 2  ==>  2MHz
  ADC_CR1.SPSEL = ADC_CR1_SPSEL_fmaster_2;
  // ADC_sample_rate = fADC / 14 / 10 ==>  14.2kSPS
  // И нам надо два канала -- это 7.1kSPS на канал ==> T_sample = 140us

  // Включим прерывание по окончании конвертации
  ADC_CSR.EOCIE = true;
  // Включаем непрерывную конвертацию
  ADC_CR1.CONT = true;
  // Делаем за одно прерывание 10 измерений,
  // Складывая результаты в ADC_DBxR
  ADC_CR3.DBUF = true;

  // Начнём конвертации с Uin
  ADC_CSR.CH = ADC.channels[ADC.converting_idx];

  // Выведем ADC из энергосебергающего режима
  ADC_CR1.ADON = true;

  // Ждём 7us до его пробуждения
  // У нас fCPU = 2MHz
  for (volatile uint32_t i = 0; i < (7 * 2); ++i)
    ;

  // Пинаем ADC для начала конвертаций
  ADC_CR1.ADON = true;
}

void init() {
  EEPROM_load();
  clock_init();
  GPIO_init();
  TIM2_init();
  ADC_init();
}
