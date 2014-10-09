#include "LIS35_SPI.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_gpio.h"
void Config_LIS(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	//miso mosi sck
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //alternat f.
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//CS
	GPIO_InitStructure.GPIO_Pin = CS;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//SPI
	SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex; //tryb pracy
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_High; //stan sck przy braku transmisji
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_2Edge; //aktywne zbocze sygnalu sck
	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft; //sposob sterowania nss
	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_128;
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial=7;
	SPI_Init(SPI1,&SPI_InitStructure);
	
	SPI_Cmd(SPI1,ENABLE);	
}

char SPI_Transmit(char Data)
{
	SPI_I2S_SendData(SPI1,Data);
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET);
	return (char) SPI_I2S_ReceiveData(SPI1);
}

void SPI_CS_Enable(void)
{
	GPIO_ResetBits(GPIOA,CS);
}
void SPI_CS_Disable(void)
{
	GPIO_SetBits(GPIOA,CS);
}
void SPI_Read(char adr, char*odb)
{
	SPI_CS_Enable();
	SPI_Transmit(0x80|adr);
	*odb=SPI_Transmit(0xFF);
	SPI_CS_Disable();	
}
void SPI_Write(char adr, char msg)
{
	SPI_CS_Enable();
	SPI_Transmit(0x00|adr);
	SPI_Transmit(msg);
	SPI_CS_Disable();
}
char Init_LIS35(void)
{
	char Reg;
	SPI_Write(CTRL_REG2,0x40); //boot
	SPI_Write(CTRL_REG1,0x47); ///aktywacja wszystkich osi
	SPI_Read(CTRL_REG1,&Reg);
	if(Reg==0x47) return 1;
	else return 0;

}






