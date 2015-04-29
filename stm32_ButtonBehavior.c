
#include "main.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern uint8_t itemcount;							// Item count, it calculate MinusKeyFlag & PlusKeyFlag
volatile uint8_t SetupMode;						// Page 1 ModeIns
extern int8_t clock_min;							// Page 2 Clock, Min.
extern int8_t clock_hour;							// Page 2 Clock, Hour
volatile uint8_t SetupTemperature;   	// Page 3 Target Temperature
extern uint8_t clockLighting_min;			// Page 4 Timer, Min.
extern uint8_t clockLighting_hour;		// Page 4 Timer, Hour
extern uint8_t Brightness;						// Page 5 Brightness Ins
extern uint8_t livingColor; 					// Page 6 LivingColorIns
extern uint8_t clockLivingColor_min;  // Page 7 Timer, Min.
extern uint8_t clockLivingColor_hour; // Page 7 Timer, Hour

volatile bool _layer2Flag;
volatile bool plus_flag2;
volatile bool minus_flag2;
extern bool Layer2BehaviorFlag;
extern bool cursorFlag;

volatile bool SetTimeFlag, SetAlarmFlag, SetAlarmFlag_LC;

char String4Behavior[16];
int loop;
int8_t tempClockMin, tempClockHour;						// Page2 get RTC timer parameter

/* Private functions ---------------------------------------------------------*/
void page1ButtonBehavior(void);
void page2ButtonBehavior(void);
void page3ButtonBehavior(void);
void page4ButtonBehavior(void);
void page5ButtonBehavior(void);
void page6ButtonBehavior(void);
//void page7ButtonBehavior(void);

/**
  * @brief 	Layer2Behavior Selector
  * @param  None
  * @retval None
  */
void Layer2Behavior(uint8_t Layer2BehaviorIns){
	if(Layer2BehaviorFlag==1){
		switch(Layer2BehaviorIns) {	
			case 0:
				break;
				
			case 1:
				page1ButtonBehavior();
				break;
			
			case 2:
				getCurrentRTCValue(&tempClockHour, &tempClockMin);
				SetTimeFlag = 1;
				page2ButtonBehavior();
				break;
			case 3:
				page3ButtonBehavior();
				break;
			
			case 4:
				SetAlarmFlag = 1;
				page4ButtonBehavior();
				break;
				
			case 5:
				page5ButtonBehavior();
				break;
			
			case 6:
				page6ButtonBehavior();
				break;
			
			case 7:
				SetAlarmFlag_LC = 1;
				//page7ButtonBehavior();
				break;
		}
	plus_flag2  = 0;
	minus_flag2 = 0;
	}
}

/* 1. Setup Mode behavior */
void page1ButtonBehavior(void){
	if(cursorFlag == 1){
		ShowPModeCursor(itemcount);
		
		/* Setting Parameter */
		SetupMode = 0;
		cursorFlag = 0;
	}
	else {
		if(plus_flag2 == 1){
			itemcount++;	
			itemcount = itemcount % 3;
	
			ShowPModeCursor(itemcount);
			
			/* Setting Parameter */
			SetupMode = itemcount;
		
		}
		else if(minus_flag2 == 1){
			itemcount--;
			itemcount = itemcount % 3;
			ShowPModeCursor(itemcount);		

			/* Setting Parameter */
			SetupMode = itemcount;
			
			if(itemcount == 0){
				itemcount = 3;
			}
		}
	}

}

