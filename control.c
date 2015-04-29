#include <stdio.h>
#include <string.h>
#include "main.h"
#include "queue.h"
#include "system_stm32f0xx.h"
#include "stdlib.h"
#include "screen.h"
#include "control.h"
#include "stm32_rtc.h"

#define PM_NUM 2

bool touched=FALSE;
uint8_t PMCursorIdx=0, curBrightLevel=1, savBrightLevel=3, debugIdx=0;
uint8_t NFCOn=FALSE, PumpOn=TRUE, FansOn=TRUE, CO2On=FALSE;
eFarm_screen_state ScreenState;
eFarm_plant_mode PlantMode;
void (*stateFunc)(void);

RTC_DateTypeDef tmpDateStruct;
RTC_TimeTypeDef tmpTimeStruct;

void RunMainPage(void){
	
	QNODE* pEvNode = GetEvent();
	if(pEvNode==NULL)
		return;
	
	switch(pEvNode->event){
	case power_key_pressed:
		GoTo(idle);
		break;
	case setup_key_pressed:
		if(CasetOK || !NFCOn)
			GoTo(set_plant_mode);
		else
			GoTo(set_time);
		break;
	case switch_key_pressed:
	case minus_key_pressed:
	case plus_key_pressed:
	default:
		break;
	}
	free(pEvNode);
}

void RunSetPlantMode(void){
	
	QNODE* pEvNode = GetEvent();
	if(pEvNode==NULL)
		return;
	
	switch(pEvNode->event){
	case setup_key_pressed:
		if(PlantMode!=UnSet)
		{
			GoTo(set_time);
			break;
		}
		//else go through below case
	case switch_key_pressed:
		if(PlantMode!=PMCursorIdx)
			PlantMode=PMCursorIdx;
		SetNumDays(0);
		SetUpPlantEnv();
		WriteBkUpInfor();
		GoTo(main_page);
		break;
	case power_key_pressed:
		GoTo(idle);
		break;
	case minus_key_pressed:
		PMCursorIdx = (PMCursorIdx==0 ? (PM_NUM-1) : --PMCursorIdx);
		PMCursorIdx %= PM_NUM;
		ShowPModeCursor(PMCursorIdx);
		break;
	case plus_key_pressed:
		PMCursorIdx = ++PMCursorIdx % PM_NUM;
		ShowPModeCursor(PMCursorIdx);
		break;
		default:
		break;
	}
	free(pEvNode);
	
}

uint8_t *timeElement[]={ &tmpDateStruct.RTC_Year,
											    &tmpDateStruct.RTC_Month,
											    &tmpDateStruct.RTC_Date,
											    &tmpTimeStruct.RTC_Hours,
											    &tmpTimeStruct.RTC_Minutes};
uint8_t timeMax=59, timeMin=0,timeIdx=4;
void UpdateTimeRange()
{
	switch(timeIdx){
		case 4:
		timeMax=59; 
		timeMin=0;
		break;
		case 3:
		timeMax=23; 
		timeMin=0;
		break;
		case 2:
		timeMin=1;
		if(tmpDateStruct.RTC_Month==2){
			if(tmpDateStruct.RTC_Month%4)
				timeMax=28;
			else
				timeMax=29;
		}
		else if ( (tmpDateStruct.RTC_Month<8 && tmpDateStruct.RTC_Month%2) ||
							(tmpDateStruct.RTC_Month>7 && !(tmpDateStruct.RTC_Month%2) ) )
			timeMax=31;
		else
			timeMax=30;
		if(tmpDateStruct.RTC_Date > timeMax)
			tmpDateStruct.RTC_Date=timeMax;
		break;
		case 1:
			timeMin=1;
			timeMax=12;
		break;
		case 0:
			timeMax=99;
			timeMin=0;
		break;
	}
}

