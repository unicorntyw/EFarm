
#ifndef __LIGHT_H
#define __LIGHT_H

#ifdef __cplusplus
 extern "C" {
#endif 
#include "main.h"

void LightInit(void);
void SetDutyCycle(uint16_t duty); 
void AdjustLEDLevel(uint8_t level);
void LEDOnOff(uint8_t Ins);
//void MainLedOn(void);
//void MainLedOff(void);

#ifdef __cplusplus
}
#endif

#endif 
