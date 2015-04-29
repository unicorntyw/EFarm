#include "main.h"
#include "control.h"
#include <string.h>
#include "stm32_rtc.h"

RTC_InitTypeDef RTC_InitStructure;
RTC_TimeTypeDef RTC_TimeStruct;
RTC_DateTypeDef RTC_DateStruct;  

#define SENSOR_ADDRESS 0xD0 // or set this according to your HW configuration
#define HEX_2_DEC(val) (((val)/16)*10+((val)%16))
#define DEC_2_HEX(val) (((val)/10)*16+((val)%10))
#define BKP_VALUE    0x32F0 

char temp[8];

void OutputLsClock(void){
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_0);
  // Output LSI Clock on MCO pin
  RCC_MCOConfig(RCC_MCOSource_LSE, RCC_MCOPrescaler_1);
}

/**
  * @brief  Initial STM32 RTC parameter.
  * @param  None
  * @retval None
	* The LSEON, LSEBYP, RTCSEL and RTCEN bits of the RTC domain control register (RCC_BDCR) are in the RTC domain.
	* These bits are write-protected and the DBP bit in the Power control register (PWR_CR) has to be set before these can be modified.
	* These bits are only reset after a RTC domain Reset (see Section 7.1.3: RTC domain reset). 
	* Any internal or external Reset will not have any effect on these bits.
  */
void STM32RTC_Init(StoredInfor* infor){

	/* Enable the PWR clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	/* Allow access to Backup Domain Registers including RTC related ones */
  PWR_BackupAccessCmd(ENABLE);
	RTC_WriteProtectionCmd(DISABLE);
	uint32_t BackUpVal = RTC_ReadBackupRegister(RTC_BKP_DR0);

	if (BackUpVal == 0){		
			/* Reset Backup Domain */ 
			RCC_BackupResetCmd(ENABLE);
			RCC_BackupResetCmd(DISABLE);
			/*Turn LSE on*/
			RCC_LSEConfig(RCC_LSE_ON);
			/* Wait until LSE is ready */
			while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
			/* Select the RTC Clock Source */
			RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
			/* Enable the RTC */
			RCC_RTCCLKCmd(ENABLE);
			/* Wait for RTC APB registers synchronisation */
			RTC_WaitForSynchro();
			//Configure the RTC data register and RTC prescaler 
			RTC_InitStructure.RTC_AsynchPrediv = 0x7F;//128-1;
			RTC_InitStructure.RTC_SynchPrediv  = 0xFF;//256-1;
			RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;
			RTC_Init(&RTC_InitStructure);

			// Set the time to 00h 00mn 00s AM 
			//RTC_TimeStruct.RTC_H12     = RTC_H12_AM;
			//RTC_TimeStruct.RTC_Hours   = 0x00;
			//RTC_TimeStruct.RTC_Minutes = 0x00;
			//RTC_TimeStruct.RTC_Seconds = 0x00;
			//RTC_SetTime(RTC_Format_BCD, &RTC_TimeStruct);
			
			setRTCTime(18,55, 0);
	    setRTCDate(14, 1, 12, 10);
			infor->inited=FALSE;
			infor->mode=UnSet;
		  //RTC_WriteBackupRegister(RTC_BKP_DR0, BKP_VALUE);
	}
	else{
			/* Wait for RTC APB registers synchronisation */
			RTC_WaitForSynchro();
			/* Clear the RTC Alarm Flag */
			RTC_ClearFlag(RTC_FLAG_ALRAF);
			/* Clear the EXTI Line 17 Pending bit (Connected internally to RTC Alarm) */
			EXTI_ClearITPendingBit(EXTI_Line17);
			memcpy(infor,&BackUpVal, sizeof(StoredInfor));
	}
	
}

/**
  * @brief 	STM32 RTC Alarm function
  * @param  None
  * @retval None
  */
void STM32RTCAlarm_Init(void){

	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* EXTI configuration */
	EXTI_ClearITPendingBit(EXTI_Line17);
	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; // EXTI_Mode_Interrupt
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable the RTC Alarm Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

/* Setup STM32 Alarm Time */
void SetupAlarmTime(uint8_t Hours, uint8_t Min, uint8_t AM_PM){
	RTC_AlarmTypeDef RTC_AlarmStructure;
	RTC_AlarmCmd(RTC_Alarm_A, DISABLE);  
	
	RTC_AlarmStructure.RTC_AlarmTime.RTC_H12     = AM_PM;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours   = Hours;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = Min;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = 0x30;
	RTC_AlarmStructure.RTC_AlarmDateWeekDay			 = 0x31; // Nonspecific
	RTC_AlarmStructure.RTC_AlarmDateWeekDaySel	 = RTC_AlarmDateWeekDaySel_Date;
	RTC_AlarmStructure.RTC_AlarmMask						 = RTC_AlarmMask_DateWeekDay;
	
	RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);
	RTC_ITConfig(RTC_IT_ALRA, ENABLE);
	RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
	RTC_ClearFlag(RTC_FLAG_ALRAF);
}

