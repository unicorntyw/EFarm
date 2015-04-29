#include "main.h"
#include <string.h>
#include "stdint.h"

/* Private typedef -----------------------------------------------------------*/
GPIO_InitTypeDef LcdCtrlPin, LcdPwrPin;  
uint8_t Busyflag = 0x00;
uint8_t busy;

/* Private define ------------------------------------------------------------*/
#define Lcd_RS(x) x ? GPIO_SetBits(GPIOC, RS) : GPIO_ResetBits(GPIOC, RS)
#define Lcd_RW(x) x ? GPIO_SetBits(GPIOC, RW) : GPIO_ResetBits(GPIOC, RW)
#define Lcd_EN(x) x ? GPIO_SetBits(GPIOC, EN) : GPIO_ResetBits(GPIOC, EN)

#define DB0 GPIO_Pin_3
#define DB1 GPIO_Pin_6
#define DB2 GPIO_Pin_7
#define DB3 GPIO_Pin_8
#define DB4 GPIO_Pin_9
#define DB5 GPIO_Pin_10
#define DB6 GPIO_Pin_11
#define DB7 GPIO_Pin_12

/* Private variables ---------------------------------------------------------*/

/* Output Pin define */
const uint16_t pin[] = {
	GPIO_Pin_12,
	GPIO_Pin_11,
	GPIO_Pin_10,
	GPIO_Pin_9,
	GPIO_Pin_8,
	GPIO_Pin_7,
	GPIO_Pin_6,
	GPIO_Pin_3
};

unsigned char CGRAM1[8] = {0x02,0x05,0x02,0x00,0x00,0x00,0x00,0x00};

/* Private functions ---------------------------------------------------------*/
void send8bit(unsigned char);
size_t getlen(char const *);

/*----------------------------------------------------------------------------*/

/**
  * @brief  Control GPIO send a byte data via GIOP pin.
  * @param  None
  * @retval None
  */
void send8bit(unsigned char data){
	/*
	uint32_t BSRRval = ( ( data & (Bit7|Bit6|Bit5|Bit4|Bit3|Bit2|Bit1) ) << 6 ) 
	   							 | ( ( data & Bit0 ) << 3 );
	
	GPIOC->BSRR= (~BSRRval << 16) | BSRRval;
	*/
	
	unsigned char MASK = Bit7;
	unsigned char idx=0;
	for(;idx<8; idx++){
		if(data & MASK)
			GPIO_HIGH(GPIOC, pin[idx]);
		else
			GPIO_LOW(GPIOC, pin[idx]);
		MASK >>=1;
	}
	
}

/**
  * @brief  LCD GPIO PIN initial output pin, it is follow STM32 settings.
  * @param  None
  * @retval None
  */
void LCD_GPIOsInit(void){

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	LcdPwrPin.GPIO_Pin 	 = GPIO_Pin_1;
	LcdPwrPin.GPIO_Mode  = GPIO_Mode_OUT;
	LcdPwrPin.GPIO_Speed = GPIO_Speed_10MHz;
	LcdPwrPin.GPIO_OType = GPIO_OType_PP;
  LcdPwrPin.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &LcdPwrPin);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	LcdCtrlPin.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | 
										GPIO_Pin_6 |GPIO_Pin_7| GPIO_Pin_8 | GPIO_Pin_9 |
										GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	LcdCtrlPin.GPIO_Mode = GPIO_Mode_OUT;
  LcdCtrlPin.GPIO_OType = GPIO_OType_PP;
  LcdCtrlPin.GPIO_PuPd = GPIO_PuPd_NOPULL;
  LcdCtrlPin.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &LcdCtrlPin);
	
	GPIO_Write(GPIOC, 0x0000);
}

/**
  * @brief  LCD initial, follow ST7066 settings.
  * @param  None
  * @retval None
  */
void LCDWH1604_Init(void){

	LCD_GPIOsInit();
	// Develop note: Maybe need nsDelay function, the function is use msDelay functionn.
	msDelay(15);
	writeIns(0x38);
	msDelay(5);
	writeIns(0x38);
	msDelay(5);
	writeIns(0x38);
	msDelay(5);
	writeIns(0x38);
	msDelay(5);
	
	writeIns(0x08); // LCD DisplayOff
	msDelay(5);
	writeIns(0x01);	// Clean data of the Display
	msDelay(5);
	writeIns(0x0c); // LCD DisplyaON
	msDelay(5);
	writeIns(0x06); // cursor
	msDelay(5);
	
	GPIOA->BSRR |= Bit1;
}

