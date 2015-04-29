#include "stm32f0xx_it.h"
#include "main.h"
#include "queue.h"
#include "string.h"
#include "keyWatcher.h"

#define DBTICKS 150
#define PWR_DBTICKS 150
#define enableGlobalInterrupts()   __set_PRIMASK(0);
#define disableGlobalInterrupts()  __set_PRIMASK(1);

void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
	printf("Hard Fault Happened!!\n\r"); 
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}


static volatile uint64_t SystemTicks=0;

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  SystemTicks++;
	WatchKeys(SystemTicks);
}

void RTC_IRQHandler(void){
#ifdef RTC_DEBUG
	printf("RTC Alarm Triggered!!\n\r");
#endif
	if(RTC_GetITStatus(RTC_IT_ALRA) != RESET){
		RTC_ClearITPendingBit(RTC_IT_ALRA);
		RtcAlarmFlag=TRUE ;
	}
	EXTI_ClearITPendingBit(EXTI_Line17);

}

void USART1_IRQHandler(void){
	
	uint8_t in;
	if(USART_GetITStatus(USART1, USART_IT_RXNE))
  {	
    in = (USART_ReceiveData(USART1) & 0x7F );
  }

}

static volatile uint64_t SetUpTicks=0;
static volatile bool SetUpHit=FALSE;
static volatile uint64_t PlusTicks=0;
static volatile bool PlusHit=FALSE;
static volatile uint64_t MinusTicks=0;
static volatile bool MinusHit=FALSE;
static volatile uint64_t SwitchTicks=0;
static volatile bool SwitchHit=FALSE;
static volatile uint64_t PwrTicks=0;
static volatile bool PwrHit=FALSE;

static volatile uint64_t hitTicks=0;
#ifdef KEY_DEBUG
volatile uint8_t hitNum=0;
uint64_t ticks[100]={0};
#endif		
void EXTI4_15_IRQHandler(void){
	
	disableGlobalInterrupts();
	hitTicks = SystemTicks;
	
	if(EXTI_GetITStatus(EXTI_Line8)){
#ifdef KEY_DEBUG
		ticks[hitNum++]=hitTicks;
#endif
		if(PlusTicks != hitTicks)
		{
			//if( ((hitTicks-PlusTicks)>DBTICKS) && !PlusHit ){
			if(!PlusHit) {
				AddEvent(plus_key_pressed);
				PlusHit=TRUE;
			}
			else
				PlusHit=FALSE;
		
			PlusTicks=hitTicks;
		}
		EXTI_ClearITPendingBit(EXTI_Line8);		
	}
	
	if(EXTI_GetITStatus(EXTI_Line9)){
#ifdef KEY_DEBUG
		ticks[hitNum++]=hitTicks;	
#endif
		if(MinusTicks != hitTicks)
		{
			//if( ((hitTicks-MinusTicks)>DBTICKS) && !MinusHit ){
			if( !MinusHit ){
				AddEvent(minus_key_pressed);
				MinusHit=TRUE;
			}
			else
				MinusHit=FALSE;
			
			MinusTicks=hitTicks;
		}
		EXTI_ClearITPendingBit(EXTI_Line9);
	}
	
	if(EXTI_GetITStatus(EXTI_Line10)){
#ifdef KEY_DEBUG
		ticks[hitNum++]=hitTicks;
#endif
		if(SwitchTicks!=hitTicks){
			//if( ((hitTicks-SwitchTicks)>DBTICKS) && !SwitchHit ){
			if( !SwitchHit ){
				AddEvent(switch_key_pressed);
				SwitchHit=TRUE;
			}
			else
				SwitchHit=FALSE;
			
			SwitchTicks=hitTicks;
		}
		EXTI_ClearITPendingBit(EXTI_Line10);
	}
	
	if(EXTI_GetITStatus(EXTI_Line11)){
#ifdef KEY_DEBUG
			//hitNum=0;
			//memset(ticks,0,sizeof(ticks));
			ticks[hitNum++]=hitTicks;
#endif			
			//if( ((hitTicks-SetUpTicks)>DBTICKS) /*&& !SetUpHit*/){
				AddEvent(setup_key_pressed);
			//	SetUpHit=TRUE;
			//}
			//else
			//	SetUpHit=FALSE;
			
			//SetUpTicks=hitTicks;
			EXTI_ClearITPendingBit(EXTI_Line11);
	}
	
	if(EXTI_GetITStatus(EXTI_Line12)){
#ifdef KEY_DEBUG
			ticks[hitNum++]=hitTicks;
#endif		
			//if( ((hitTicks-PwrTicks)>DBTICKS) /*&& !PwrHit*/ ){
				AddEvent(power_key_pressed);
			//	PwrHit=TRUE;
			//}
			//else
			//	PwrHit=FALSE;
			
			//PwrTicks=hitTicks;
			EXTI_ClearITPendingBit(EXTI_Line12);
	}
	
	enableGlobalInterrupts();	
	
}

void msDelay(uint64_t mTime){
	uint64_t startTicks=SystemTicks;
	mTime<<=1;
	uint64_t targetTicks = startTicks + mTime;
	
	if(targetTicks > startTicks)
		while(SystemTicks < targetTicks);
	else 
	if(targetTicks < startTicks) //overflow
		while(SystemTicks > startTicks || SystemTicks < targetTicks);
	
}
