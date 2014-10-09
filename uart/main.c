#include "system.h"

int i = 0;
int main(void)
{
	PeripheralInit();
	USART_INIT();

	char odb[10] = "\0";
	char b[] = "abc\n";
	USART_RECEIVE_STR rec_buf_str;

    while(1)
    {

    	if(Flag_read_rec_ext == 1)
    	{
    		rec_buf_str = USART_RECEIVE();
    		Flag_read_rec_ext = 0;
    		for(i = 0; i < rec_buf_str.recieive_length; i++)
    		{
    			odb[i] = *rec_buf_str.receive_buf;
    			rec_buf_str.receive_buf++;
    		}
    	}
    	USART_SENDTXT(b);
    	GPIO_SetBits(GPIOC,GPIO_Pin_9);
    	Delay_ms(300);
    	GPIO_ResetBits(GPIOC,GPIO_Pin_9);
    	Delay_ms(300);
    }
}

