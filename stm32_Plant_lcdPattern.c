/**
  ******************************************************************************
  * @file    stm32l_discovery_lcd.c
  * @author  Microcontroller Division
  * @version V1.0.3
  * @date    May-2013
  * @brief   This file includes driver for the glass LCD Module mounted on 
  *          STM32l discovery board MB963
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <string.h>
char stringTemp[16];
int size = sizeof(stringTemp);


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void _ModeSelect(uint8_t ModeIns, char *stringTemp);
/*----------------------------------------------------------------------------*/

/**
  * @brief  Main Page display - All Pattern
  * @param  None
  * @retval None
  */
void _mainPage(uint8_t modeIns){
	
	/* LCD_WH1604 Row - 1st */
	SetAC(Row1,0x0A);
	//sprintf(stringTemp, "Auto 1");
	//snprintf(stringTemp, size, "Auto 1"); // Avoid buffer overflow 
	//snprintf(stringTemp, size, sTemp);
	_ModeSelect(modeIns, stringTemp);
	directWriteString((unsigned char *)stringTemp); 
	
	#ifdef DEBUG
		printf("STATE_0: Main function - %s\n\r", stringTemp);
	#endif
	
	/* LCD_WH1604 Row - 2nd */
	SetAC(Row2,0);
	snprintf(stringTemp, size, "Temp."); // Avoid buffer overflow 
	directWriteString((unsigned char *)stringTemp);
	
	writeIns(0xCE);
	writeData(0xDF);
	writeIns(0xCF);
	writeData('C');
	
	/* LCD_WH1604 Row - 3th */
	SetAC(Row3,0);
	snprintf(stringTemp, size, "Humidity"); 
	directWriteString((unsigned char *)stringTemp); 
	
	writeIns(0x9F);
	writeData(0x25);
	
	/* LCD_WH1604 Row - 4th */
	SetAC(Row4,0);
	snprintf(stringTemp, size, "Casket  ,");
	directWriteString((unsigned char *)stringTemp); 

}


void _page1(void){
	#ifdef DEBUG
		printf("While loop in Page1: Setup Mode\n\r");
	#endif
	
	SetAC(Row1,0);
	sprintf(stringTemp, "Setup Mode");
	directWriteString((unsigned char *)stringTemp); 
	
	writeIns(0xC3);
	sprintf(stringTemp, "Auto 1");
	directWriteString((unsigned char *)stringTemp); 
	
	writeIns(0x93);
	sprintf(stringTemp, "Auto 2");
	directWriteString((unsigned char *)stringTemp);
	
	writeIns(0xD3);
	sprintf(stringTemp, "Manual");
	directWriteString((unsigned char *)stringTemp);
	
}
void _page2(void){
	#ifdef DEBUG
		printf("While loop in Page2: Setup Time\n\r");
	#endif
	
	SetAC(Row1,0);
	sprintf(stringTemp, "Setup Time");
	directWriteString((unsigned char *)stringTemp); 
	
	writeIns(0xC1);
	sprintf(stringTemp, "Please Press");
	directWriteString((unsigned char *)stringTemp); 
	
	writeIns(0x93);
	sprintf(stringTemp, "Switch Key");
	directWriteString((unsigned char *)stringTemp);
	
	writeIns(0xD5);
	sprintf(stringTemp, "To Set Time");
	directWriteString((unsigned char *)stringTemp);

}
void _page3(void){
	#ifdef DEBUG
		printf("While loop in Page3: Setup Temp.\n\r");
	#endif
	
	SetAC(Row1,0);
	sprintf(stringTemp, "Setup Temp.");
	directWriteString((unsigned char *)stringTemp); 
	
	writeIns(0xC1);
	sprintf(stringTemp, "Please Press");
	directWriteString((unsigned char *)stringTemp); 
	
	writeIns(0x93);
	sprintf(stringTemp, "Switch Key");
	directWriteString((unsigned char *)stringTemp);
	
	writeIns(0xD4);
	sprintf(stringTemp, "To Set Temp.");
	directWriteString((unsigned char *)stringTemp);
	
}
void _page4(void){
	#ifdef DEBUG
		printf("While loop in Page4: Lighting Timing\n\r");
	#endif 

	SetAC(Row1,0);
	sprintf(stringTemp, "Lighting Timer");
	directWriteString((unsigned char *)stringTemp); 

	writeIns(0xC1);
	sprintf(stringTemp, "Please Press");
	directWriteString((unsigned char *)stringTemp); 
	
	writeIns(0x93);
	sprintf(stringTemp, "Switch Key");
	directWriteString((unsigned char *)stringTemp);
	
	writeIns(0xD4);
	sprintf(stringTemp, "To Set Timer");
	directWriteString((unsigned char *)stringTemp);

}
void _page5(void){
	#ifdef DEBUG
		printf("While loop in Page5: Brightness\n\r");
	#endif
	
	SetAC(Row1,0);
	sprintf(stringTemp, "Brightness");
	directWriteString((unsigned char *)stringTemp);
	
	writeIns(0xC1);
	sprintf(stringTemp, "Please Press");
	directWriteString((unsigned char *)stringTemp); 
	
	writeIns(0x93);
	sprintf(stringTemp, "Switch Key");
	directWriteString((unsigned char *)stringTemp);
	
	writeIns(0xD6);
	sprintf(stringTemp, "To Set LED");
	directWriteString((unsigned char *)stringTemp);
}

