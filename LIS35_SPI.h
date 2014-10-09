#define CS GPIO_Pin_4

#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define HP_FILTER_RESET 0x23
#define STATUS_REG 0x27
#define OUT_X 0x29
#define OUT_Y 0x2B
#define OUT_Z 0x2D
#define CLICK_CFG 0x38
#define CLICK_SRC 0x39
#define CLICK_THS_Y_X 0x3B
#define CLICK_THS_Z 0x3C
#define CLICK_TIMELIMIT 0x3D
#define CLICK_LATENCY 0x3E
#define CLICK_WINDOW 0x3F
void Config_LIS(void);
char SPI_Transmit(char Data);
void SPI_CS_Enable(void);
void SPI_CS_Disable(void);
void SPI_Read(char adr, char*odb);
void SPI_Write(char adr, char msg);
char Init_LIS35(void);
