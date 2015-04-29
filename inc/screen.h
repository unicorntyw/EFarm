
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef screen_H
#define screen_H

#ifdef __cplusplus
 extern "C" {
#endif 
#include "main.h"
void ShowRtcDateTime(void);
void ShowMainPage(char* mode, uint16_t days, float tmpt, float hmdt);
void ShowPlantModePage(uint8_t pmIdx);
void ShowSetTime(RTC_DateTypeDef *RTC_DateStruct, RTC_TimeTypeDef *RTC_TimeStruct );
void ShowSetBrightness(uint8_t bLevel);
void ShowVersion(void);	 
void ShowSetCo2(void);
void ShowSetTemperature(void);
void ShowTptrHmdt(float tmpt, float hmdt);
void ShowPlantInfor(char* mode, uint16_t days);
void ShowMessage(char* msg);
void ShowChangedTime(RTC_DateTypeDef *RTC_DateStruct, RTC_TimeTypeDef *RTC_TimeStruct, uint8_t idx );
void ShowDebug1(uint8_t NFCOn, uint8_t PumpOn, uint8_t FansOn, uint8_t CO2On);
void ShowDebug2(uint8_t idx, int16_t days);
void ShowChangedDebug1(uint8_t idx, uint8_t val);
void ShowIdle(void);
void ShowCO2Vol(uint32_t vol);	 
void ShowPModeCursor(uint8_t Ins);
void timerSetupPattern(uint8_t Ins, uint8_t data);
void tempSetupPattern(uint8_t tempIns);
void lightingTimerSetupPattern(uint8_t Ins, uint8_t data);
void ShowBrightLevel(uint8_t Ins);
void livingColorTimerSetupPattern(uint8_t Ins, uint8_t data);

#ifdef __cplusplus
}
#endif

#endif /* __STM32L1xx_IT_H */

/******************* KINPO RDC *****END OF FILE****/
