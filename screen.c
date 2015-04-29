#include "main.h"
#include <string.h>

static char tempStr[17];
static int size = sizeof(tempStr);


void ShowRtcDateTime(void){
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);
	RTC_DateTypeDef RTC_DateStruct;
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
	
	memset(tempStr,0,sizeof(tempStr));
	sprintf(tempStr, "20%02d %02d/%02d %02d %02d",
										RTC_DateStruct.RTC_Year, 
										RTC_DateStruct.RTC_Month,
	                  RTC_DateStruct.RTC_Date,
										RTC_TimeStruct.RTC_Hours,
	                  RTC_TimeStruct.RTC_Minutes);
	SetAC(Row1,0);
	DisplayStr((unsigned char *)tempStr);
}

void ShowTptrHmdt(float tmpt, float hmdt){
	memset(tempStr, 0,sizeof(tempStr));
	sprintf(tempStr, "T.%2.1f%cC H.%2.0f%c  ", tmpt, 0xDF, hmdt, 0x25);
	SetAC(Row3,0);
	DisplayStr((unsigned char *)tempStr);
}

void ShowPlantInfor(char* mode, uint16_t days){
	memset(tempStr,0,sizeof(tempStr));
	sprintf(tempStr, "%02d Days   %s",days,mode);
	SetAC(Row2,0);
	DisplayStr((unsigned char *)tempStr);
}

void ShowMessage(char* msg){
	memset(tempStr,0,sizeof(tempStr));
	sprintf(tempStr, "%s",msg);
	SetAC(Row4,0);
	DisplayStr((unsigned char *)tempStr);
}

void ShowCO2Vol(uint32_t vol){
	memset(tempStr,0,sizeof(tempStr));
	sprintf(tempStr, "CO2 Level=%d    ",vol);
	SetAC(Row4,0);
	DisplayStr((unsigned char *)tempStr);
}
/**
  * @brief  Main Page display - All Pattern
  * @param  None
  * @retval None
  */
void ShowMainPage(char* mode, uint16_t days, float tmpt, float hmdt){
	
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);
	RTC_DateTypeDef RTC_DateStruct;
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
	
	cleanLCD();
	
	memset(tempStr,0,sizeof(tempStr));
	sprintf(tempStr, "20%02d %02d/%02d %02d %02d",
										RTC_DateStruct.RTC_Year, 
										RTC_DateStruct.RTC_Month,
	                  RTC_DateStruct.RTC_Date,
										RTC_TimeStruct.RTC_Hours,
	                  RTC_TimeStruct.RTC_Minutes);
	SetAC(Row1,0);
	DisplayStr((unsigned char *)tempStr);
	
	memset(tempStr,0,sizeof(tempStr));
	sprintf(tempStr, "%02d Days   %s",days,mode);
	SetAC(Row2,0);
	DisplayStr((unsigned char *)tempStr);
	
	memset(tempStr,0,sizeof(tempStr));
	sprintf(tempStr, "T.%2.1f%cC H.%2.0f%c", tmpt, 0xDF, hmdt, 0x25);
	SetAC(Row3,0);
	DisplayStr((unsigned char *)tempStr);
	
	memset(tempStr,0,sizeof(tempStr));
	snprintf(tempStr, size, "XYZ E-Farm.     ");
	SetAC(Row4,0);
	DisplayStr((unsigned char *)tempStr); 
	
}

//extern volatile uint32_t PMCursorIdx;
void ShowPModeCursor(uint8_t Ins){

	switch(Ins){
		default:
		case 0: 
			/* cursor */
			writeIns(0xC1);
			DisplayChar(0x3E);
			writeIns(0x91);
			DisplayChar(0xFE);			
			writeIns(0xD1);
			DisplayChar(0xFE);
		break;
		
		case 1:
			writeIns(0xC1);
			DisplayChar(0xFE);
			/* cursor */
			writeIns(0x91);
			DisplayChar(0x3E);			
			writeIns(0xD1);
			DisplayChar(0xFE);
		break;
		
		case 2:
			writeIns(0xC1);
			DisplayChar(0xFE);
			writeIns(0x91);
			DisplayChar(0xFE);	
			/* cursor */
			writeIns(0xD1);
			DisplayChar(0x3E);
		break;
	}

}

void ShowPlantModePage(uint8_t pmIdx){

	cleanLCD();
	
	memset(tempStr,0,sizeof(tempStr));
	sprintf(tempStr, "Setup Mode");
	SetAC(Row1,0);
	DisplayStr((unsigned char *)tempStr); 

	memset(tempStr,0,sizeof(tempStr));
	sprintf(tempStr, "Auto 1");
	SetAC(Row2,3);
	DisplayStr((unsigned char *)tempStr); 

	memset(tempStr,0,sizeof(tempStr));
	sprintf(tempStr, "Auto 2");
	SetAC(Row3,3);
	DisplayStr((unsigned char *)tempStr);
/*	
	memset(tempStr,0,sizeof(tempStr));
	sprintf(tempStr, "Manual");
	SetAC(Row4,3);
	DisplayStr((unsigned char *)tempStr);
*/
	ShowPModeCursor(pmIdx);

}