/* 2. Setup Time behavior */
void page2ButtonBehavior(void){
	if(cursorFlag == 1){
		//clock_min = clock_hour = 0;
		clock_hour = tempClockHour;
		clock_min  = tempClockMin;
		
		for(loop=2; loop<5; loop++){
			cleanLCDLine(loop);
		}
		/* Print LCD Pattern */
		writeIns(0xC1);
		sprintf(String4Behavior, "Press +/- key");
		directWriteString((unsigned char *)String4Behavior); 
		
		writeIns(0x91);
		sprintf(String4Behavior, "Hour(-):Min.(+)");
		directWriteString((unsigned char *)String4Behavior); 
		
		timerSetupPattern(0xD5, clock_hour);
		writeIns(0xD8);
		writeData(':');
		timerSetupPattern(0xDA, clock_min);
		
		cursorFlag = 0;
	}
	else{
		if(plus_flag2 == 1){
			/* Setting Parameter */
			clock_min++;		
			if(clock_min > 59){
				clock_min = 0;
			}
			timerSetupPattern(0xDA, clock_min);

		}
		else if(minus_flag2 == 1){
			/* Setting Parameter */
			clock_hour++;

			if(clock_hour > 23){
				clock_hour = 0;
			} 
			
			/* LCD Page 3 Pattern */ 
			timerSetupPattern(0xD5, clock_hour);

		}
	}

}

/* 3. Setup Temp. behavior */
void page3ButtonBehavior(void){
	if(cursorFlag == 1){
		for(loop=2; loop<5; loop++){
			cleanLCDLine(loop);
		}
		
		SetupTemperature = 25;
		tempSetupPattern(SetupTemperature);
		
		
		writeIns(0xC1);
		sprintf(String4Behavior, "Press +/- key");
		directWriteString((unsigned char *)String4Behavior); 
		
		writeIns(0x94);
		sprintf(String4Behavior, "Temp.= ");
		directWriteString((unsigned char *)String4Behavior); 
			
		writeIns(0x9D);
		writeData(' ');
		writeIns(0x9E);
		writeData(0xDF);
		writeIns(0x9F);
		writeData('C');

		cursorFlag = 0;
	}
	else{
		if(plus_flag2 == 1){
			/* Setting Parameter */
			SetupTemperature++;		
			if(SetupTemperature > 25){
				SetupTemperature = 25;
			}
			tempSetupPattern(SetupTemperature);
		}
		else if(minus_flag2 == 1){
			/* Setting Parameter */
			SetupTemperature--;

			if(SetupTemperature < 15){
				SetupTemperature = 15;
			} 
			/* LCD Page 3 Pattern */ 
			tempSetupPattern(SetupTemperature);
		}
	}
}

/* 4. Lighting Timing behavior */
void page4ButtonBehavior(void){
	if(cursorFlag == 1){
		clockLighting_min = clockLighting_hour = 0;
		
		for(loop=2; loop<5; loop++){
			cleanLCDLine(loop);
		}
		writeIns(0xC1);
		sprintf(String4Behavior, "Press +/- key");
		directWriteString((unsigned char *)String4Behavior); 
		
		writeIns(0x91);
		sprintf(String4Behavior, "Hour(-):Min.(+)");
		directWriteString((unsigned char *)String4Behavior); 
		
		lightingTimerSetupPattern(0xD5, clockLighting_hour);
		writeIns(0xD8);
		writeData(':');
		lightingTimerSetupPattern(0xDA, clockLighting_min);
		
		cursorFlag = 0;
	}
	else{
		if(plus_flag2 == 1){
			/* Setting Parameter */
			clockLighting_min = clockLighting_min + 5;		
			if(clockLighting_min > 59){
				clockLighting_min = 0;
			}
			/* LCD Page 4 Pattern */ 
			lightingTimerSetupPattern(0xDA, clockLighting_min);
		}
		else if(minus_flag2 == 1){
			/* Setting Parameter */
			clockLighting_hour++;
			if(clockLighting_hour > 23){
				clockLighting_hour = 0;
			} 
			/* LCD Page 4 Pattern */ 
			lightingTimerSetupPattern(0xD5, clockLighting_hour);
		}
	}
	
}


