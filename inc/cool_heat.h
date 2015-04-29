#ifndef __COOLHEAT_H
#define __COOLHEAT_H

#include "stm32f0xx.h"
#include <stdio.h>

void FanInit(void);
void CoolHeatInit(void);
void StartFan(void);
void StopFan(void);
void StartCooling(void);
void StopCooling(void);
void StartHeating(void);
void StopHeating(void);
void StartOnlyFan(void);
void StopOnlyFan(void);

#endif