void ShowSetTime(RTC_DateTypeDef *RTC_DateStruct, RTC_TimeTypeDef *RTC_TimeStruct ){
	
	cleanLCD();
	
	memset(tempStr,0,sizeof(tempStr));
	sprintf(tempStr, "Setup Time");
	SetAC(Row1,0);
	DisplayStr((unsigned char *)tempStr); 
	
	ShowChangedTime(RTC_DateStruct, RTC_TimeStruct, 4);
}

void ShowChangedTime(RTC_DateTypeDef *RTC_DateStruct, RTC_TimeTypeDef *RTC_TimeStruct, uint8_t idx )
{
	memset(tempStr,0,sizeof(tempStr));
	sprintf(tempStr, "20%02d %02d/%02d %02d:%02d",
										RTC_DateStruct->RTC_Year, 
										RTC_DateStruct->RTC_Month,
	                  RTC_DateStruct->RTC_Date,
										RTC_TimeStruct->RTC_Hours,
	                  RTC_TimeStruct->RTC_Minutes);
	SetAC(Row3,0);
	DisplayStr((unsigned char *)tempStr);
	SetAC(Row4,0);
	DisplayStr((unsigned char *)"                ");
	SetAC(Row4,2+idx*3);
	DisplayStr((unsigned char *)"^^");
}


void ShowSetTemperature(void){
	
	cleanLCD();
	
	memset(tempStr,0,sizeof(tempStr));
	sprintf(tempStr, "Temperature");
	SetAC(Row1,0);
	DisplayStr((unsigned char *)tempStr); 
	
	memset(tempStr,0,sizeof(tempStr));
	sprintf(tempStr, "Press +/- key");
	writeIns(0xC1);
	DisplayStr((unsigned char *)tempStr); 
	
	memset(tempStr,0,sizeof(tempStr));
	sprintf(tempStr, "Max Temp.= ");
	SetAC(Row3, 1);
	DisplayStr((unsigned char *)tempStr); 
	
	memset(tempStr,0,sizeof(tempStr));
	sprintf(tempStr, "Min Temp.= ");
	SetAC(Row4, 1);
	DisplayStr((unsigned char *)tempStr); 
	
}
void ShowVersion(void){
	
	cleanLCD();
	
	memset(tempStr,0,sizeof(tempStr));
	sprintf(tempStr, "PCBA ver: EVT 1");
	SetAC(Row2,0);
	DisplayStr((unsigned char *)tempStr);

	memset(tempStr,0,sizeof(tempStr));
	sprintf(tempStr, "FW ver:%s",ver);
	SetAC(Row3,0);
	DisplayStr((unsigned char *)tempStr); 

}

void ShowSetCo2(void){

	cleanLCD();
	
	memset(tempStr,0,sizeof(tempStr));
	sprintf(tempStr, "CO2 Control");
	SetAC(Row1,0);
	DisplayStr((unsigned char *)tempStr); 
	
	memset(tempStr,0,sizeof(tempStr));
	sprintf(tempStr, "ON");
	SetAC(Row2,3);
	DisplayStr((unsigned char *)tempStr); 
	
	memset(tempStr,0,sizeof(tempStr));
	sprintf(tempStr, "OFF");
	SetAC(Row3, 3);
	DisplayStr((unsigned char *)tempStr);
}

void ShowDebug1(uint8_t NFCOn, uint8_t PumpOn, uint8_t FansOn, uint8_t CO2On){

	cleanLCD();
	
	memset(tempStr,0,sizeof(tempStr));
	if(NFCOn)
		sprintf(tempStr, "> NFC   On");
	else
		sprintf(tempStr, "> NFC   Off");
	SetAC(Row1,1);
	DisplayStr((unsigned char *)tempStr); 

	memset(tempStr,0,sizeof(tempStr));
	if(PumpOn)
		sprintf(tempStr, "Pump  On");
	else
		sprintf(tempStr, "Pump  Off");
	SetAC(Row2,3);
	DisplayStr((unsigned char *)tempStr); 

	memset(tempStr,0,sizeof(tempStr));
	if(FansOn)
		sprintf(tempStr, "Fans  On");
	else
		sprintf(tempStr, "Fans  Off");
	SetAC(Row3,3);
	DisplayStr((unsigned char *)tempStr);

	memset(tempStr,0,sizeof(tempStr));
	if(CO2On)
		sprintf(tempStr, "CO2   On");
	else
		sprintf(tempStr, "CO2   Off");
	SetAC(Row4,3);
	DisplayStr((unsigned char *)tempStr);
}

