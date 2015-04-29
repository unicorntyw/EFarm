
#include <string.h>
#include "main.h"
#include "stdlib.h"
#include "screen.h"
#include "control.h"
#include "stm32_rtc.h"
#include "keyWatcher.h"

#define SEC_FLAG (TIM6->SR & TIM_SR_UIF) 
#define CO2_VOL_L_THRES 1000
#define TMPT_L_THRES 20
#define TMPT_H_THRES 25
#define HEAT_H_THRES 21//heat up stop at 22
#define COOL_L_THRES 24//cool down stop at 23

#define TMPT_LOW	10
#define TMPT_HIGH	45
#define HMDT_LOW	0
#define HMDT_HIGH	95
#define TMPT_DIFF 1.0

QUEUE events;
volatile bool RtcAlarmFlag=FALSE;
volatile bool CasetOK=FALSE;
volatile bool THSnrOn=FALSE;
volatile uint8_t SecCnt=0, MinCnt=0;
float tmpt=23.0;
float hmdt=70.0; 
uint16_t	numDays=0;
uint8_t PlantStage=0;
bool heatOn=FALSE, coolOn=FALSE;

void HandleRtcAlarm(void);
bool THSnrInit(void);

char* PMode2Str(eFarm_plant_mode ModeIns){
	switch(ModeIns){
		default:
		case Auto1:
			return "Auto 1";
		case Auto2:
			return "Auto 2";
		case Manual:
			return "Manual";
		case UnSet:
			return " N/A  ";
	}
}

uint16_t GetNumDays(){
	return numDays;
}

void SetNumDays(int16_t days)
{	
	RTC_TimeTypeDef tmpTimeStruct;
	
	if(days > -1)
		numDays=days;
	else
		numDays=0;
	
	switch(days)
	{	
		case 0:
			PlantStage=0;
		break;
		case 1:
			PlantStage=1;
		break;
		default:
		case 2:case 3:case 4:case 5:case 6:case 7:case 8:case 9:
		case 10:case 11:case 12:case 13:case 14:case 15:
		//	PlantStage=2; //Removed by request
		//break;
		case 16:case 17:case 18:case 19:case 20:case 21:case 22:
		case 23:case 24:case 25:case 26:case 27:case 28:case 29:
		case 30:case 31:case 32:case 33:case 34:case 35:
			RTC_GetTime(RTC_Format_BIN, &tmpTimeStruct);
			if(PlantMode==Auto1)
			{
				if(tmpTimeStruct.RTC_Hours<8)
					PlantStage=4;
				else
					PlantStage=3;
			}else
			if(PlantMode==Auto2)
			{
				if(tmpTimeStruct.RTC_Hours>=8 && tmpTimeStruct.RTC_Hours<20)
					PlantStage=3;
				else
					PlantStage=4;
			}
			else
				PlantStage=5;
		break;
		/* //Removed by Request
		default:
			PlantStage=5;
		break;
			*/
	}
}

void AddEvent(Key_Event event){
	QNODE* pEvtNode = malloc(sizeof(QNODE));
	pEvtNode->next = NULL;
	pEvtNode->event=event;
	queue_add(&events, pEvtNode);
}

QNODE* GetEvent(){
	return queue_remove(&events);
}

void SetUpPlantEnv()
{
	WriteBkUpInfor();
	switch(PlantStage)
	{
		case 0:
		case 1:
			AdjustLEDLevel(0);
			SetupAlarmTime(8,0, RTC_H12_AM);
			break;
		case 2:
			AdjustLEDLevel(savBrightLevel);
			SetupAlarmTime(8,0, RTC_H12_AM);
			break;
		case 3:
			AdjustLEDLevel(savBrightLevel);
			switch(PlantMode)
			{
					case Auto1://16hrs
						SetupAlarmTime(0,0, RTC_H12_AM);
					break;
					case Auto2://12hrs
						SetupAlarmTime(20,0, RTC_H12_PM);
					break;
					default:
					SetupAlarmTime(8,0, RTC_H12_AM);//Error prevention.
					break;
			}
			break;
		default:	
		case 4:
			AdjustLEDLevel(0);
			SetupAlarmTime(8,0, RTC_H12_AM);
			break;
		case 5:
			AdjustLEDLevel(savBrightLevel);
			SetupAlarmTime(8,0, RTC_H12_AM);
			break;
	}
}

void HandleRtcAlarm(){
#ifdef RTC_ALRM_DEBUG
	printf("RTC Alram PlantStage:%d Days:%d",PlantStage,numDays);
#endif
	RtcAlarmFlag=FALSE;
	if(PlantStage!=3)
		numDays++;
	//PlantStage increased inside SetNumDays()
	SetNumDays(numDays);
#ifdef RTC_ALRM_DEBUG
	printf("RTC Alram PlantStage:%d Days:%d",PlantStage,numDays);
#endif
	SetUpPlantEnv();
}

