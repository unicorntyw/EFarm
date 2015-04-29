
#define bool _Bool
#define FALSE 0
#define TRUE !FALSE

#define GPIO_HIGH(a,b) 				a->BSRR = b
#define GPIO_LOW(a,b)					a->BRR  = b;
#define GPIO_TOGGLE(a,b) 			a->ODR ^= b 

typedef struct
{
  bool inited;
	uint8_t mode; 
  uint16_t days; 
}StoredInfor;

typedef enum
{	
	main_page, 
	set_plant_mode, 
	set_time, 
	set_temperature, 
	set_brightness,
	show_ver,
	set_co2,
	debug1,
	debug2,
	idle
} eFarm_screen_state;

typedef enum{	
	Auto1=0,
	Auto2,
	Manual,
	UnSet
} eFarm_plant_mode;

#define Bit0 1
#define Bit1 (1 << 1)
#define Bit2 (1 << 2)
#define Bit3 (1 << 3)
#define Bit4 (1 << 4)
#define Bit5 (1 << 5)
#define Bit6 (1 << 6)
#define Bit7 (1 << 7)
#define Bit8 (1 << 8)
#define Bit9 (1 << 9)
#define Bit10 (1 << 10)
#define Bit11 (1 << 11)
#define Bit12 (1 << 12)
#define Bit13 (1 << 13)
#define Bit14 (1 << 14)
#define Bit15 (1 << 15)
#define Bit16 (1 << 16)
#define Bit17 (1 << 17)
#define Bit18 (1 << 18)
#define Bit19 (1 << 19)
#define Bit20 (1 << 20)
#define Bit21 (1 << 21)
#define Bit22 (1 << 22)
#define Bit23 (1 << 23)
#define Bit24 (1 << 24)
#define Bit25 (1 << 25)
#define Bit26 (1 << 26)
#define Bit27 (1 << 27)
#define Bit28 (1 << 28)
#define Bit29 (1 << 29)
#define Bit30 (1 << 30)
#define Bit31 (1 << 31)

#if defined(ENG_DEBUG_U2) || defined(ENG_DEBUG_U1) 
	#define ver "1.41 dbg"
#elif !defined(COOL_HEAT_ENABLE)
	#define ver "1.41 NoHC"
#else
	#define ver "1.41"
#endif
