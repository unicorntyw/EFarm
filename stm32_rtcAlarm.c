/**
  ******************************************************************************
  * @file    stm32_rtcAlarm.c
  * @author  Shinhua
  * @version 
  * @date    June-2014
  * @brief    
  *          
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t day_hour, day_min, night_hour, night_min;
uint8_t AlarmTime;
/* Private functions ---------------------------------------------------------*/
void Auto1AlarmClock(bool Ins, int16_t *SAT);
void Auto2AlarmClock(bool Ins, int16_t *SAT);

/**
  * @brief 	STM32 RTC Alarm function
  * @param  None
  * @retval None
  */
void STM32RTCAlarm_Init(void){
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

#ifdef DEBUG
	printf("STM32RTCAlarm_Init\n\r");
#endif

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
	
	RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_A, &RTC_AlarmStructure);
	RTC_ITConfig(RTC_IT_ALRA, ENABLE);
	RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
	RTC_ClearFlag(RTC_FLAG_ALRAF);
}

/* Kinpo E-Farm Alarm Selector Setup */
void AlarmSelector(uint8_t modeIns){
	switch(modeIns){
		case 0:
			day_hour 	 = 0x16;
			night_hour = 0x08;
			day_min = night_min = 0x00;

		break;
		
		case 1:
			day_hour   = 0x12;
			night_hour = 0x08;
			day_min = night_min = 0x00;
		break;
		
		case 2:
			day_hour   = 0x00;
			night_hour = 0x00;
			day_min = night_min = 0x00;
		break;
	}

#ifdef DEBUG
	printf("Alarm Selector, Day_Hour-Min: [%x:%x], Night_Hour-Min: [%x:%x]\n\r", 
					day_hour, day_min, night_hour, night_min);
	
	printf("AlarmTime: %04x\n\r", AlarmTime);
#endif	

}

/* idiot idiot idiot function idiot idiot idiot */
void AlarmMode(uint8_t modeIns, int16_t *SAT, uint8_t DayNight){
	switch(modeIns){
		case 0:
			Auto1AlarmClock(DayNight, &(*SAT));
		break;
		
		case 1:
			Auto2AlarmClock(DayNight, &(*SAT));
		break;
	}

}

/* EFarm Auto 1 mode, Day: 6 o'clock, Night: 22 o'clock */
void Auto1AlarmClock(bool Ins, int16_t *SAT){
	switch(Ins){
		case 0:
			*SAT = 6;
		  *SAT = (*SAT) * 100;
		break;
		
		case 1:
			*SAT = 22;
		  *SAT = (*SAT) * 100;
		break;
	}
}

/* EFarm Auto 1 mode, Day: 6 o'clock, Night: 18 o'clock */
void Auto2AlarmClock(bool Ins, int16_t *SAT){
	switch(Ins){
		case 0:
			*SAT = 6;
		  *SAT = (*SAT) * 100;
		break;
		
		case 1:
			*SAT = 18;
		  *SAT = (*SAT) * 100;
		break;
	}
}
/******************* KINPO Electronic RDC  END OF FILE *******************/
