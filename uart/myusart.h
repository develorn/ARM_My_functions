#ifndef __MYUSART_H
#define __MYUSART_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "itoa.h"
#include "misc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"



/***************************** TYPEDEFS********************************/
typedef struct{
	char*receive_buf;
	size_t recieive_length;
}USART_RECEIVE_STR;

/******************** GLOBAL VARIABLES ************************/
extern uint8_t Flag_read_rec_ext;

/******************** GLOBAL FUNCTIONS ************************/
void USART_SENDTXT(const char*buf);
void USART_INIT(void);
USART_RECEIVE_BUF USART_RECEIVE(void);

/**************************************************************/



#ifdef __cplusplus
}
#endif

#endif /* __DELAY_H__ */
