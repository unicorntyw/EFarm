
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __stm32_Plant_lcdPattern_H
#define __stm32_Plant_lcdPattern_H

#ifdef __cplusplus
 extern "C" {
#endif 
#include "main.h"
/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* Application ---------------------------------------------------------------*/
void _mainPage(uint8_t modeIns);
void _page1(void);
void _page2(void);
void _page3(void);
void _page4(void);	 
void _page5(void);
void _page6(void);
void _page7(void);

	 
void _3item_cursor(uint8_t Ins);
void timerSetupPattern(uint8_t Ins, uint8_t data);
void tempSetupPattern(uint8_t tempIns);
void lightingTimerSetupPattern(uint8_t Ins, uint8_t data);
void brightnessSetupPattern(uint8_t Ins);
void livingColorTimerSetupPattern(uint8_t Ins, uint8_t data);

#ifdef __cplusplus
}
#endif

#endif /* __STM32L1xx_IT_H */

/******************* KINPO RDC *****END OF FILE****/
