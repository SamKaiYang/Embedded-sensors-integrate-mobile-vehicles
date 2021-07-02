# include "stm32f4xx.h"
# include "user_usart.h"
# include "user_delay.h"
# include "stdio.h"
# include "stdlib.h"
/*
volatile uint32_t msTicks; // counts 1ms timeTicks

void SysTick_Handler(void) {
  msTicks++;
}

void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks) {
    __NOP();
  }
}*/

void GPIO_Configuration();
void TIM2_Configuration(u16 arr, u16 psc);
float get_Diatance();
/*
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
*/
int main(void) {
  USART2_Configuration();
	GPIO_Configuration();
	delayInit(16);
	printf("FINISH!\n");
   while (1)
  {
		printf("%f\n", get_Diatance());
  }
}

void GPIO_Configuration() {
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
  GPIO_InitTypeDef GPIO_InitStructure;
	
  // HC-SR04 echo
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  // HC-SR04 trig
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

float get_Diatance() {
	long time, timeout;
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
	delayUs(5);
	GPIO_SetBits(GPIOB, GPIO_Pin_5);
	delayUs(10);
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
	
	timeout = 1000000;
	while (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4)) {
		if (timeout-- == 0) {
			return -1;
		}
	}
	
	time = 0;
	while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4)) {
		time++;
	}
	
	return (float)time / 2 / 29.1;
}
