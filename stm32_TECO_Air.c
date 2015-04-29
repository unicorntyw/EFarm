/**
  ******************************************************************************
  * @file    stm32_TECO_Air.c
  * @author  Shinhua
  * @version 
  * @date    June-2014
  * @brief    
  *          
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t PrescalerValue = 0;
__IO uint16_t CCR1_Val = 1000;

extern float DegC;

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  TECO_GPIO initial with PWM
  * @param  None
  * @retval None
  */
void STM32_TECO_PWMGPIOInit(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	#ifdef DEBUG
		printf("STM32_TECO_GPIOInit\n\r");
	#endif

  /* TIM3 clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_TIM3);
	
}

void STM32_TECO_GPIOInit(void){
	
	GPIO_InitTypeDef GPIO_InitStruct;
#ifdef DEBUG
	printf("STM32_CoolingGPIOInit with GPIO\n\r");
#endif
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}


void STM32_CoolingGPIOInit(void){
	GPIO_InitTypeDef GPIO_InitStructure;
#ifdef DEBUG
	printf("STM32_CoolingGPIOInit with GPIO\n\r");
#endif
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void STM_FanGPIOInit(void){
	GPIO_InitTypeDef GPIO_InitStructure;
#ifdef DEBUG
	printf("STM32_FanGPIOInit with GPIO\n\r");
#endif
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* High level on Pump*/ 	
	GPIO_HIGH(GPIOA, GPIO_Pin_5);	
	
}


/**
  * @brief  TECO_TIM Config
  * @param  None
  * @retval None
  */
void STM32_TECO_TIMConfig(void){

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
#ifdef DEBUG
	printf("STM32_TECO_TIMInit\n\r");
#endif	
	
	/* Compute the prescaler value */
	
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 1000;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	/* PWM1 Mode configuration: Channel 2*/
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC2Init(TIM3, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	
	/* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);
	
}

/**
  * @brief  TECO duty cycle control
  * @param  None
  * @retval None
  */
void _dutyCycle(__IO uint16_t CCR){	
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM1 Mode configuration: Channel 2*/
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC2Init(TIM3, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	
	/* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);
}




/**
  * @brief  TECO_GPIO initial with GPIO
	* @param  0: Night, 1: Day
  * @retval None
  */
void TECOAirControl(uint8_t Ins){
	switch(Ins){
		case 0:
			GPIO_LOW(GPIOB, GPIO_Pin_5);
		break;
		
		case 1:
			GPIO_HIGH(GPIOB, GPIO_Pin_5);
		break;
	}
}

void CoolingControl(uint8_t Ins){
		switch(Ins){
		case 0:
			if( DegC > 17.5){
				GPIO_HIGH(GPIOB, GPIO_Pin_5);
			}
			else {
				GPIO_LOW(GPIOB, GPIO_Pin_5);
			}
		break;
		
		case 1:
			if( DegC > 23.5){
				GPIO_HIGH(GPIOB, GPIO_Pin_5);
			}
			else {
				GPIO_LOW(GPIOB, GPIO_Pin_5);
			}
			
		break;
	}
}

void TECO_test(uint8_t Ins){
	switch(Ins){
		case 0:
			if( DegC > 17.5){
				GPIO_LOW(GPIOB, GPIO_Pin_7);

			}
			else {
				GPIO_HIGH(GPIOB, GPIO_Pin_7);
			}
		break;
		
		case 1:
			if( DegC > 23.5){
				GPIO_LOW(GPIOB, GPIO_Pin_7);
			}
			else {
				GPIO_HIGH(GPIOB, GPIO_Pin_7);
			}
			
		break;
	}
	
}
/******************* KINPO Electronic RDC  END OF FILE *******************/
