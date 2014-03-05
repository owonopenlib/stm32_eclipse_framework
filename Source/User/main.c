#include "stm32f10x.h"

void UsartInit(void);
void UsartPrintChar(char c);
void UsartPrintString(char *s);
void DelayInit(void);
void Delay_mS(uint16_t n);

int main(void)
{
	DelayInit();
	UsartInit();
	while (1) {
		UsartPrintString("Hello eclipse.\n");
		Delay_mS(500);
	}
}

void UsartInit(void)
{
	USART_InitTypeDef usart;
	GPIO_InitTypeDef gpio;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 | //
		RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Pin = GPIO_Pin_9;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);

	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &gpio);

	usart.USART_BaudRate = 115200;
	usart.USART_WordLength = USART_WordLength_8b;
	usart.USART_StopBits = USART_StopBits_1;
	usart.USART_Parity = USART_Parity_No;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart.USART_Mode = USART_Mode_Tx;
	USART_Init(USART1, &usart);

	USART_Cmd(USART1, ENABLE);
}

void UsartPrintChar(char c)
{
	USART_SendData(USART1, c);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {
		;
	}
}

void UsartPrintString(char *s)
{
	while (*s) {
		UsartPrintChar(*s++);
	}
}

void DelayInit(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}

void Delay_mS(uint16_t n)
{
	uint32_t tmp;

	SysTick->LOAD = (uint32_t) n * (SystemCoreClock / 8000);
	SysTick->VAL = 0x00;
	SysTick->CTRL = 0x01;
	do {
		tmp = SysTick->CTRL;
	} while ((tmp & 0x01) && !(tmp & (1 << 16)));
	SysTick->CTRL = 0x00;
	SysTick->VAL = 0X00;
}
