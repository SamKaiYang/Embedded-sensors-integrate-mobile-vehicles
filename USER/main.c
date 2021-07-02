# include "stm32f4xx.h"

# include "string.h"
# include "stm32f446_Serial.h"

int main(){
	Serial2_Init(115200);
	Serial3_Init(115200);
	while(1);
}
 
void USART3_IRQHandler(){
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET){
		printf("%c", (char)USART_ReceiveData(USART3));
	}
}

void USART2_IRQHandler(){
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET){
		char c = USART_ReceiveData(USART2);
		switch(c) {
			case '0':
				Serial3_write("AT+NAME?\r\n");
			break;
			case '1':
				Serial3_write("AT+UART?\r\n");
			break;
			case '2':
				Serial3_write("AT+NAME=HC-05-1\r\n");
			break;
			case '3':
				Serial3_write("AT+UART=115200,0,0\r\n");
			break;
		}
	}
}
