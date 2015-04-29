
#ifndef __stm32_I2C_HTS221_H
#define __stm32_I2C_HTS221_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "stm32f0xx.h"
#include "stm32f0xx_it.h"
#include "main.h"

uint8_t WHO_AM_I(void);
uint8_t AV_CONF(void);
uint8_t checkHTS221(void);

void I2CInit(void);	 
void showRegData(void);
bool HTS221_Init(void);
bool CalibHTS221(void);
bool IsTHSrErr(void);
float GetTemperature(void);
float GetHumidity(void);
void showSensorInfo(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32L1xx_IT_H */
