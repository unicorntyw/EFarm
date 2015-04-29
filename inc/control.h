#include "main.h"

void RunMainPage(void);
void RunNormal(void);
void RunSetPlantMode(void);
void RunSetTime(void);
void RunSetTemperature(void);
void RunSetBrightness(void);
void RunSetCO2(void);
void RunShowVer(void);
void RunDebug1(void);
void RunIdle(void);
void GoTo(eFarm_screen_state aState);

extern void (*stateFunc)(void);
extern eFarm_screen_state ScreenState;
extern eFarm_plant_mode PlantMode;
extern uint8_t curBrightLevel, savBrightLevel;
extern uint8_t NFCOn, CO2On;
