
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __stm32_ButtonBehavior_H
#define __stm32_ButtonBehavior_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void ButtonBehavior(uint8_t BehaviorIns);
void Layer2Behavior(uint8_t Layer2BehaviorIns);	 
	 	 
uint8_t _SetupModeBehavior(void);
uint8_t _3itemBehavior(void);
	 
void allPageSetting(void);

/* Application ---------------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif /* __STM32L1xx_IT_H */

/******************* KINPO RDC *****END OF FILE****/
