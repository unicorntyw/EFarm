#include "main.h"

GPIO_InitTypeDef EXTIGPIO_InitStructure;		// Declare EXTI GPIO Structure 
EXTI_InitTypeDef EXTI_InitStructure;				// EXTI, External Interrupt controller
NVIC_InitTypeDef NVIC_InitStructure;

void EXTI4_15config(void);

void BtnGpioExtiInit(void){
	EXTI4_15config();
}

void EXTI4_15config(void){
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	EXTIGPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12; 
	EXTIGPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	EXTIGPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN ;
	//EXTIGPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP ;
	EXTIGPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
	EXTIGPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &EXTIGPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource8);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource9);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource10);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource11);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource12);

	EXTI_InitStructure.EXTI_Line = EXTI_Line8 | EXTI_Line9 | EXTI_Line10 | EXTI_Line11 | EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//  EXTI_Trigger_Rising_Falling; EXTI_Trigger_Falling
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
