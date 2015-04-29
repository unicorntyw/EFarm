#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f0xx.h"
#include <stdio.h>
#include "eFarm.h"
#include "queue.h"
#include "stm32_rtcAlarm.h"
#include "usart.h"
#include "stm32_lcd_WH1604.h"
#include "stm32_I2C_HTS221.h"
#include "screen.h"
#include "airCoolHeat.h"
#include "light.h"
#include "co2.h"
#include "stm32_EFarm_exit.h"

#define undateNum (SystemCoreClock/200)

extern volatile bool RtcAlarmFlag;
extern volatile bool CasetOK;
extern float hmdt;  
extern float tmpt;

void msDelay(uint64_t mTime);	
void AddEvent(Key_Event event);
char* PMode2Str(eFarm_plant_mode mode);
void SetNumDays(int16_t days);
uint16_t GetNumDays(void);
void SetUpPlantEnv(void);
QNODE* GetEvent(void);

#endif
