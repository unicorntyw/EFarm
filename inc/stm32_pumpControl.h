
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __stm32_pumpControl_H
#define __stm32_pumpControl_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* Application ---------------------------------------------------------------*/
void Pump_GPIOInit(void);
void StartPump(void);
void StopPump(void);
#ifdef __cplusplus
}
#endif

#endif /* __STM32L1xx_IT_H */

/******************* KINPO RDC *****END OF FILE****/
