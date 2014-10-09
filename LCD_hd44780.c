/**************LCD_HD44780*************/
//dodajemy biblioteki hd44780
//w main
unsigned char licznikTekst[5] = {"0\0"}; //tablica na przekonwertowana liczbe na ASCI
LCD_Initialize();                   //inicjalizacja wysietlacza
LCD_WriteCommand(HD44780_CLEAR);    //wyczysc wyswietlacz
LCD_WriteText("Odliczanie:");     //wyswietl tekst pierwszej linii
sprintf((char *)licznikTekst,"%2i",licznik); //zamiana zmiennej licznik(int) na string i przypisanie do tablicy
LCD_WriteTextXY(licznikTekst,0,1);  //wyswietl napis poz XY (kolumna/wiersz)
