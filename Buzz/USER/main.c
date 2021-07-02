/* Includes ------------------------------------------------------------------*/
# include "stm32f4xx.h"
# include "stdio.h"
/* End of Includes -----------------------------------------------------------*/

/* Declare Function ----------------------------------------------------------*/
void USART2_Configuration(void);
void GPIO_Configuration(void);
int getTone(char symbol);
void Tone(int freq, int duration);
void noTone(void);
/* End of Declare Functions --------------------------------------------------*/

/* Override function ---------------------------------------------------------*/
int fputc(int ch, FILE * f) {
  USART_SendData(USART2, (unsigned char)ch);
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
  return ch;
}

int fgetc(FILE * file) {
/* Override stdio function "scanf" */
  while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);
  return (USART_ReceiveData(USART2));
}
/* End of Override function --------------------------------------------------*/

/* System Delay --------------------------------------------------------------*/
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
}
/* End of System Delay -------------------------------------------------------*/

const int toneTable[7][5] = {
  { 66, 131, 262, 523, 1046},  // C Do
  { 74, 147, 294, 587, 1175},  // D Re
  { 83, 165, 330, 659, 1318},  // E Mi
  { 88, 175, 349, 698, 1397},  // F Fa
  { 98, 196, 392, 784, 1568},  // G So
  {110, 220, 440, 880, 1760},  // A La
  {124, 247, 494, 988, 1976}   // B Si
};

char toneName[] = "CDEFGAB";
char beeTone[] = "GEEFDDCDEFGGGGEEFDDCEGGEDDDDDEFEEEEEFGGEEFDDCEGGC";
int beeBeat[] = {
  1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2,
  1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 4,
  1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2,
  1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 4
};

/* main ----------------------------------------------------------------------*/
int main(void) {
  USART2_Configuration();
  GPIO_Configuration();

  SysTick_Config(SystemCoreClock / 1000); // SysTick 1 msec interrupts
  int ii, sing_length, toneNo;
  int duration;
  sing_length = sizeof(beeTone) - 1;
  for ( ii = 0; ii < sing_length; ii++ ) {
    toneNo = getTone(beeTone[ii]);
    duration = beeBeat[ii] * 333;
    Tone(toneTable[toneNo][3], beeBeat[ii] * 333);
    noTone();
  }
  while (1) ;
}
/* End of main ---------------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/

int getTone(char symbol) {
  int toneNo = 0;
  for ( int ii = 0; ii < 7; ii++ ) {
    if ( toneName[ii] == symbol ) {
      toneNo = ii;
      break;
    }
  }
  return toneNo;
}

void GPIO_Configuration(void) {
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_TIM2);
}

void Tone(int freq, int duration) {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_TIM2);
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
	
  TIM_TimeBaseStructure.TIM_Period = (10000/freq) - 1;
  TIM_TimeBaseStructure.TIM_Prescaler = 10000;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	
  TIM_OCInitStructure.TIM_Pulse = TIM_TimeBaseStructure.TIM_Period * 0.5;
  TIM_OC1Init(TIM2, &TIM_OCInitStructure);

  TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
  TIM_ARRPreloadConfig(TIM2, ENABLE);
  TIM_Cmd(TIM2, ENABLE);
	
	Delay(duration);
}

void noTone(void) {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_TIM2);
}

void USART2_Configuration(void) {
  /* Declare Structure of USART and GPIO  */
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable RCC Clock of GPIOA and USART2  */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

  /* Set PA2 and PA3 for USART*/
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

  /* Configure USART2 Tx (PA.02) as alternate function push-pull */
  /* Configure USART2 Rx (PA.03) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Set USART2 Paramter Configuration */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART2, &USART_InitStructure);

  /* Enable USARTy Receive and Transmit interrupts */
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

  /* Enable USART2 */
  USART_Cmd(USART2, ENABLE);
}
/* End of Functions ---------------------------------------------------------------*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
