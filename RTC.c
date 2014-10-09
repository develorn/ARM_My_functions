/*****************RTC****************/
//dodajemy biblioteki: pwr.c / rtc.c/ bkp.c + oznaczamy je w conf.h
//przed main
void RTC_Config(void);  //potem w main wywolanie funkcji
//RCC
RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE); //sygnaly do RTC
//nvic
NVIC_InitTypeDef NVIC_InitStructure; //na poczatku nvic
// Ustalenie modelu przerwan
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  // Wlacz przerwanie od RTC
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
//funkcja RTC
  void RTC_Config(void)
{
			
	//konfigurowanie RTC
  PWR_BackupAccessCmd(ENABLE);             //wlaczenie rejestrow domeny Backup
  RCC_LSEConfig(RCC_LSE_ON);
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);  //LSE bedzie zrodlem sygnalu zegarowego dla RTC
  RCC_RTCCLKCmd(ENABLE);                   //Wlaczenie taktowania RTC
  RTC_WaitForSynchro();                    //Oczekiwanie na synchronizacje
  RTC_WaitForLastTask();                   //Oczekiwanie az rejestry RTC zostana zapisane
  RTC_SetPrescaler(65522);                 //Okres RTC bedzie wynosil 1s  RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1)
  RTC_WaitForLastTask();
  RTC_ITConfig(RTC_IT_SEC, ENABLE);        // Wlacz przerwanie "sekundnika" RTC 
  RTC_WaitForLastTask();
  RCC_ClearFlag();
	
}
//przerwanie
  void RTC_IRQHandler(void)
{
	static unsigned long int stanRTC;
  unsigned char RTCText[5] = {"0\0"};
	LCD_WriteCommand(HD44780_CLEAR);
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
  {
    RTC_WaitForLastTask(); // Czekaj na zakonczenie ewentualnych operacji zapisu do rejestrów RTC
    RTC_ClearITPendingBit(RTC_IT_SEC); // Wyczysc flage od przerwania RTC
    
    GPIO_WriteBit(GPIOB,Led_1, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOB,Led_1)));
		
		sprintf((char *)RTCText,"%2i\0",stanRTC);
		LCD_WriteTextXY(RTCText,1,1);
		stanRTC++;
  }
}