
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EFARM_USART_H
#define __EFARM_USART_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "stm32f0xx_it.h"
#include "main.h"

void Usart2Init(void);
void Usart1Init(void);
bool NFC_Init(void);
bool NFC_Check(void);
void NFC_Send(uint8_t* cmd, uint32_t num);	 
//void print(unsigned char *string);

	 
#ifdef __cplusplus
}
#endif

#endif
