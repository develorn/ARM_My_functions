/*******************ADC 1 KANAL*********************/
//praca jednorazoa (alternatywa ciagla))
//dodanie z periphdiv xx.adc.c, conf.h odznaczenie adc
void ADC_Config(void);
//RCC
RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //wlacz taktowanie ADC1
//GPIO
GPIO_InitStructure.GPIO_Pin =analog;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //wejscie analogowe
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //funkcja
  void ADC_Config(void)
{
  //konfigurowanie przetwornika AC
  ADC_InitTypeDef ADC_InitStructure;

#include <stm32f10x.h>
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                  //Jeden przetwornik, praca niezalezna
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;                       //Pomiar jednego kanalu, skanowanie kanalow nie potrzebne
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                 //Pomiar w trybie jednokrotnym
  //ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                //Pomiar w trybie ciaglym
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//Brak wyzwalania zewnetrznego
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;            	//Wyrownanie danych do prawej - 12 mlodszych bitow znaczacych
	ADC_InitStructure.ADC_NbrOfChannel = 1; 	                          //Liczba uzywanych kanalow =1
	ADC_Init(ADC1, &ADC_InitStructure);                                 //Incjalizacja przetwornika

	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_1Cycles5);
	                                                                    //Kanal 14 - GPIOC4 - potencjometr na plytce ZL27ARM
                                                                      //Grupa podstawowa, czas probkowania 1,5+12,5=14 cykli ->  f probkowania = 14MHz/14cykli = 1MHz

	ADC_Cmd(ADC1, ENABLE);						                                  //Wlacz ADC1

	ADC_ResetCalibration(ADC1);	                                        //Reset rejestrow kalibracyjnych ADC1
	while(ADC_GetResetCalibrationStatus(ADC1));                         //Odczekanie na wykonanie resetu
	ADC_StartCalibration(ADC1);                                         //Kalibracja ADC1
	while(ADC_GetCalibrationStatus(ADC1));                              //Odczekanie na zakonczenie kalibracji ADC1

}
  //main
  if (GPIO_ReadInputDataBit(GPIOA,key1)==0)
		{
			ADC_SoftwareStartConvCmd(ADC1, ENABLE);		               //wyzwolenie pojedynczego pomiaru
      while (!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));           //odczekaj na zakonczenie konwersji
      //wartoscADC1V = ADC_GetConversionValue(ADC1) * 0.8059;   //przelicz wartosc wyrazona jako calkowita, 12-bit na rzeczywista
		  wartoscADC1  = ADC_GetConversionValue(ADC1);             //pobiez zmierzona wartosc
		  wartoscADC1V =  wartoscADC1 * 8059/10000;                //przelicz wartosc wyrazona jako calkowita, 12-bit na rzeczywista
	  	sprintf((char *)wartoscADC1VTekst, "%d,%03d V\0", wartoscADC1V / 1000, wartoscADC1V % 1000);//Przekszta³cenie wyniku na tekst, dzielenie calkowite wyzancza wartosc w V, dzielenie modulo - czesc po przecinku
      LCD_WriteTextXY(wartoscADC1VTekst,7,0);
  }
  //jesli praca ciagla to softwarestarconvcmd dajemy przed petla while(1) i usuwamy przycisk