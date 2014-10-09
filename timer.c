/*************TIMER******************/


void TIM_Conf(void);
//w RCC_Conf
RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

//w NVID
NVIC_InitTypeDef NVIC_InitStructure;
	//przerwanie UP (przepelnienie) timera1
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
	
	//przerwanie CC (porownanie) timera1
  	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);

//funckaj
	void TIM_Config(void) {
  //Konfiguracja timerow
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;

  //Konfiguracja licznika 1
	//Ustawienia taktowania i trybu pracy licznika 1
	TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;                //taktowanie licznka fclk = 72MHz/7200 = 10kHz
	TIM_TimeBaseStructure.TIM_Period = 10000;                    //okres przepelnien licznika = 20000 taktow = 2 sekundy
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;      //dzielnik zegara dla ukladu generacji dead-time i filtra
 	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;               //licznik powtorzen
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //tryb pracy licznika

	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);              //Inicjalizacja licznika

	//Konfiguracja kanalu 1 - ctc
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;          //tryb pracy kanalu
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//wlaczenie generowania sygnalu na wyjsciu licznika
	TIM_OCInitStructure.TIM_Pulse =5000;                         //5000 taktow = 0.5s od restartu licznika
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;    //polaryzacja wyjscia
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);                     //Inicjalizacja kanalu 1 licznika TIM1

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Disable);           //zapis ustawien, wylaczenie buforowania  - ewentualne zmiany konfiguracji uwzgledniane sa od razu  
                                                               //gdy preload jest wlaczony - zmiany wchodza w zycie dopiero przy nastepnym update



	// Wlaczenie przerwan od licznikow
  TIM_ITConfig(TIM1, TIM_IT_Update , ENABLE);  //wlaczenie przerwania od przepelnienia
  TIM_ITConfig(TIM1, TIM_IT_CC1 , ENABLE);     //wlaczenie przerwanie od porownania w kanale 1

  
	// Wlaczenie timerow
  TIM_Cmd(TIM1, ENABLE);

}
//wywolanie w main
	TIM_Config();
//w it.c
	void TIM1_CC_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)	{                  //CC1 - najmniejsza czestotliwosc
  	TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);							//zerowanie flagi
  	GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_8)));  	//LED1
  	TIM_SetCompare1(TIM1, TIM_GetCapture1(TIM1) + CC1);              //modyfikacja wartosci odniesienia w kanale CC1
	}	
  
}


/******************PWM****************************/
	//w RCC
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	//GPIO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   // ustawienie funkcji alternatywnej -> tam podlaczamy led
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //funckja
  void TIM_Conf(void)
{
	  //konfigurowanie licznikow
  
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;

  // Konfiguracja TIM4
  // Ustawienia ukladu podstawy czasu
  TIM_TimeBaseStructure.TIM_Prescaler = 0;         //bez prescelera (prescaler=1)
  TIM_TimeBaseStructure.TIM_Period = 49999ul;      //czestotliwosc PWM = 1440 Hz
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  
  // Konfiguracja kanalu 3 - uzywamy kanalu 3 poniewaz jego wyjscie jest na GPIOB8 - gdzie jest LED1
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 100;          //wypelnienie = 50000/5000=10%
  TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);

  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);//wlaczenie buforowania
  
 

  TIM_ARRPreloadConfig(TIM3, ENABLE);//wlaczenie buforowania 
  // Wlaczenie timera
  TIM_Cmd(TIM3, ENABLE);  

}
//main

if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==0)
			{
				if((TIM3->CCR1)>=45000)   //bezposrednie odwolanie do rejestru
				{
					TIM3->CCR1=100;
				}
				TIM3->CCR1 = TIM3->CCR1+250;
				GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_SET);
				for (i=0;i<400000;i++);	

}



/***************	ELIMINCJA DRGAN STYKOW !!!!!!!!!!!!!!!!!!! *************************/
/**************************************************************************************/
//RCC
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
//NVIC
NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef  EXTI_InitStructure; 

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
   
  //przerwanie ogolne od timera2
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);


  //przerwanie na EXTI_Line0 - czyli przycisk SW0 na lini PA0
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0); //ustawienie zrodla przerwania

  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //zbocze opadajace - wcisniecie przycisku
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);  
  //timer
  void TIM_Config(void) {
//Konfigurowanie licznikow TIM

  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  
  // Ustawienia ukladu podstawy czasu
	TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;  //10kHz
	TIM_TimeBaseStructure.TIM_Period = 700ul;      //70ms
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  //Tylko konfiguracja licznika - bez wlaczenia przerwania i samego licznika - to jest robione dopiero po wcisnieciu przycisk
}
//przerwanie EXT0 - gdy wcisniety klawisz
     //przed funkcjami
  extern bool wcisniecie;
  //dalej
  void EXTI0_IRQHandler(void)
{
	 if (EXTI_GetITStatus(EXTI_Line0) != RESET){
    TIM_SetCounter(TIM2, 0);            //zerowanie licznika
    TIM_Cmd(TIM2, ENABLE);              //wlaczenie licznika
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//wlaczenie przerwania
    EXTI_ClearITPendingBit(EXTI_Line0);  //kasowanie flagi przerwania
  }
}

  void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)	{ //przepelnienie
  	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);         //kasowanie flagi przerwania
    TIM_Cmd(TIM2, DISABLE);                             //wylaczenie licznika
    TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);         //wylaczenie przerwania

    if (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)){     //jesli po odliczeniu czasu przycisk nadal wcisniety -> wystapilo wcisniecie
      wcisniecie = TRUE;
    } else {
      wcisniecie = FALSE;
    }
  }
}


  //main
  volatile bool wcisniecie;
int main(void)
//konfiguracja systemu
{
	volatile unsigned long int lWcisniec;
  unsigned char lWcisniecTekst[6] = {"0\0"};

  RCC_Config();
  GPIO_Config();
  NVIC_Config();
TIM_Config();
	//GPIO_ResetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
	
	LCD_Initialize();                   //inicjalizacja wysietlacza
  LCD_WriteCommand(HD44780_CLEAR);    //wyczysc wyswietlacz
  LCD_WriteText("Odliczanie:\0");     //wyswietl tekst pierwszej linii
	lWcisniec=0;
  while (1) 
  {		
			  if (wcisniecie == TRUE) {
      lWcisniec++;
      sprintf((char *)lWcisniecTekst,"%5i\0",lWcisniec);
      LCD_WriteTextXY(lWcisniecTekst,10,1);
      wcisniecie=FALSE;
    }
	
  };
  return 0;
}

