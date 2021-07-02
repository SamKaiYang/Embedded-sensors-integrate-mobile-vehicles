/* Includes ------------------------------------------------------------------*/
# include "user_usart.h"
/* End of Includes -----------------------------------------------------------*/

/* Override function ---------------------------------------------------------*/
int fputc(int ch, FILE * f) {
  /* Override stdio function "printf" */
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

/* Functions -----------------------------------------------------------------*/
void USART2_Configuration(void) {
  /* Declare Structure of USART and GPIO  */
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable RCC Clock of GPIOA and USART2  */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

  /* Configure USART2 Tx (PA.02) as alternate function push-pull */
  /* Configure USART2 Rx (PA.03) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Set PA2 and PA3 for USART2*/
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

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

/* End of Functions ----------------------------------------------------------*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
