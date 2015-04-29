#include "main.h"

/**
  * @brief 	
  * @param  None
  * @retval None
  */
void CoolHeatInit(void){

	//PA0 PC5 PB0 PB1 : PB2 PC4 PA7 PC13 : PB4 PB3
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2; //| GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
}

void FanInit(){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void StartOnlyFan(void){
	GPIO_HIGH(GPIOB,GPIO_Pin_8);
}

void StopOnlyFan(void){
	GPIO_LOW(GPIOB,GPIO_Pin_8);
}

void StartFan(void){
	GPIO_HIGH(GPIOB,GPIO_Pin_3);
	GPIO_HIGH(GPIOB,GPIO_Pin_4);
	GPIO_HIGH(GPIOB,GPIO_Pin_8);
}

void StopFan(void){
	GPIO_LOW(GPIOB,GPIO_Pin_3);
	GPIO_LOW(GPIOB,GPIO_Pin_4);
	GPIO_LOW(GPIOB,GPIO_Pin_8);
}

void StartCooling(void){
	
	GPIO_LOW(GPIOA,GPIO_Pin_0);
	GPIO_HIGH(GPIOC,GPIO_Pin_5);
	GPIO_LOW(GPIOB,GPIO_Pin_0);
	GPIO_HIGH(GPIOB,GPIO_Pin_1);
	
	GPIO_LOW(GPIOB,GPIO_Pin_2);
	GPIO_HIGH(GPIOC,GPIO_Pin_4);
	GPIO_LOW(GPIOA,GPIO_Pin_7);
	GPIO_HIGH(GPIOC,GPIO_Pin_13);
	
}

void StopCooling(void){
	
	GPIO_LOW(GPIOA,GPIO_Pin_0);
	GPIO_LOW(GPIOC,GPIO_Pin_5);
	GPIO_LOW(GPIOB,GPIO_Pin_0);
	GPIO_LOW(GPIOB,GPIO_Pin_1);
	
	GPIO_LOW(GPIOB,GPIO_Pin_2);
	GPIO_LOW(GPIOC,GPIO_Pin_4);
	GPIO_LOW(GPIOA,GPIO_Pin_7);
	GPIO_LOW(GPIOC,GPIO_Pin_13);
	
}

void StartHeating(void){
	
	GPIO_HIGH(GPIOA,GPIO_Pin_0);
	GPIO_LOW(GPIOC,GPIO_Pin_5);
	GPIO_HIGH(GPIOB,GPIO_Pin_0);
	GPIO_LOW(GPIOB,GPIO_Pin_1);
	
	GPIO_HIGH(GPIOB,GPIO_Pin_2);
	GPIO_LOW(GPIOC,GPIO_Pin_4);
	GPIO_HIGH(GPIOA,GPIO_Pin_7);
	GPIO_LOW(GPIOC,GPIO_Pin_13);
	
}

void StopHeating(void){
	
	GPIO_LOW(GPIOA,GPIO_Pin_0);
	GPIO_LOW(GPIOC,GPIO_Pin_5);
	GPIO_LOW(GPIOB,GPIO_Pin_0);
	GPIO_LOW(GPIOB,GPIO_Pin_1);
	
	GPIO_LOW(GPIOB,GPIO_Pin_2);
	GPIO_LOW(GPIOC,GPIO_Pin_4);
	GPIO_LOW(GPIOA,GPIO_Pin_7);
	GPIO_LOW(GPIOC,GPIO_Pin_13);
	
}
