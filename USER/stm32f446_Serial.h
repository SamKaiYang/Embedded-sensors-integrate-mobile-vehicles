# include "stm32f4xx.h"
# include "stdio.h"

void Serial2_Init(uint32_t baudrate);
void Serial3_Init(uint32_t baudrate);

void Serial3_write(char *string);
