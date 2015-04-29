/**
  ******************************************************************************
  * @file    Project/STM32L1xx_StdPeriph_Templates/stm32l1xx_it.c 
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    16-May-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx_it.h"
#include "main.h"
#include "TimeDelay.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t btn; /* Debounce Parameter*/

extern uint8_t menuFlag;		// For 1st layer flag
extern uint8_t switch_flag; // For 1st layer flag
extern uint8_t plus_flag;		// For 1st layer flag
extern uint8_t minus_flag;	// For 1st layer flag
extern uint8_t plus_flag2;	// For 2nd layer flag
extern uint8_t minus_flag2; // For 2nd layer flag

extern uint8_t flag;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
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
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
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
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
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

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void){
	TimingDelay_Decrement();	
}

void RTC_Alarm_IRQnHandler(void){
	printf("test\n\r");

	if(RTC_GetITStatus(RTC_IT_ALRA) != RESET){
		RTC_ClearITPendingBit(RTC_IT_ALRA);
		flag =1 ;
	}
	EXTI_ClearITPendingBit(EXTI_Line17);

}

void EXTI0_IRQHandler(void) {
#ifdef DEBUG
	printf("_____EXTI0_IRQHandler____\r\n");
#endif
	disableGlobalInterrupts();
	
	if(EXTI_GetITStatus(EXTI_Line0)!= RESET){
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 1){
			//SetupButtonHandler(); // excute function
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line0);
	enableGlobalInterrupts();
	
	/*
	while((EXTI_GetITStatus(EXTI_Line0)!=RESET & GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)) == 1){
		SetupButtonHandler(); // excute function
		EXTI_ClearITPendingBit(EXTI_Line0);
		enableGlobalInterrupts();
	}
	*/
}

void EXTI9_5_IRQHandler(void) {
	
	
	disableGlobalInterrupts();
	
	if(EXTI_GetITStatus(EXTI_Line8) != RESET){
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)==0){	
			if(btn){
				btn = 0;
				GPIO_ResetBits(GPIOA, GPIO_Pin_8);

				/* Add confirm status in below*/
				minus_flag = 1;
				minus_flag2 = 1;
			}
		
			else{
				btn++;
				GPIO_ResetBits(GPIOA, GPIO_Pin_8);
			}
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line8);
	
	if(EXTI_GetITStatus(EXTI_Line9) != RESET){
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_9)==0){	
			if(btn){
				btn = 0;
				GPIO_ResetBits(GPIOA, GPIO_Pin_9);
				
				/* Add confirm status in below*/
				plus_flag = 1;
				plus_flag2 = 1;
			}
			else{
				btn++;
				GPIO_ResetBits(GPIOA, GPIO_Pin_9);
			}
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line9);
	enableGlobalInterrupts();	
	
	/*
	if(EXTI_GetITStatus(EXTI_Line9) != RESET){
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_9) == 0){
			plus_flag = 1;			
			plus_minus_flag = 1;
		}
	} 
	*/

}

void EXTI15_10_IRQHandler(void){
	disableGlobalInterrupts();
	
	if(EXTI_GetITStatus(EXTI_Line10) != RESET){
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10)==0){	
			if(btn){
				btn = 0;
				GPIO_ResetBits(GPIOA, GPIO_Pin_10);
				
				/* Add confirm status in below*/
				switch_flag = 1;
			}
			else{
				btn++;
				GPIO_ResetBits(GPIOA, GPIO_Pin_10);
			}
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line10);

	
	if(EXTI_GetITStatus(EXTI_Line11) != RESET){
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11)==0){	
			if(btn){
				btn = 0;
				GPIO_ResetBits(GPIOA, GPIO_Pin_11);
				
				/* Add confirm status in below*/
				menuFlag = 1;
			}
			else{
				btn++;
				GPIO_ResetBits(GPIOA, GPIO_Pin_11);
			}
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line11);
	enableGlobalInterrupts();	
	
	
	
	
	/*
	if(EXTI_GetITStatus(EXTI_Line10) != RESET){
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10) == 0){
			switchbtn_flag = 1;
		}
	}
	
	
	if(EXTI_GetITStatus(EXTI_Line11) != RESET){
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11) == 0){
				update_flag = 1;
				info_flag = 0;
		}
	} 
*/
	


}
/******************************************************************************/
/*                 STM32L1xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32l1xx_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/
/******************* KINPO Electronic RDC  END OF FILE *******************/
