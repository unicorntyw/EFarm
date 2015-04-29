
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __stm32_rtcAlarm_H
#define __stm32_rtcAlarm_H

#ifdef __cplusplus
 extern "C" {
#endif 
#include "main.h"
	 
/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void STM32RTCAlarm_Init(void);
void SetupAlarmTime(uint8_t Hours, uint8_t Min, uint8_t AM_PM);
/* Application ---------------------------------------------------------------*/
void AlarmSelector(uint8_t modeIns);
void AlarmMode(uint8_t modeIns, int16_t *SAT, uint8_t DayNight);

#ifdef __cplusplus
}
#endif

#endif /* __STM32L1xx_IT_H */

/******************* KINPO RDC *****END OF FILE****/
