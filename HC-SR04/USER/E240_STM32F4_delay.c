/* Includes ------------------------------------------------------------------*/
# include "E240_STM32F4_delay.h"
/* End of Includes -----------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/

static uint8_t  facUs = 0;
static uint16_t facMs = 0;
 
void delayInit(uint8_t SYSCLK) {
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
  facUs = SYSCLK / 8;
  facMs = (uint16_t)facUs * 1000;
}
 
void delayUs(uint32_t nus) {
  uint32_t temp;
  SysTick->LOAD = nus*facUs;
  SysTick->VAL = 0x00;
  SysTick->CTRL = 0x01;
  do {
    temp = SysTick->CTRL;
  } while (temp & 0x01 && !(temp &(1 << 16)));
  SysTick->CTRL = 0x00;
  SysTick->VAL = 0x00;
}

void delayMs(uint16_t nms) {
  uint32_t temp;
  SysTick->LOAD = nms*facMs;
  SysTick->VAL = 0x00;
  SysTick->CTRL = 0x01;
  do {
    temp = SysTick->CTRL;
  } while (temp & 0x01 && !(temp &(1 << 16)));
  SysTick->CTRL = 0x00;
  SysTick->VAL = 0x00;
}


/* End of Functions ----------------------------------------------------------*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
