#include "main.h"

#define MINUS_DOWN	(GPIOA->IDR & Bit9)
#define PLUS_DOWN   (GPIOA->IDR & Bit8)
#define SWITCH_DOWN (GPIOA->IDR & Bit10)
#define SETUP_DOWN  (GPIOA->IDR & Bit11)
#define POWER_DOWN  (GPIOA->IDR & Bit12)
#define DBTICKS 80

typedef struct
{
  bool pressed;
	uint64_t DownTicks;
	Key_Event event;
}KeyInfor;

KeyInfor MinsKey	={FALSE, 0, minus_key_pressed};
KeyInfor PlusKey	={FALSE, 0, plus_key_pressed};
KeyInfor SwitchKey={FALSE, 0, switch_key_pressed};
KeyInfor SetUpKey	={FALSE, 0, setup_key_pressed};
KeyInfor PowerKey	={FALSE, 0, power_key_pressed};

void InitKey(void)
{	
	GPIO_InitTypeDef EXTIGPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	EXTIGPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12; 
	EXTIGPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	EXTIGPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN ;
	//EXTIGPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP ;
	EXTIGPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
	EXTIGPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &EXTIGPIO_InitStructure);
}

static uint64_t diffTicks=0;
void CheckActiveKey(bool pressed, KeyInfor* key, uint64_t SystemTicks)
{
	uint64_t thisTicks = SystemTicks;
	
	if(pressed || key->pressed)
	{
		if(!key->pressed)
		{
			key->pressed=TRUE;
			key->DownTicks=thisTicks;
		}
		else if (!pressed)
		{
			key->pressed=FALSE;
		}
		else 
		{	
			if(thisTicks > key->DownTicks)
				diffTicks=thisTicks-key->DownTicks;
			else
				diffTicks=0xffffffffffffffff-key->DownTicks+thisTicks;
			
			if( diffTicks % DBTICKS ==0 )
				AddEvent(key->event);
		}
	}
}

void CheckNobleKey(bool pressed, KeyInfor* key)
{
	if(pressed != key->pressed)
	{
		if(pressed)
		{
			key->pressed=TRUE;
			AddEvent(key->event);
		}
		else if (!pressed)
		{
			key->pressed=FALSE;
		}
	}
}

void WatchKeys(uint64_t SystemTicks)
{
	if(MINUS_DOWN || MinsKey.pressed)
	{
		CheckActiveKey(MINUS_DOWN, &MinsKey, SystemTicks);
	}
	
	if(PLUS_DOWN || PlusKey.pressed)
	{
		CheckActiveKey(PLUS_DOWN, &PlusKey, SystemTicks);
	}
	
	if(SWITCH_DOWN != SwitchKey.pressed)
	{
		CheckNobleKey(SWITCH_DOWN, &SwitchKey);
	}
	
	if(SETUP_DOWN != SetUpKey.pressed)
	{
		CheckNobleKey(SETUP_DOWN, &SetUpKey);
	}
	
	if(POWER_DOWN != PowerKey.pressed)
	{
		CheckNobleKey(POWER_DOWN, &PowerKey);
	}
}