void SetAC(uint8_t row, uint8_t column)
{
	writeIns( (row+column)|AC_Set_Bit);
}

/**
  * @brief 	Follow ST7066 settings, write in instruct register.
  * @param  None
  * @retval None
  */
void writeIns(unsigned char instruction){
	Lcd_EN(0);	// Chip enable signal, High: Disable, Low: Enable
	__NOP();
	Lcd_RS(0);	// Write instruction, 
	__NOP();
	Lcd_RW(0);	
	__NOP();
	
	send8bit(instruction);
	
	Lcd_EN(1);	
	__NOP();
	Lcd_EN(0);
	msDelay(1);
	checkBusy();
}

/**
  * @brief 	Follow ST7066 settings, write in CGROM (Character Generator ROM Pattern) register.
  * @param  None
  * @retval None
  */
void DisplayChar(unsigned char data){
	Lcd_RS(1);	// Write instruction, 
	__NOP();
	Lcd_RW(0);	
	__NOP();
	
	send8bit(data);
	
	Lcd_EN(1);	
	__NOP();
	Lcd_EN(0);
	msDelay(1);
	checkBusy();
}

/**
  * @brief 	"Known" string lenth, variable declare of writeString().
  * @param  None
  * @retval None
	* @declare
	unsigned char MSG1[0x09] = "Germinate";
	unsigned char MSG2[0x0b] = "Temperature";
	unsigned char MSG3[0x08] = "Humility";
	unsigned char MSG4[0x03] = "Casket";	
	*/
void writeString(char len,unsigned char *MSG){
	char i;
	for(i=0; i<len; i++){
		DisplayChar(MSG[i]);
	}
}

void DisplayStr(unsigned char *MSG){
	uint8_t num=0;
	while(*MSG != 0 && num++<16){
	//while(*MSG != 0 ){
		DisplayChar(*MSG++);
	}
}

/**
  * @brief 	Follow ST7066 settings, check register state. (Not yet finish this function.)
  * @param  None
  * @retval None
  */
void checkBusy(void){
	Lcd_RS(0);
	Lcd_RW(1);
	
	do{
		Lcd_EN(1);
		busy = GPIO_ReadOutputDataBit(GPIOC, DB7);
		Lcd_EN(0);
	}while(0);

}

/**
  * @brief 	Check LCD state, test pattern function
  * @param  None
  * @retval None
  */
void CGRAM(void){
	unsigned char x;
	send8bit(0x40);
	for(x=0 ; x<8; x++){
		DisplayChar(CGRAM1[x]);
	}
	
}

bool on = FALSE;

/**
  * @brief 	LCO Display "ON" & "OFF" function 
  * @param  None
  * @retval None
  */
void DisplayOn(void){
	GPIOA->BSRR |= Bit1;
	writeIns(0x0c);
	msDelay(5);
	on = TRUE;
}
void DisplayOff(void){
	GPIOA->BRR |= Bit1;
	writeIns(0x08);
	msDelay(5);
	on = FALSE;
}

void DisplayToggle(void){
	if(on)
		DisplayOff();
	else
		DisplayOn();
}

/**
  * @brief 	LCO Display "Clean" function 
  * @param  None
  * @retval None
  */
void cleanLCD(void){
	writeIns(0x01);
	msDelay(5);
}

void cleanLCDLine(uint8_t Line){
	uint8_t  i, start, end;
	switch(Line){
		case 1:
			start = 0x80;
				end = 0x8F;
			break;
		case 2:
			start = 0xC0;
			  end = 0xCF;		
			break;
		case 3:
			start = 0x90;
				end = 0x9F;
		 break;
		case 4:
			start = 0xD0;
				end = 0xDF;
			break;
	}
	
	for( i = start; i <= end; i++ ){
		writeIns(i);
		DisplayChar(' ');
	}

}

void Lcd_Wcmd(uint16_t cmd){
	// Develop note: Maybe need nsDelay function, the function is use msDelay functionn.
	
	Lcd_EN(0);	// Chip enable signal, High: Disable, Low: Enable
	__NOP();
	Lcd_RS(0);	// Write instruction, 
	__NOP();
	Lcd_RW(0);	
	__NOP();
	
	GPIOC->ODR = cmd;
	
	/*
	GPIOC->BSRRL = cmd<<8 & 0x00ff;
	GPIOC->BSRRH = ((~cmd)<<8) & 0x00ff;
	*/
	
	msDelay(1);//nsDelay(500);

	Lcd_EN(1);	
	msDelay(1);
	Lcd_EN(1);
	msDelay(1);//nsDelay(100);
}
/******************* KINPO Electronic RDC  END OF FILE *******************/
