/************PRZERWANIE ZEWNERTRZNE EXTI0**************/ //w funkcji NVIC

//RCC
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//Konfigurowanie kontrolera przerwan NVIC
  NVIC_InitTypeDef  NVIC_InitStructure;   //struktury musza byc zaraz na poczatku funkcji NVIC
  EXTI_InitTypeDef  EXTI_InitStructure; 

  //Konfiguracja NVIC - ustawienia priorytetow przerwania EXTI0
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);                //Wybor modelu grupowania przerwan

  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;               //Wybor konfigurowanego IRQ
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;      //Priorytet grupowy
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;             //Podpriorytet
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                //Wlaczenie obslugi IRQ
  NVIC_Init(&NVIC_InitStructure);

  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);    //Ustawienie zrodla przerwania

  //Konfiguracja przerwania EXTI0 na linie 0
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;                     //Wybor linii 
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;            //Ustawienie generowania przerwania (a nie zdarzenia)
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;        //Wyzwalanie zboczem opadajacym (wcisniecie przycisku)
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                      //Wlaczenie przerwania
  EXTI_Init(&EXTI_InitStructure);

  //musimy dolaczyc biblioteke periphdriv st32xxexti.c  oraz w conf.h odkreskowac biblioteke

  //w stm32xxit.c piszemy funkcje od przerwania
 void EXTI0_IRQHandler(void)
{
  //potwierdz zrodlo przerwania
  if (EXTI_GetITStatus(EXTI_Line0) != RESET){
    GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction) ((1-GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_8))));
    EXTI_ClearITPendingBit(EXTI_Line0);   //wyzeruj flage obslugi przerwania
  }
}
