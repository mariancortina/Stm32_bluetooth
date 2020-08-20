#include "stm32f4xx.h"                  
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char  str[50];
uint32_t i=0,a=0;
char VariableTemporal[21] = {0};

//FUNCION PARA TOMAR UN CARACTER DEL USART 
void USART_Puts(USART_TypeDef* USARTx,volatile char *s) 
{
 while(*s)
 {
  while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	 USART_SendData(USARTx,*s);
	 *s++;
 }
}


//FUNCION PARA INICIALIZAR EL BLUETOOTH Y LOS PINES DE LOS LED DE LA DISCOVERY
void inicializarBluetooth(){
	GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE); 
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); 


		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12 | GPIO_Pin_13 |GPIO_Pin_14 | GPIO_Pin_15 ;
		GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
		GPIO_Init(GPIOD, &GPIO_InitStructure); 

		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF; 
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2 | GPIO_Pin_3;
		GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;

		GPIO_Init(GPIOA,&GPIO_InitStructure);

		GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

		USART_InitStructure.USART_BaudRate=115200; // VELOCIDAD DE BAUDRATE DEL BLUETOOTH
		USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;;
		USART_InitStructure.USART_Parity=USART_Parity_No;
		USART_InitStructure.USART_StopBits=USART_StopBits_1;
		USART_InitStructure.USART_WordLength=USART_WordLength_8b;
		USART_Init(USART2,&USART_InitStructure);

		USART_Cmd(USART2,ENABLE);
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

		NVIC_InitTypeDef NVIC_InitStructure;

	  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	  NVIC_Init(&NVIC_InitStructure);
}

//INTERRUPCION PARA RECIBIR UN CARACTER DEL BLUETOOTH 
void USART2_IRQHandler(void)
	{

	 while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);
  	char tmp = USART_ReceiveData(USART2); // Data registeri dolu ise okunan veriyi gelenVeri'ye kaydet
    USART_SendData(USART2, tmp);
		VariableTemporal[i]=tmp;
		i++;
//SI RECIBO UN 1 POR MEDIO DEL BLUETOOTH ENTONCES PRENDO EL LED VERDE DE LA DISCOVERY Y APAGO LOS DEMAS
		  	if(tmp=='1')
			{

			    GPIO_SetBits(GPIOD,GPIO_Pin_12);
					GPIO_ResetBits(GPIOD,GPIO_Pin_13);
					GPIO_ResetBits(GPIOD,GPIO_Pin_14);
					GPIO_ResetBits(GPIOD,GPIO_Pin_15);
					 sprintf(str, "\rLED VERDE \n");
					USART_Puts(USART2,str);
//SI RECIBO UN 2 POR MEDIO DEL BLUETOOTH ENTONCES PRENDO EL SIGUIENTE LED DE LA DISCOVERY Y APAGO LOS DEMAS
			}
       if(tmp=='2')
			{
			    GPIO_ResetBits(GPIOD,GPIO_Pin_12);
					GPIO_SetBits(GPIOD,GPIO_Pin_13);
					GPIO_ResetBits(GPIOD,GPIO_Pin_14);
					GPIO_ResetBits(GPIOD,GPIO_Pin_15);
//SI RECIBO UN 3 POR MEDIO DEL BLUETOOTH ENTONCES PRENDO EL SIGUIENTE LED DE LA DISCOVERY Y APAGO LOS DEMAS
			}
       if(tmp=='3')
			{
			    GPIO_ResetBits(GPIOD,GPIO_Pin_12);
					GPIO_ResetBits(GPIOD,GPIO_Pin_13);
					GPIO_SetBits(GPIOD,GPIO_Pin_14);
					GPIO_ResetBits(GPIOD,GPIO_Pin_15);
//SI RECIBO UN 4 POR MEDIO DEL BLUETOOTH ENTONCES PRENDO EL SIGUIENTE LED DE LA DISCOVERY Y APAGO LOS DEMAS
			}
       if(tmp=='4')
			{
			    GPIO_ResetBits(GPIOD,GPIO_Pin_12);
					GPIO_ResetBits(GPIOD,GPIO_Pin_13);
					GPIO_ResetBits(GPIOD,GPIO_Pin_14);
					GPIO_SetBits(GPIOD,GPIO_Pin_15);
			}}
			
int main()
{

	while(1)
	{
		inicializarBluetooth();
		 USART2_IRQHandler();
	}
}