void RunSetTime(void){
	/*
	memset(&tmpDateStruct, 0, sizeof(RTC_DateTypeDef));
	memset(&tmpTimeStruct, 0, sizeof(RTC_TimeTypeDef));
	GetRtcDateTime(&tmpDateStruct, &tmpTimeStruct);
	ShowChangedTime(&tmpDateStruct, &tmpTimeStruct, timeIdx);
	*/
	
	QNODE* pEvNode = GetEvent();
	if(pEvNode==NULL)
		return;
	
	switch(pEvNode->event){
	case power_key_pressed:
		GoTo(idle);
		break;
	case setup_key_pressed:
		
		if(touched)
		{
			if(++timeIdx!=2){
				RTC_SetTime(RTC_Format_BIN, &tmpTimeStruct);
				RTC_SetDate(RTC_Format_BIN, &tmpDateStruct);
				//Must set day again to set new plant stage by new time setting
				SetNumDays(GetNumDays());
				SetUpPlantEnv();
				GoTo(main_page);
			}
			else{
				UpdateTimeRange();
				break;
			}
		}
		/*
		if(PlantMode==UnSet)
			GoTo(set_plant_mode);
		else 
		if(touched)
			GoTo(main_page);*/
		else
			GoTo(set_brightness);
		free(pEvNode);
		return;//Skip ShowChangedTime()
	case switch_key_pressed:
		timeIdx=++timeIdx%5;
		UpdateTimeRange();
		//ShowChangedTime(&tmpDateStruct, &tmpTimeStruct, timeIdx);
		break;
	case minus_key_pressed:
		touched=TRUE;
		if( *timeElement[timeIdx] > timeMin ){
			(*timeElement[timeIdx])--;
		}
		else{
			(*timeElement[timeIdx])=timeMax;
		}
		break;
	case plus_key_pressed:
		touched=TRUE;
		if( *timeElement[timeIdx] < timeMax ){
			(*timeElement[timeIdx])++;
		}
		else{
			(*timeElement[timeIdx])=timeMin;
		}
		break;
	default:
		break;
	}
	free(pEvNode);
	ShowChangedTime(&tmpDateStruct, &tmpTimeStruct, timeIdx);
	
}

void RunSetTemperature(void){
	
	QNODE* pEvNode = GetEvent();
	if(pEvNode==NULL)
		return;
	
	switch(pEvNode->event){
	case power_key_pressed:
		GoTo(idle);
		break;
	case setup_key_pressed:
		GoTo(set_brightness);
		break;
	case switch_key_pressed:
	case minus_key_pressed:
	case plus_key_pressed:
	default:
		break;
	}
	free(pEvNode);
	
}

void RunSetBrightness(void){
	
	QNODE* pEvNode = GetEvent();
	if(pEvNode==NULL)
		return;
	
	switch(pEvNode->event){
	case power_key_pressed:
		GoTo(idle);
		break;
	case setup_key_pressed:
		if(touched){
			GoTo(main_page);
			savBrightLevel=curBrightLevel;
		}
		else
			GoTo(show_ver);
		break;
	case switch_key_pressed:
		break;
	case minus_key_pressed:
		touched=TRUE;
		curBrightLevel = (curBrightLevel==0 ? 0 : --curBrightLevel);
		ShowBrightLevel(curBrightLevel);
		AdjustLEDLevel(curBrightLevel);
		break;
	case plus_key_pressed:
		touched=TRUE;
		curBrightLevel = (curBrightLevel==4 ? 4 : ++curBrightLevel);
		ShowBrightLevel(curBrightLevel);
		AdjustLEDLevel(curBrightLevel);
		break;
	default:
		break;
	}
	free(pEvNode);
	
}

void RunIdle(void){
	
	QNODE* pEvNode = GetEvent();
	if(pEvNode==NULL)
		return;
	
	switch(pEvNode->event){
	case power_key_pressed:
		DisplayOn();
		GoTo(main_page);
		break;
	case setup_key_pressed:
	case switch_key_pressed:
	case minus_key_pressed:
	case plus_key_pressed:
	default:
		break;
	}
	free(pEvNode);
}

void RunSetCO2(void){
	
	QNODE* pEvNode = GetEvent();
	if(pEvNode==NULL)
		return;
	
	switch(pEvNode->event){
	case power_key_pressed:
		GoTo(idle);
		break;
	case setup_key_pressed:
		GoTo(show_ver);
		break;
	case switch_key_pressed:
	case minus_key_pressed:
	case plus_key_pressed:
	default:
		break;
	}
	free(pEvNode);
}

void RunShowVer(void){
	
	QNODE* pEvNode = GetEvent();
	if(pEvNode==NULL)
		return;
	
	switch(pEvNode->event){
	case power_key_pressed:
		GoTo(idle);
		break;
	case setup_key_pressed:
		//GoTo(main_page);
		GoTo(debug1);
		break;
	case switch_key_pressed:
	case minus_key_pressed:
	case plus_key_pressed:
	default:
		break;
	}
	free(pEvNode);	
}

void Debug1Control()
{
	//SetNumDays(numDays);
	//WriteBkUpInfor();
	if(PumpOn)
		StartPump();
	else
		StopPump();
	
	if(FansOn)
		StartFan();
	else
		StopFan();
	
}