void ShowChangedDebug1(uint8_t idx, uint8_t val){
	switch(idx){
		default:
		case 0: 
			/* cursor */
			SetAC(Row1,1);
			DisplayChar(0x3E);
			writeIns(0xC1);
			DisplayChar(0xFE);
			writeIns(0x91);
			DisplayChar(0xFE);			
			writeIns(0xD1);
			DisplayChar(0xFE);
			SetAC(Row1,9);
		break;
		
		case 1: 
			SetAC(Row1,1);
			DisplayChar(0xFE);
			writeIns(0xC1);
			DisplayChar(0x3E);
			writeIns(0x91);
			DisplayChar(0xFE);			
			writeIns(0xD1);
			DisplayChar(0xFE);
		  SetAC(Row2,9);
		break;
		
		case 2:
			SetAC(Row1,1);
			DisplayChar(0xFE);
			writeIns(0xC1);
			DisplayChar(0xFE);
			/* cursor */
			writeIns(0x91);
			DisplayChar(0x3E);
			writeIns(0xD1);
			DisplayChar(0xFE);
			SetAC(Row3,9); 
		break;
		
		case 3:
			SetAC(Row1,1);
			DisplayChar(0xFE);
			writeIns(0xC1);
			DisplayChar(0xFE);
			writeIns(0x91);
			DisplayChar(0xFE);	
			/* cursor */
			writeIns(0xD1);
			DisplayChar(0x3E);
			SetAC(Row4,9);
		break;
	}
	
	if(val)
		DisplayStr((unsigned char *)"On ");
	else
		DisplayStr((unsigned char *)"Off");
	
}

void ShowDebug2(uint8_t idx, int16_t days){

	cleanLCD();
	
	memset(tempStr,0,sizeof(tempStr));
	sprintf(tempStr, "   Days  %d", days);
	SetAC(Row1,0);
	DisplayStr((unsigned char *)tempStr);
	
	memset(tempStr,0,sizeof(tempStr));
	sprintf(tempStr, "   TMPT  %2.0f", tmpt);
	SetAC(Row2,0);
	DisplayStr((unsigned char *)tempStr);
	
	switch(idx){
		default:
		case 0: 
			/* cursor */
			SetAC(Row1,1);
			DisplayChar(0x3E);
			writeIns(0xC1);
			DisplayChar(0xFE);
			writeIns(0x91);
			DisplayChar(0xFE);			
			/*
			writeIns(0xD1);
			DisplayChar(0xFE);
			SetAC(Row1,9);*/
		break;
		
		case 1: 
			SetAC(Row1,1);
			DisplayChar(0xFE);
			writeIns(0xC1);
			DisplayChar(0x3E);
			writeIns(0x91);
			DisplayChar(0xFE);			
			/*
			writeIns(0xD1);
			DisplayChar(0xFE);
		  SetAC(Row2,9);*/
		break;
	}
}

void ShowSetBrightness(uint8_t bLevel){
	
	cleanLCD();
	
	memset(tempStr,0,sizeof(tempStr));
	sprintf(tempStr, "Brightness");
	SetAC(Row1,0);
	DisplayStr((unsigned char *)tempStr);
	
	memset(tempStr,0,sizeof(tempStr));
	sprintf(tempStr, "Press +/- key");
	writeIns(0xC1);
	DisplayStr((unsigned char *)tempStr); 
		
	memset(tempStr,0,sizeof(tempStr));
	sprintf(tempStr, "Low");
	writeIns(0xD0);
	DisplayStr((unsigned char *)tempStr); 
		
	memset(tempStr,0,sizeof(tempStr));
	sprintf(tempStr, "High");
	writeIns(0xDC);
	DisplayStr((unsigned char *)tempStr); 
		
	ShowBrightLevel(bLevel);
}

void ShowBrightLevel(uint8_t Ins){
		/* Cursor */
	switch(Ins){
		
		case 0: 
			/* cursor */
			writeIns(0xD4);
			DisplayChar(0x20);
			writeIns(0xD6);
			DisplayChar(0x20);
			writeIns(0xD8);
			DisplayChar(0x20);
			writeIns(0xDA);
			DisplayChar(0x20);
		break;
		
		case 1: 
			/* cursor */
			writeIns(0xD4);
			DisplayChar(0xFF);
			writeIns(0xD6);
			DisplayChar(0x20);
			writeIns(0xD8);
			DisplayChar(0x20);
			writeIns(0xDA);
			DisplayChar(0x20);
		break;
		
		case 2:
			writeIns(0xD4);
			DisplayChar(0xFF);
			writeIns(0xD6);
			DisplayChar(0xFF);
			writeIns(0xD8);
			DisplayChar(0x20);
			writeIns(0xDA);
			DisplayChar(0x20);
		break;
		
		case 3:
			writeIns(0xD4);
			DisplayChar(0xFF);
			writeIns(0xD6);
			DisplayChar(0xFF);
			writeIns(0xD8);
			DisplayChar(0xFF);
			writeIns(0xDA);
			DisplayChar(0x20);
		break;
		
		case 4:
			writeIns(0xD4);
			DisplayChar(0xFF);
			writeIns(0xD6);
			DisplayChar(0xFF);
			writeIns(0xD8);
			DisplayChar(0xFF);
			writeIns(0xDA);
			DisplayChar(0xFF);
		break;
	}
	
}

void ShowIdle(void){
	cleanLCD();
	DisplayOff();
}
