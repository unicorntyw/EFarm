
#ifndef __TimeDelay_H
#define __TimeDelay_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "stm32f0xx.h"
#include "stm32f0xx_it.h"
#include "main.h"

//void Delay(__IO uint32_t nTime);
void msDelay(uint32_t mTime);
//void nsDelay(__IO uint32_t nTimer);
void TimingDelay_Decrement(void);
	
#ifdef __cplusplus
}
#endif

#endif /* __TimeDelay_H */
