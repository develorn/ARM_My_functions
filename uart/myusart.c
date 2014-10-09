#include "myusart.h"

/************** GLOBAL VARIABLES *************************/
uint8_t Flag_read_rec_ext = 0;
USART_RECEIVE_STR usart_rec_buf_str;
/************** LOCAL VARIABLES **************************/
size_t length_tran;
size_t length_recbuff = 0; //ilosc odebranych znakow
uint8_t *transmit;
uint8_t Flag_ready_send = 0;
uint8_t Flag_f_send = 1;
uint8_t Flag_read_send_ext = 0;
char rec_buf[10];
char *wsk_buf_rec;

/************** GLOBAL FUNCTION **************************/
void USART_INIT(void)
{
	//konfigurowanie ukladu USART
	USART_InitTypeDef  USART_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;

	//PA9 - Tx
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//PA10 - Rx
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);

 	USART_InitStructure.USART_BaudRate = 9600;                                      //Predkosc transmisji =9600bps
 	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     //Dlugosc slowa = 8 bitow
 	USART_InitStructure.USART_StopBits = USART_StopBits_1;                          //1 bit stopu
  	USART_InitStructure.USART_Parity = USART_Parity_No;                             //Brak kontroli parzystosci
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Kontrola przeplywu danych (brak)
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	                //Tryb pracy
  	USART_Init(USART1, &USART_InitStructure);

	USART_Cmd(USART1, ENABLE);

	//NVIC
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	//Przerwanie od USART1
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	wsk_buf_rec = &rec_buf[0]; //nie dalo sie globalnie
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // wlaczenie receive
}

void USART_SENDTXT(const char* buf)
{
	if(Flag_f_send == 1)
	{
		length_tran = strlen(buf);
		transmit=(uint8_t*)buf;
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		Flag_f_send = 0;
		Flag_ready_send = 0;
	}
	else if(Flag_ready_send == 1)
	{

		Flag_f_send = 1;
		Flag_read_send_ext = 1;
	}
}

USART_RECEIVE_BUF USART_RECEIVE(void)
{
	usart_rec_buf_str.receive_buf = &rec_buf[0];
	return usart_rec_buf_str;
}

void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //receive
		{
			*wsk_buf_rec=USART_ReceiveData(USART1);

			if(length_recbuff == 10 || *wsk_buf_rec == '%')
				{
					//length_recbuff = length_rec;
					usart_rec_buf_str.recieive_length = length_recbuff;
					length_recbuff = 0;
					wsk_buf_rec = &rec_buf[0];
					Flag_read_rec_ext = 1;
				}
			else
			{
				Flag_read_rec_ext = 0;
				wsk_buf_rec++;
				length_recbuff++;
			}

		}

	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)  //send
		{
			if(length_tran!=0 || Flag_ready_send == 0)
				{
					USART_SendData(USART1,*transmit++);
					length_tran--;
				}
			if (length_tran == 0)
				{
					USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
					Flag_ready_send = 1;
				}
		}
}


