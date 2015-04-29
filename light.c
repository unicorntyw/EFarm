#include "main.h"
#include "control.h"

uint16_t TimerPeriod, P100Pulse, P90Pulse, P80Pulse, P75Pulse, P60Pulse, P50Pulse, P45Pulse,P40Pulse, P35Pulse, P30Pulse, P25Pulse, P20Pulse, P15Pulse, P10Pulse, P05Pulse;

void LightGPIOInit(void){	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_2);
	
}

void PulseInit(){
	/* Compute the value to be set in ARR regiter to generate signal frequency at 17.57 Khz */
  TimerPeriod = (SystemCoreClock / 17570 ) - 1;
	
	/* Compute CCR2 value to generate a duty cycle at 100% */
  P100Pulse = (uint16_t) (((uint32_t) 100 * (TimerPeriod - 1)) / 100);
	P90Pulse = (uint16_t) (((uint32_t) 90 * (TimerPeriod - 1)) / 100);
	P80Pulse = (uint16_t) (((uint32_t) 80 * (TimerPeriod - 1)) / 100);
	/* Compute CCR2 value to generate a duty cycle at 75% */
  P75Pulse = (uint16_t) (((uint32_t) 75 * (TimerPeriod - 1)) / 100);
	P60Pulse = (uint16_t) (((uint32_t) 60 * (TimerPeriod - 1)) / 100);
	/* Compute CCR1 value to generate a duty cycle at 50% */
	P50Pulse = (uint16_t) (((uint32_t) 50 * (TimerPeriod - 1)) / 100);
  P45Pulse = (uint16_t) (((uint32_t) 45 * (TimerPeriod - 1)) / 100);
  P40Pulse = (uint16_t) (((uint32_t) 40 * (TimerPeriod - 1)) / 100);
  P35Pulse = (uint16_t) (((uint32_t) 35 * (TimerPeriod - 1)) / 100);
  P30Pulse = (uint16_t) (((uint32_t) 30 * (TimerPeriod - 1)) / 100);
  /* Compute CCR3 value to generate a duty cycle at 25%  */
  P25Pulse = (uint16_t) (((uint32_t) 25 * (TimerPeriod - 1)) / 100);
  P20Pulse = (uint16_t) (((uint32_t) 20 * (TimerPeriod - 1)) / 100);
	/* Compute CCR3 value to generate a duty cycle at 15%  */
  P15Pulse = (uint16_t) (((uint32_t) 15 * (TimerPeriod - 1)) / 100);
  P10Pulse = (uint16_t) (((uint32_t) 10 * (TimerPeriod - 1)) / 100);
	/* Compute CCR3 value to generate a duty cycle at 5%  */
  P05Pulse = (uint16_t) (((uint32_t) 5 * (TimerPeriod - 1)) / 100);
}

void LightPWMInit(void){
	PulseInit();
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);
	
	/* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM16, &TIM_TimeBaseStructure);
	
	/* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_Pulse = P05Pulse;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

  TIM_OC1Init(TIM16, &TIM_OCInitStructure);

  //TIM_OC1PreloadConfig(TIM16, TIM_OCPreload_Enable);
  //TIM_ARRPreloadConfig(TIM16, ENABLE);

  TIM_Cmd(TIM16, ENABLE);
	TIM_CtrlPWMOutputs(TIM16, ENABLE);
}

void LightInit(){
	LightGPIOInit();
	LightPWMInit();
}
void SetDutyCycle(uint16_t duty){
	TIM16->CCR1=duty;
}

void AdjustLEDLevel(uint8_t level){
	curBrightLevel=level;
	switch(level){
		case 0:
			//MainLedOff();
			SetDutyCycle(0);
		break;
		case 1:
			SetDutyCycle(P05Pulse);
		break;
		case 2:
			SetDutyCycle(P20Pulse);
		break;
		case 3:
			//MainLedOn();
			SetDutyCycle(P40Pulse);
		break;
		case 4:
			//MainLedOn();
			SetDutyCycle(P80Pulse);
		break;
		default:
			SetDutyCycle(P80Pulse);
		break;
	}
}

void LEDOnOff(uint8_t Ins){
	switch(Ins){
		case 0:
			SetDutyCycle(0);
		break;
		
		case 1:
			SetDutyCycle(750);
		break;
		
		default:
			break;
	}
	
}

void MainLedOn(){
	GPIOB->BSRR=Bit6;
	//GPIO_WriteBit(GPIOB,GPIO_Pin_6,Bit_SET);
}

void MainLedOff(){
	GPIOB->BRR=Bit6;
	//GPIO_WriteBit(GPIOB,GPIO_Pin_6,Bit_RESET);
}
