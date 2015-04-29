
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __stm32_lcd_WH1604_H
#define __stm32_lcd_WH1604_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "stm32f0xx_it.h"
#include "main.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#define RS GPIO_Pin_0
#define RW GPIO_Pin_1
#define EN GPIO_Pin_2
	 
#define AC_Set_Bit	Bit7
#define Row1 0x00   // Character lcated_DDRAM address: 0x00|0x80
#define Row2 0x40		// Character lcated_DDRAM address: 0x40|0x80
#define Row3 0x10		// Character lcated_DDRAM address: 0x10|0x80
#define Row4 0x50		// Character lcated_DDRAM address: 0x50|0x80

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void LCD_GPIOsInit(void);
void LCDWH1604_Init(void);
void SetAC(uint8_t, uint8_t);

/* Application ---------------------------------------------------------------*/
void writeIns(unsigned char instruction);
void DisplayChar(unsigned char data);
void writeString(char len, unsigned char *MSG);
void DisplayStr(unsigned char *);
void checkBusy(void);
uint8_t checkBF(void);
void Lcd_Init(void);
void CGRAM(void);
void DisplayOn(void);
void DisplayOff(void);
void DisplayToggle(void);
void cleanLCD(void);
void cleanLCDLine(uint8_t Line);

#ifdef __cplusplus
}
#endif

#endif /* __STM32L1xx_IT_H */
