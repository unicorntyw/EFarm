
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __stm32_rtc_H
#define __stm32_rtc_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "stm32f0xx_it.h"
#include "main.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void STM32RTC_Init(StoredInfor* infor);
void ShowRtcTime(void);
void ShowRtcTime_USART(void);
void ShowRtcDate(void);	
void setRTCTime(uint8_t hour, uint8_t min, uint8_t sec);	 
void setRTCDate(uint8_t year, uint8_t weekDay, uint8_t month, uint8_t date);
void WriteBkUpInfor(void);
void STM32RTCAlarm_Init(void);
void SetupAlarmTime(uint8_t Hours, uint8_t Min, uint8_t AM_PM);	 

/* Application ---------------------------------------------------------------*/
void getCurrentRTCValue(int8_t *hour, int8_t *min);
void getRTCCurrentValue(int16_t *RCT);
void GetRtcDateTime(RTC_DateTypeDef* RTC_DateStruct, RTC_TimeTypeDef* RTC_TimeStruct);

#ifdef __cplusplus
}
#endif

#endif /* __STM32L1xx_IT_H */

/******************* KINPO RDC *****END OF FILE****/