void _page6(void){
	#ifdef DEBUG
		printf("While loop in Page6: Living Colors\n\r");
	#endif 
	
	SetAC(Row1,0);
	sprintf(stringTemp, "Living Colors");
	directWriteString((unsigned char *)stringTemp); 
	
	writeIns(0xC3);
	sprintf(stringTemp, "Red");
	directWriteString((unsigned char *)stringTemp); 
	
	writeIns(0x93);
	sprintf(stringTemp, "Green");
	directWriteString((unsigned char *)stringTemp);
	
	writeIns(0xD3);
	sprintf(stringTemp, "Blue");
	directWriteString((unsigned char *)stringTemp);

}



void _page7(void){
	#ifdef DEBUG
		printf("While loop in Page7: LivingColor\n\r");
	#endif 
	
	SetAC(Row1,0);
	sprintf(stringTemp, "LivingColor");
	directWriteString((unsigned char *)stringTemp);
	
	writeIns(0xC1);
	sprintf(stringTemp, "Please Press");
	directWriteString((unsigned char *)stringTemp); 
	
	writeIns(0x93);
	sprintf(stringTemp, "Switch Key");
	directWriteString((unsigned char *)stringTemp);
	
	writeIns(0xD4);
	sprintf(stringTemp, "To Set Timer");
	directWriteString((unsigned char *)stringTemp);
}

/* Page 1 & 6 button behavior pattern */
void _3item_cursor(uint8_t Ins){
	/* Cursor */
	switch(Ins){
		case 0: 
			/* cursor */
			writeIns(0xC1);
			writeData(0x3E);
			writeIns(0x91);
			writeData(0xFE);			
			writeIns(0xD1);
			writeData(0xFE);
		break;
		
		case 1:
			writeIns(0xC1);
			writeData(0xFE);
			/* cursor */
			writeIns(0x91);
			writeData(0x3E);			
			writeIns(0xD1);
			writeData(0xFE);
		break;
		
		case 2:
			writeIns(0xC1);
			writeData(0xFE);
			writeIns(0x91);
			writeData(0xFE);	
			/* cursor */
			writeIns(0xD1);
			writeData(0x3E);
		break;
	}
}

void _ModeSelect(uint8_t ModeIns, char *stringTemp){
	switch(ModeIns){
		case 0:
			snprintf(stringTemp, size, "Auto 1");
		break;
		
		case 1:
			snprintf(stringTemp, size, "Auto 2");
		break;
		
		case 2:
			snprintf(stringTemp, size, "Manual");
		break;
	}
	
}



/* Page 2 button behavior pattern */
void timerSetupPattern(uint8_t Ins, uint8_t data){
	writeIns(Ins);
	sprintf(stringTemp, "%02d", data);
	directWriteString((unsigned char *)stringTemp); 
}


/* Page 3 button behavior pattern */
void tempSetupPattern(uint8_t tempIns){
	writeIns(0x9B);
	sprintf(stringTemp, "%02d", tempIns);
	directWriteString((unsigned char *)stringTemp); 
}

/* Page 4 button behavior pattern */
void lightingTimerSetupPattern(uint8_t Ins, uint8_t data){
	writeIns(Ins);
	sprintf(stringTemp, "%02d", data);
	directWriteString((unsigned char *)stringTemp); 
}

void brightnessSetupPattern(uint8_t Ins){
		/* Cursor */
	switch(Ins){
		case 1: 
			/* cursor */
			writeIns(0xD4);
			writeData(0xFF);
			writeIns(0xD6);
			writeData(0x20);
			writeIns(0xD8);
			writeData(0x20);
			writeIns(0xDA);
			writeData(0x20);

		break;
		
		case 2:
			writeIns(0xD4);
			writeData(0xFF);
			writeIns(0xD6);
			writeData(0xFF);
			writeIns(0xD8);
			writeData(0x20);
			writeIns(0xDA);
			writeData(0x20);
		break;
		
		case 3:
			writeIns(0xD4);
			writeData(0xFF);
			writeIns(0xD6);
			writeData(0xFF);
			writeIns(0xD8);
			writeData(0xFF);
			writeIns(0xDA);
			writeData(0x20);
		break;
		
		case 4:
			writeIns(0xD4);
			writeData(0xFF);
			writeIns(0xD6);
			writeData(0xFF);
			writeIns(0xD8);
			writeData(0xFF);
			writeIns(0xDA);
			writeData(0xFF);
		break;
	}
	
}

/* Page 7 button behavior pattern */
void livingColorTimerSetupPattern(uint8_t Ins, uint8_t data){
	writeIns(Ins);
	sprintf(stringTemp, "%02d", data);
	directWriteString((unsigned char *)stringTemp); 
}

/******************* KINPO Electronic RDC  END OF FILE *******************/
