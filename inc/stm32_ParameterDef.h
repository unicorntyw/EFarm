 /**
  ******************************************************************************
  * @file    discover_board.h
  * @author  Microcontroller Division
  * @version V1.0.3
  * @date    May-2013
  * @brief   Input/Output defines
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __DISCOVER_BOARD_H
#define __DISCOVER_BOARD_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"  

/* Timer define */
#define DELAY Delay(150)
#define TEMPO if(!KeyPressed) DELAY



#define bool _Bool
#define FALSE 0
#define TRUE !FALSE


/* MACROs for SET, RESET or TOGGLE Output port */

#define GPIO_HIGH(a,b) 				a->BSRR = b
#define GPIO_LOW(a,b)					a->BRR  = b;
#define GPIO_TOGGLE(a,b) 			a->ODR ^= b 

#define USERBUTTON_GPIO_PORT	GPIOA
#define USERBUTTON_GPIO_PIN   GPIO_Pin_0
#define USERBUTTON_GPIO_CLK   RCC_AHBPeriph_GPIOA

#define LD_GPIO_PORT 					GPIOB
#define LD_GREEN_GPIO_PIN 		GPIO_Pin_7
#define LD_GPIO_PORT_CLK      RCC_AHBPeriph_GPIOB

#define LCD_GPIO_PORT GPIOC
#define LCD_GPIO_PORT_CLK RCC_AHBPeriph_GPIOC

// Interrupt Event declare
#define STATE_0		0
#define STATE_1 	1
#define STATE_2 	2
#define STATE_3		3
#define STATE_4		4
#define STATE_5		5
#define STATE_6		6
#define STATE_7		7
#define MAX_STATE	8	 // function lay

#define enableGlobalInterrupts()   __set_PRIMASK(0);
#define disableGlobalInterrupts()  __set_PRIMASK(1);


// USART Pin declare
//#define USART_TX GPIO_Pin_2	// GPIO_PinSource2
//#define USART_RX GPIO_Pin_3	// GPIO_PinSource3

#endif


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