/* 5. Brightness behavior */
void page5ButtonBehavior(void){
	if(cursorFlag == 1){

		for(loop=2; loop<5; loop++){
			cleanLCDLine(loop);
		}
		writeIns(0xC1);
		sprintf(String4Behavior, "Press +/- key");
		directWriteString((unsigned char *)String4Behavior); 
		
		writeIns(0xD0);
		sprintf(String4Behavior, "Low");
		directWriteString((unsigned char *)String4Behavior); 
		
		writeIns(0xDC);
		sprintf(String4Behavior, "High");
		directWriteString((unsigned char *)String4Behavior); 
		
		/* Setting Parameter */
		Brightness = 4;
		itemcount = Brightness;
		brightnessSetupPattern(Brightness);
		
		cursorFlag = 0;
	}
	else {
		if(plus_flag2 == 1){
			itemcount++;	
			if(itemcount > 4){
				itemcount = 4;
			}
			itemcount = itemcount % 5;
			
			/* Setting Parameter */
			Brightness = itemcount;
			brightnessSetupPattern(Brightness);		// Call stm32_Plant_lcdPattern
			AdjustLEDLevel(Brightness); 					// Call stm32_LedBrightness
		
		}
		else if(minus_flag2 == 1){
			itemcount--;
			if(itemcount == 0){
				itemcount  = 1;
			}
			itemcount = itemcount % 5;


			/* Setting Parameter */
			Brightness = itemcount;
			brightnessSetupPattern(Brightness);		// Call stm32_Plant_lcdPattern
			AdjustLEDLevel(Brightness); 					// Call stm32_LedBrightness
			
			/*
			if(itemcount == 0){
				itemcount = 3;
			}
			*/
		}
	}
}

/* 6. Living Color behavior */
void page6ButtonBehavior(void){
	if(cursorFlag == 1){
		ShowPModeCursor(itemcount);
		
		/* Setting Parameter */
		livingColor = 0;
		cursorFlag = 0;
	}
	else {
		if(plus_flag2 == 1){
			itemcount++;	
			itemcount = itemcount % 3;
			ShowPModeCursor(itemcount);
			
			/* Setting Parameter */
			livingColor = itemcount;
		
		}
		else if(minus_flag2 == 1){
			itemcount--;
			itemcount = itemcount % 3;
			ShowPModeCursor(itemcount);		

			/* Setting Parameter */
			livingColor = itemcount;
			
			if(itemcount == 0){
				itemcount = 3;
			}
		}
	}
}
/* 7. Living Alarm behavior*/
/*
void page7ButtonBehavior(void){
	if(cursorFlag == 1){
		clockLivingColor_min = clockLivingColor_hour = 0;
		
		for(loop=2; loop<5; loop++){
			cleanLCDLine(loop);
		}
		writeIns(0xC1);
		sprintf(String4Behavior, "Press +/- key");
		directWriteString((unsigned char *)String4Behavior); 
		
		writeIns(0x91);
		sprintf(String4Behavior, "Hour(-):Min.(+)");
		directWriteString((unsigned char *)String4Behavior); 
		
		
		livingColorTimerSetupPattern(0xD5, clockLivingColor_hour);
		writeIns(0xD8);
		writeData(':');
		livingColorTimerSetupPattern(0xDA, clockLivingColor_min);
		
		cursorFlag = 0;
	}
	else{
		if(plus_flag2 == 1){
			
			clockLivingColor_min = clockLivingColor_min + 5;		
			if(clockLivingColor_min > 59){
				clockLivingColor_min = 0;
			}
			 
			livingColorTimerSetupPattern(0xDA, clockLivingColor_min);

		}
		else if(minus_flag2 == 1){
			
			clockLivingColor_hour++;
			if(clockLivingColor_hour > 23){
				clockLivingColor_hour = 0;
			} 
			 
			livingColorTimerSetupPattern(0xD5, clockLivingColor_hour);

		}
	}
}
*/
/******************* KINPO Electronic RDC  END OF FILE *******************/