void WriteBkUpInfor()
{
		StoredInfor infor;
		infor.inited=TRUE;
		infor.mode=PlantMode;
		infor.days=GetNumDays();
		uint32_t BackUpVal;
		memcpy(&BackUpVal, &infor, sizeof(StoredInfor));
		RTC_WriteBackupRegister(RTC_BKP_DR0, BackUpVal);
}

/**
  * @brief  Provide Main function get data of the RTC time.
  * @param  None
  * @retval None
  */
void ShowRtcTime(void){
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);

	/* Hours */
	sprintf(temp, "%02d", RTC_TimeStruct.RTC_Hours);	
	writeIns(0x00|0x80);
	DisplayStr((unsigned char *)temp);
	

	/* Minutes */
	sprintf(temp,"%02d",RTC_TimeStruct.RTC_Minutes);
	writeIns(0x03|0x80);
	DisplayStr((unsigned char *)temp); 
	
	//RealClockTime = RealClockTime * 100 + RTC_TimeStruct.RTC_Minutes;
	
	/* Seconds */
	/* Display ":" for second */
	writeIns(0x02|0x80);
	DisplayChar(0x3A);
	msDelay(500);
	writeIns(0x02|0x80);
	DisplayChar(0x20);
	msDelay(500);
	
	/* Display Second for digital
	sprintf(temp,"%d",RTC_TimeStruct.RTC_Seconds);
	writeIns(0x06|0x80);
	DisplayStr((unsigned char *)temp);
	*/
}

void ShowRtcTime_USART(void){
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);
	printf("RTC: %d:%d:%d\n\r", RTC_TimeStruct.RTC_Hours, RTC_TimeStruct.RTC_Minutes, RTC_TimeStruct.RTC_Seconds);
}

void getCurrentRTCValue(int8_t *hour, int8_t *min){
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);
	*hour = RTC_TimeStruct.RTC_Hours;
	*min = RTC_TimeStruct.RTC_Minutes;
}

void getRTCCurrentValue(int16_t *RCT){
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);
	*RCT =  RTC_TimeStruct.RTC_Hours;
	*RCT = (*RCT * 100) + RTC_TimeStruct.RTC_Minutes;
}

/**
  * @brief  Provide Main function get data of the RTC date.
  * @param  None
  * @retval None
  */
void ShowRtcDate(void){
	
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
	
	/* Year */
	sprintf(temp, "%d", RTC_DateStruct.RTC_Year);
	SetAC(Row1,0);
	DisplayStr((unsigned char *)temp); 
	__NOP();
	
	/* WeekDay */	
	sprintf(temp, "%d", RTC_DateStruct.RTC_WeekDay);
	SetAC(Row2,0);
	DisplayStr((unsigned char *)temp); 
	__NOP();
	
	/* Month */
	sprintf(temp, "%d", RTC_DateStruct.RTC_Month);
	SetAC(Row3,0);
	DisplayStr((unsigned char *)temp); 
	__NOP();
	
	/* Date */
	sprintf(temp, "%d", RTC_DateStruct.RTC_Date);
	SetAC(Row4,0);
	DisplayStr((unsigned char *)temp); 
	__NOP();
}

/**
  * @brief  Provide Main function to set RTC time.
  * @param  None
  * @retval None
  */
void setRTCTime(uint8_t hour, uint8_t min, uint8_t sec){
	RTC_TimeStruct.RTC_Hours   = hour;
	RTC_TimeStruct.RTC_Minutes = min;
	RTC_TimeStruct.RTC_Seconds = sec;
	RTC_TimeStruct.RTC_H12		 = 0;
	RTC_SetTime(RTC_Format_BIN, &RTC_TimeStruct);
}	

/**
  * @brief  Provide Main function to set RTC date.
  * @param  None
  * @retval None
  */
void setRTCDate(uint8_t year, uint8_t weekDay, uint8_t month, uint8_t date){
	RTC_DateStruct.RTC_Year    = year;
	RTC_DateStruct.RTC_WeekDay = weekDay;
	RTC_DateStruct.RTC_Month   = month;
	RTC_DateStruct.RTC_Date    = date;
	RTC_SetDate(RTC_Format_BIN, &RTC_DateStruct);
}


void GetRTC(long *hhmmss,long *yymmdd){  
  RTC_TimeTypeDef time;  
  //RTC_DateTypeDef date;  
  RTC_GetTime(RTC_Format_BIN,&time);  
  //RTC_GetDate(RTC_Format_BIN,&date);  
    	
	*hhmmss = time.RTC_Seconds;  
  *hhmmss = time.RTC_Hours*10000 + time.RTC_Minutes*100 + time.RTC_Seconds;  
  //*yymmdd = date.RTC_Year*10000 + date.RTC_Month*100 + date.RTC_Date;  
}

void GetRtcDateTime(RTC_DateTypeDef* RTC_DateStruct, RTC_TimeTypeDef* RTC_TimeStruct){
	RTC_GetDate(RTC_Format_BIN, RTC_DateStruct);
	RTC_GetTime(RTC_Format_BIN, RTC_TimeStruct);
}
	
/******************* KINPO Electronic RDC  END OF FILE *******************/