float AgHdmt=0, AgTmpt=0;
uint8_t hNum=0, tNum=0;
void MonitorTemperature(void){
	
	if(!THSnrOn){
		THSnrOn=THSnrInit();
		if(!THSnrOn)
		return ;
	}
	
	if(ScreenState!=debug2)
	{
		float tempVal=0, diffVal=0;
		if(!IsTHSrErr()){
			
			tempVal=(GetTemperature()-1);
			diffVal=tmpt-tempVal;
			if( diffVal<=TMPT_DIFF && diffVal>=TMPT_DIFF*-1){
				tmpt=tempVal;
			}
			else if (diffVal>0){
				tmpt-=TMPT_DIFF;
			}
			else if (diffVal<0){
				tmpt+=TMPT_DIFF;
			}
				
			tempVal=(GetHumidity()-15);
			if(tempVal<HMDT_LOW)
				hmdt=HMDT_LOW;
			else if (tempVal>HMDT_HIGH)
				hmdt=HMDT_HIGH;
			else
				hmdt=tempVal;
		}

		if(ScreenState==main_page)
			ShowTptrHmdt(tmpt, hmdt);
	}
	
#ifdef COOL_HEAT_ENABLE	
	if(tmpt>TMPT_H_THRES && !coolOn)
	{
		StartCooling1();
		StartCooling2();
		coolOn=TRUE;
	}
	else
	if(coolOn && tmpt<COOL_L_THRES)
	{
		StopCH1();
		StopCH2();
		coolOn=FALSE;
	}
	else
	if(tmpt<TMPT_L_THRES && !heatOn)
	{	
		StartHeating1();
		StartHeating2();
		heatOn=TRUE;
	}
	else
	if(heatOn && tmpt>HEAT_H_THRES)
	{
		StopCH1();
		StopCH2();
		heatOn=FALSE;
	}
#endif	
}

void Monitor(void){
	
	if(RtcAlarmFlag){
		HandleRtcAlarm();
		if(ScreenState==main_page){
			ShowPlantInfor(PMode2Str(PlantMode), numDays);
		}
	}
	
	if(!SEC_FLAG)
		return;
	TIM6->SR = TIM6->SR & ~TIM_SR_UIF;//Clear Flag
	
	if(++SecCnt==60){
		SecCnt=0;
		if(++MinCnt==60)
			MinCnt=0;
	}
#ifdef DEBUG	
	if(SecCnt%10==0)
#endif
		MonitorTemperature();
	
	if(CO2On){
		if(MinCnt==2)
		{
			if(SecCnt==0)
				StartCO2();
			else
			if(SecCnt==5)
				StopCO2();
		}	
		if(SecCnt%7==0){
			uint32_t CO2Vol=ReadCO2Vol();
			if(ScreenState==main_page){
				if(CO2Vol < CO2_VOL_L_THRES)
					ShowMessage("CO2 Level Low!!");
				else
					ShowMessage("XYZ E-Farm.     ");
			}
		}
	}
	
	if(SecCnt%5==0 && NFCOn)
	{
		if(CasetOK!=NFC_Check()){
			CasetOK=!CasetOK;
			if(CasetOK)
				GoTo(set_plant_mode);
			else{
				PlantMode=UnSet;
			}
		}
		if(!CasetOK){
			if(ScreenState==main_page){
				ShowPlantInfor(PMode2Str(PlantMode), numDays);
				ShowMessage("Casset Missing!!");
			}
		}
	}
	
	if(ScreenState==main_page){
		ShowRtcDateTime();
		if(SecCnt%2==0){
			SetAC(Row1,13);
			DisplayChar(':');
		}
	}
}

void BasicTimerInit()
{	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Prescaler=(SystemCoreClock/1000)-1;//48000-1 Timer clock 48M/48K=1K(Hz)
	TIM_TimeBaseStructure.TIM_Period=1000;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	TIM_Cmd(TIM6, ENABLE);
}

void InitReading(){
		float tempVal=GetTemperature();
		if(!IsTHSrErr()){
			if(tempVal>TMPT_LOW && tempVal<TMPT_HIGH)
				tmpt=tempVal;
		}
		tempVal=GetHumidity();
		if(!IsTHSrErr()){
			if(tempVal>HMDT_LOW && tempVal<HMDT_HIGH)
				hmdt=tempVal;
		}
}

bool THSnrInit(void){
	return HTS221_Init() && CalibHTS221();
}

int main(void){	
	
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f0xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f0xx.c file
     */

#ifdef ENG_DEBUG_U1	
	Usart1Init();
#endif
	Usart2Init();
	I2CInit();
	
	StoredInfor infor;
	STM32RTC_Init(&infor);
	
	SystemCoreClockUpdate();
	InitKey();
	SysTick_Config(SystemCoreClock/1000);//Do NOT call again in the entire system
	
	queue_init(&events);
	//BtnGpioExtiInit();
#ifdef ENG_DEBUG_U2
	NFCOn=FALSE;
#else
//NFCOn=NFC_Init();
	NFC_Init();
#endif
	CO2_Init();
	CO2_SensorInit();
#ifdef COOL_HEAT_ENABLE 	
	CoolHeatInit();
#endif
#ifndef ENG_DEBUG_U1
	LightInit();
#endif
	LCDWH1604_Init();
	//CGRAM();
	ShowMessage("XYZ E-Farm.     ");
	
	THSnrOn=THSnrInit();
#ifdef HTS221_DEBUG
	printf("-----------------------------------\n\r");
	printf("THSnrOn=%d\n\r",THSnrOn);
	printf("-----------------------------------\n\r");
#endif
	
	FanPumpInit();
	StartFan();
	StartPump();
	
	STM32RTCAlarm_Init();
	BasicTimerInit();
	
	if(infor.inited){
		PlantMode=infor.mode;
		SetNumDays(infor.days);
		SetUpPlantEnv();
		GoTo(main_page);
	}
	else{
		PlantMode=UnSet;
		GoTo(set_time);
	}
	
	if(PlantMode!=UnSet)
		CasetOK=TRUE;;
	
  while (1){
		Monitor();
		stateFunc();
	}
	
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line){ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while(1){
  }
}
#endif
