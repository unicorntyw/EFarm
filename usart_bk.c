
#include "main.h"
#include <string.h>

// Before enter DEBUG mode, must set "Use MicroLIB"
// Setting Step: Option for Target -> Target -> Code Generation -> Use MicroLIB
#ifdef __GNUC__
	// With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
	//   set to 'Yes') calls __io_putchar() */
	#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
	#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif // __GNUC__ 

#define USART2_TX GPIO_Pin_2	// GPIO_PinSource2
#define USART2_RX GPIO_Pin_3	// GPIO_PinSource3
#define USART1_TX GPIO_Pin_6	// GPIO_PinSource6
#define USART1_RX GPIO_Pin_7	// GPIO_PinSource7

#define undateNum (SystemCoreClock/200)
	
//USART_TypeDef* DebugUSART=0;

uint8_t NFC_Init_Cmd[]  ={0x00, 0x00, 0xFF, 0x11, 0xEF, 0xD4, 0x55, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x03, 0xFD, 0xD4, 0x14, 0x01, 0x17, 0x00, 0x83, 0x00};
uint8_t Hello[]  ={'H', 'e', 'l', 'l', '0','\n','\r'};
uint8_t NFC_Init_Rsp[]  ={0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x02, 0xFE, 0xD5, 0x15, 0x16, 0x00};
uint8_t NFC_Check_Cmd[] ={0x00, 0x00, 0xFF, 0x04, 0xFC, 0xD4, 0x4A, 0x01, 0xE1, 0x00};
uint8_t NFC_Check_Rsp[] ={0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x0C, 0xF4, 0xD5, 0x4B, 0x01, 0x01, 0x00, 0x04, 0x08, 0x04, 0x2C, 0x7F, 0xAB, 0xA2, 0xD6, 0x00};
uint8_t buf[160];
int num=0;

//void NFC_Send(uint8_t* cmd, uint32_t num);
	
void Usart2Init(void){
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin 	= USART2_TX | USART2_RX;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* GPIO_PinAFConfig must follow "GPIO_Pin" & "Alternate function" to define */
	// This case used "USART2"
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);
		
	/* UART settings, include BaudRate, WrodLength, StopBits, Parity, HW Flower Control */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART2, &USART_InitStructure); 
//	USART_MSBFirstCmd(USART2, DISABLE);
//	USART_OverSampling8Cmd(USART2, ENABLE);
	USART_Cmd(USART2, ENABLE);
	/*
	NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = 28;//USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);*/
	//NFC_Send("Hello from USART2\n\r", sizeof("Hello from USART2\n\r"));
	
}

void Usart1Init(void){
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin 	= USART1_TX | USART1_RX;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_0);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_0);
		
	/* UART settings, include BaudRate, WrodLength, StopBits, Parity, HW Flower Control */
	USART_InitStructure.USART_BaudRate = 115200; // 115200, 9600
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);
	USART_MSBFirstCmd(USART1, DISABLE);
	USART_Cmd(USART1, ENABLE);
	/*
	NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	*/
	printf("Hello from USART1.\n\r");
	//DebugUSART=USART1;
	
}

void NFC_Send(uint8_t* cmd, uint32_t num){

//	while(!USART_GetFlagStatus(USART2,USART_FLAG_BUSY));

#ifdef NFC_DEBUG
		printf("NFC send ");
#endif	

	for(int idx=0; idx < num; idx++){
		while(!USART_GetFlagStatus(USART2, USART_FLAG_TXE));
		USART_SendData(USART2, cmd[idx]);
		//while(!USART_GetFlagStatus(USART2, USART_FLAG_TC));

#ifdef NFC_DEBUG
		printf(" %02x",cmd[idx]);
		if(idx==9)
			printf("\n\r");
#endif

	}
	while(!USART_GetFlagStatus(USART2, USART_FLAG_TC)){};

	#ifdef NFC_DEBUG
		printf(" end.\n\r");
#endif
}

uint32_t NFC_Receive(uint8_t* buff){
	uint32_t timeOut = undateNum*5;
	
//	while(!USART_GetFlagStatus(USART2,USART_FLAG_BUSY));
	
	while(TRUE){
		while(!USART_GetFlagStatus(USART2, USART_FLAG_RXNE)){
			if( !(--timeOut) ) return num;
		}
		//buff[num]=USART_ReceiveData(USART2) & 0xFF;
		//printf("%02x ",buff[num]);
		//uint8_t in = USART_ReceiveData(USART2) & 0xFF;
		//printf("%c ",in);
		buf[num] = USART_ReceiveData(USART2);
		num++;
	}	
}

bool NFC_Init(){
	//Usart2Init();
	//NFC_Send(Hello, sizeof(Hello));
	
	NFC_Send(NFC_Init_Cmd, sizeof(NFC_Init_Cmd));
	
	uint8_t size = sizeof(NFC_Init_Rsp)*2;
	uint8_t buff[size]; 
	uint8_t num = NFC_Receive(buff);

#ifdef NFC_DEBUG
	printf("NFC Init Rx Num=%02d\n\r",num);
	printf("NFC Init %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n\r",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7],buf[8],buf[9]);
	printf("NFC Init %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n\r",buf[10],buf[11],buf[12],buf[13],buf[14],buf[15],buf[16],buf[17],buf[18],buf[19]);
#endif

	return !memcmp(buff,NFC_Init_Rsp,size);
}

bool NFC_Check(){
	NFC_Send(NFC_Check_Cmd, sizeof(NFC_Check_Cmd));
	uint8_t size = sizeof(NFC_Init_Rsp);
	uint8_t buff[size];
	NFC_Receive(buff);
	return !memcmp(buff,NFC_Check_Rsp,19); 
}

PUTCHAR_PROTOTYPE{
	/* e.g. write a character to the USART */
	USART_SendData(USART1, (uint8_t) ch);
	/* Loop until the end of transmission */
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	return ch;
} 
