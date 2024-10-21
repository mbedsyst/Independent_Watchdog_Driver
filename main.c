/*
	Title				:	IWDG
	Author			:	mbedsyst / Zalman Ul Fariz
	Description	:	Configuring the Independent Watchdog timer
								to reset the system if the Counter is not reloaded
								within a specified time.
*/

#include <stm32f401xe.h>

#define IWDG_TimeOut		5000

void IWDG_Init(void);
void IWDG_Refresh(void);

void Button_Init(void);

void UART_Init(void);
void UART_Tx(char data);
void UART_Tx_String(char *str);

void UART_Init(void)
{
  RCC->AHB1ENR |= 1<<0;
  RCC->APB1ENR |= 1<<17;

  GPIOA->MODER &= ~((1U<<4) |(1<<6));
  GPIOA->MODER |=  (1<<5) | (1<<7);
  GPIOA->AFR[0] |= (0x07<<8) | (0x07<<12);

  USART2->BRR = 0x008B;  
  USART2->CR1 |= (1<<13) | (1<<3) | (1<<2);
}

void UART_Tx(char data)
{
	
	while (!(USART2->SR & 1<<7)){}
  USART2->DR = data;
}

void UART_Tx_String(char *str)
{
	while(*str)
	{
		UART_Tx(*str++);
	}
}

void IWDG_Init(void)
{
	IWDG->KR	|=	0x5555;
	IWDG->PR	|=	0x06;
	IWDG->RLR	|=	IWDG_TimeOut * (32000/256)/1000;
	IWDG->KR	|=	0xAAAA;
	IWDG->KR	|=	0xCCCC;
}

void IWDG_Refresh(void)
{
	UART_Tx_String("\t#################### System REFRESHING ####################\n");
	IWDG->KR	|=	0xAAAA;
}

void Button_Init(void)
{
	RCC->AHB1ENR	|=	RCC_AHB1ENR_GPIOCEN;
}
 
int main(void)
{
	Button_Init();
	UART_Init();
	
	IWDG_Init();
		
  while (1) 
	{
		if (GPIOC->IDR & (1<<13)) 
		{
			IWDG_Refresh();
    }
    UART_Tx_String("\t System ONLINE\n");
	}
}




































