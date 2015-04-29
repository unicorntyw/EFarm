
#ifndef __gpio_H
#define __gpio_H

#ifdef __cplusplus
 extern "C" {
#endif 

void PumpInit(void);
void StartPump(void);
void StopPump(void);
void FanPumpInit(void);
void CoolHeatInit(void);
void StartFan(void);
void StopFan(void);
void StartHeating1(void);
void StartHeating2(void);
void StartCooling1(void);
void StartCooling2(void);
void StopCH1(void);
void StopCH2(void);
//void StartOnlyFan(void);
//void StopOnlyFan(void);

	 
#ifdef __cplusplus
}
#endif

#endif
