
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

#define TIME_OUT_TICKS (SystemCoreClock/200)
#define BUFF_SIZE 30	
USART_TypeDef* DebugUSART=0;

uint8_t Hello[]  ={'H', 'e', 'l', 'l', '0','\n','\r'};

uint8_t NFC_Init_Cmd[]  ={0x00, 0x00, 0xFF, 0x11, 0xEF, 0xD4, 0x55, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x03, 0xFD, 0xD4, 0x14, 0x01, 0x17, 0x00, 0x83, 0x00};
//uint8_t NFC_Init_Rsp[]  ={0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x02, 0xFE, 0xD5, 0x15, 0x16, 0x00};
uint8_t NFC_Init_Rsp[]  ={0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x01, 0xFF, 0x7F, 0x81, 0x00};
uint8_t NFC_Check_Cmd[] ={0x00, 0x00, 0xFF, 0x04, 0xFC, 0xD4, 0x4A, 0x01, 0x00, 0xE1, 0x00};
uint8_t NFC_Check_Rsp[] ={0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x0C, 0xF4, 0xD5, 0x4B, 0x01, 0x01, 0x00, 0x04, 0x08, 0x04, 0x2C, 0x7F, 0xAB, 0xA2, 0xD6, 0x00};

uint8_t buff[BUFF_SIZE]={0};
uint8_t RxNum=0;
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
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);
	
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART2, &USART_InitStructure); 
	USART_MSBFirstCmd(USART2, DISABLE);
	USART_Cmd(USART2, ENABLE);

#ifdef ENG_DEBUG_U2
	DebugUSART=USART2;
	printf("Hello from USART2.\n\r");
#endif
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
	DebugUSART=USART1;
	printf("Hello from USART1.\n\r");
}

void SetUpConn(){
	
	uint8_t ch;	
	while( USART_GetFlagStatus(USART2, USART_FLAG_RXNE) )
			ch=USART_ReceiveData(USART2);
	
	if(USART_GetFlagStatus(USART2, USART_ISR_ORE))
		USART_ClearFlag(USART2, USART_FLAG_ORE);
	
	memset(buff,0,BUFF_SIZE);
	RxNum=0;
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
	while(!USART_GetFlagStatus(USART2, USART_FLAG_TC));

	#ifdef NFC_DEBUG
		printf(" end.\n\r");
#endif
}

uint32_t NFC_Receive(){

	uint32_t timeOut = TIME_OUT_TICKS;
	uint8_t tempCh;
//	while(!USART_GetFlagStatus(USART2,USART_FLAG_BUSY));
	while(TRUE){
		while(!USART_GetFlagStatus(USART2, USART_FLAG_RXNE)){
			if( !(--timeOut) ) return RxNum;
		}
		tempCh=USART_ReceiveData(USART2);
		if(tempCh!='\r' && tempCh!='\n')
			buff[RxNum++]=tempCh;
	}
}

bool NFC_Init(){
	//Usart2Init();
	SetUpConn();
	NFC_Send(NFC_Init_Cmd, sizeof(NFC_Init_Cmd));
	uint8_t num = NFC_Receive();

#ifdef NFC_DEBUG
	printf("NFC Init Rx Num=%02d\n\r",num);
	printf("NFC Init %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n\r",buff[0],buff[1],buff[2],buff[3],buff[4],buff[5],buff[6],buff[7],buff[8],buff[9]);
	printf("NFC Init %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n\r",buff[10],buff[11],buff[12],buff[13],buff[14],buff[15],buff[16],buff[17],buff[18],buff[19]);
#endif

	return !memcmp(buff, NFC_Init_Rsp, 9);
}

bool NFC_Check(){
	SetUpConn();
	NFC_Send(NFC_Check_Cmd, sizeof(NFC_Check_Cmd));
	NFC_Receive();
	return !memcmp(buff,NFC_Check_Rsp,19); 
}

PUTCHAR_PROTOTYPE{
	/* e.g. write a character to the USART */
	if(!DebugUSART) return ch;
	USART_SendData(DebugUSART, (uint8_t) ch);
	/* Loop until the end of transmission */
	while(USART_GetFlagStatus(DebugUSART, USART_FLAG_TC) == RESET);
	return ch;
} 
