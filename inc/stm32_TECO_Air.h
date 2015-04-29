
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __stm32_TECO_Air_H
#define __stm32_TECO_Air_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* Application ---------------------------------------------------------------*/
void STM32_CoolingGPIOInit(void);
void STM32_TECO_GPIOInit(void); 
void STM_FanGPIOInit(void);	 
	 
void STM32_TECO_PWMGPIOInit(void);
void STM32_TECO_TIMConfig(void);
void _dutyCycle(__IO uint16_t CCR);

void TECOAirControl(uint8_t Ins);
void CoolingControl(uint8_t Ins);	 
void TECO_test(uint8_t Ins);
	 
#ifdef __cplusplus
}
#endif

#endif /* __STM32L1xx_IT_H */

/******************* KINPO RDC *****END OF FILE****/