uint8_t* Debug1Items[4]={ &NFCOn, &PumpOn, &FansOn, &CO2On };
void RunDebug1(void){
	
	QNODE* pEvNode = GetEvent();
	if(pEvNode==NULL)
		return;
	
	switch(pEvNode->event){
	case power_key_pressed:
		GoTo(idle);
		break;
	case setup_key_pressed:
		if(touched)
			Debug1Control();
#ifdef RELEASE
		GoTo(main_page);
#else
		GoTo(debug2);
#endif
		free(pEvNode);
		return;
	case switch_key_pressed:
		debugIdx=(++debugIdx)%4;
		break;
	case minus_key_pressed:
		touched=TRUE;
		*(Debug1Items[debugIdx])=FALSE;
		break;
	case plus_key_pressed:
		touched=TRUE;	
		*(Debug1Items[debugIdx])=TRUE;
		break;
	default:
		break;
	}
	free(pEvNode);
	ShowChangedDebug1(debugIdx, *(Debug1Items[debugIdx]));
}

int16_t tmpDays=0;
int16_t tmpTmpt=0;
int16_t* Debug2Items[2]={ &tmpDays, &tmpTmpt };
void RunDebug2(void){
	
	QNODE* pEvNode = GetEvent();
	if(pEvNode==NULL)
		return;
	
	switch(pEvNode->event){
	case power_key_pressed:
		GoTo(idle);
		break;
	case setup_key_pressed:
		if( ((uint16_t)tmpDays)!=GetNumDays()){
			SetNumDays(tmpDays);
			SetUpPlantEnv();
		}
		GoTo(main_page);
		break;
	case switch_key_pressed:
		debugIdx=(++debugIdx)%2;
		ShowDebug2(debugIdx, tmpDays);
		break;
	case minus_key_pressed:
		touched=TRUE;
		(*(Debug2Items[debugIdx]))--;
		switch(debugIdx)
		{
			case 0:
				if( *(Debug2Items[debugIdx]) < 0 )
						*(Debug2Items[debugIdx])=0;
			break;
			default:
			case 1:
				tmpt=*(Debug2Items[debugIdx]);
			break;
		}
		ShowDebug2(debugIdx, tmpDays);
		break;
	case plus_key_pressed:
		touched=TRUE;
		(*(Debug2Items[debugIdx]))++;
		if( debugIdx == 1)
			tmpt=*(Debug2Items[debugIdx]);
		ShowDebug2(debugIdx, tmpDays);
		break;
	default:
		break;
	}
	free(pEvNode);
}

void GoTo(eFarm_screen_state aState)
{	
	ScreenState=aState;
	touched=FALSE;
	
	switch(aState)
	{
		case main_page:
			ShowMainPage( PMode2Str(PlantMode), GetNumDays(), tmpt, hmdt);
		  stateFunc=RunMainPage;
			break;
		case set_plant_mode:
			PMCursorIdx=PlantMode%3;		 //Do NOT modify PlantMode by now for comparison
			ShowPlantModePage(PMCursorIdx);
			stateFunc=RunSetPlantMode;
			break;
		case set_time:
			timeIdx=4;
			UpdateTimeRange();
			memset(&tmpDateStruct, 0, sizeof(RTC_DateTypeDef));
			memset(&tmpTimeStruct, 0, sizeof(RTC_TimeTypeDef));
			GetRtcDateTime(&tmpDateStruct, &tmpTimeStruct);
			ShowSetTime(&tmpDateStruct, &tmpTimeStruct);
			stateFunc=RunSetTime;
			break;
		case set_temperature:
			ShowSetTemperature();
			stateFunc=RunSetTemperature;
			break;
		case set_brightness:
			ShowSetBrightness(curBrightLevel);
			stateFunc=RunSetBrightness;
			break;
		case show_ver:
			ShowVersion();
			stateFunc=RunShowVer;
			break;
		case set_co2:
			ShowSetCo2();
			stateFunc=RunSetCO2;
			break;
		case idle:
			ShowIdle();
			stateFunc=RunIdle;
			break;
		case debug1:
			debugIdx=0;
			ShowDebug1(NFCOn, PumpOn, FansOn, CO2On);
			stateFunc=RunDebug1;
			break;
		case debug2:
			debugIdx=0;
			tmpTmpt=tmpt;
			tmpDays=GetNumDays();
			ShowDebug2(debugIdx, tmpDays);
			stateFunc=RunDebug2;
			break;
	}
}
