#include "main.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay; 


/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in seconds, milliseconds, microseconds.
  * @retval None
	*/
 
// Delay function is use System interrupt function to implement.
void Delay(__IO uint32_t nTime){
	if (SysTick_Config(SystemCoreClock)){ 
			while (1);    
	}
	TimingDelay = nTime;
	while(TimingDelay != 0);
}

void msDelay(uint32_t mTime){
	/*
	if (SysTick_Config(SystemCoreClock/1000)){ 
			while (1);    
	}*/
	TimingDelay = mTime;
	while(TimingDelay);
	/*
	uint32_t delay = (mTime * (SystemCoreClock/1000))>>2;
	while(--delay);*/
	
}


// If follow Delay() & msDelay(), Interrupted too many times, cause the CPU tasks choke.
// So nsDelay() use __NOP(), but it has some error.
void nsDelay(__IO uint32_t nTime){
	// Interrupted too many times, cause the CPU tasks choke.
	uint32_t i;
	/*
	while(nTime !=0){
		__NOP();
		nTime--;
	}
	*/
	for( i=0; i<nTime; i++){
		//asm("nop");
		__NOP();__NOP();__NOP();__NOP();__NOP();
	}
	
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void){
	if(TimingDelay != 0x00){
		TimingDelay--;
	}
}


/******************* KINPO Electronic RDC  END OF FILE *******************/
