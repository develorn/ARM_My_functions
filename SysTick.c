/********************SYSTICK************/
//przed main
unsigned long int SysTick_Config_Mod(unsigned long int SysTick_CLKSource, unsigned long int Ticks);

//funkcja SysTick
unsigned long int  SysTick_Config_Mod(unsigned long int SysTick_CLKSource, unsigned long int Ticks) 
{ 
  //inicjalizacja licznika SysTick
  //zastepuje funkcje z bibliotek STM w zwiazku z bledem w funcji SysTick_Config
  unsigned long int Settings;

  assert_param(IS_SYSTICK_CLK_SOURCE(SysTick_CLKSource));

  if (Ticks > SysTick_LOAD_RELOAD_Msk)  return (1);             //Kontrola, czy wartosc poczatkowa nie przekracza max
                                                                
  SysTick->LOAD = (Ticks & SysTick_LOAD_RELOAD_Msk) - 1;        //Ustaw wartosc poczatkowa licznika 
  NVIC_SetPriority (SysTick_IRQn, 0);                           //Ustaw priorytet przerwania
  SysTick->VAL  = 0;                                            //Ustaw wartosc aktualna licznika
  Settings=SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;  //Ustaw flagi wlaczenia SysTick IRQ  i samego licznika
  if (SysTick_CLKSource == SysTick_CLKSource_HCLK){             //Wybierz flage ustawien zrodla sygnalu zegarowego
    Settings |= SysTick_CLKSource_HCLK;
  } else {
    Settings &= SysTick_CLKSource_HCLK_Div8;
  }
  SysTick->CTRL = Settings;                                     //Zapisz ustawienia do rejestru sterujacego SysTick (i wlacz licznik)
  return (0);                                        
}

//wywolanie np w main
if (SysTick_Config_Mod(SysTick_CLKSource_HCLK_Div8, 9000000ul)) //72Mhz / 8  i zlicza do 900xx...daje 1s
	{
		while(1);		// W razie bledu petla nieskonczona
	}
//przerwanie
void SysTick_Handler(void)
{
  GPIO_WriteBit(GPIOB, GPIO_Pin_15, (BitAction)(1-GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_15)));
}




// z laborki

