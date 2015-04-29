
#include "main.h"
#include <string.h>

#define I2C_SCL GPIO_Pin_10
#define I2C_SDA GPIO_Pin_11

#define SAD_Read 					0xBF	// The 8-bit slave address(SAD) of HTS221 sensor "READ"
#define SAD_Write 				0xBE	// The 8-bit slave address(SAD) of HTS221 sensor "Write"
#define SAD								0x5F<<1	// The 8-bit slave address(SAD) of HTS221 sensor

/* Data address register */
#define REG_WHOAMI_ADDR 	0x0F	// Who am i address register
#define REG_AV_CONF_ADDR	0x10	// Humidity and Temperature conf register
#define REG_CTRL1_ADDR		0x20	// Control address register 1  
#define REG_CTRL2_ADDR		0x21	// Control address register 2
#define REG_CTRL3_ADDR		0x23	// Control address register 3
#define REG_STATUS				0x27	// Status address register

/* Control address register */
#define REG_H_OUT_L				0x28	// Out humidity address register (LSB)
#define REG_H_OUT_M				0x29	// Out humidity address register (MSB)
#define REG_T_OUT_L			  0x2A	// Out temperature address register (LSB)
#define REG_T_OUT_M				0x2B	// Out temperature address register (MSB)

/* Calibration register */
#define REG_H0_rH					0x30	// Calibration H 0 RH address register 
#define REG_H1_rH					0x31	// Calibration H 1 RH address register 
#define REG_H0_T0_OUT_L		0x36	// Calibration H 0 LSB address register
#define REG_H0_T0_OUT_M		0x37	// Calibration H 0 MSB address register
#define REG_H1_T0_OUT_L		0x3A	// Calibration H 1 LSB address register
#define REG_H1_T0_OUT_M		0x3B  // Calibration H 1 MSB address register

#define REG_T0_degC				0x32	// Calibration T 0 degC address register
#define REG_T1_degC				0x33	// Calibration T 1 degC address register
#define REG_T0_OUT_L			0x3C  // Calibration T 0 LSB address register
#define REG_T0_OUT_M			0x3D	// Calibration T 0 MSB address register 
#define REG_T1_OUT_L			0x3E	// Calibration T 1 LSB address register 
#define REG_T1_OUT_M			0x3F	// Calibration T 1 LSB address register 
#define REG_T1_T0_msb			0x35

#define BUFF_SIZE 16

uint8_t buff[BUFF_SIZE]={0};
float T0_degC, T1_degC;
volatile float DegC;
float H0_rH, H1_rH; 

int16_t T0_out, T1_out;
int16_t H0_out, H1_out;

char info[4];
unsigned char i;
unsigned char value;
uint64_t timeOut;
bool err=FALSE;

bool IsTHSrErr(void){
	return err;
}

void I2CInit(void){
	
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin 	= I2C_SCL | I2C_SDA;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_2;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	/* If no pull to high, program will stop on I2C commucation. */
  //GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	/* Connect ST HTS221 SCL, SDA need pull high pin */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* GPIO_PinAFConfig must follow "GPIO_Pin" & "Alternate function" to define */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_1);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_1);
	
	/* I2C setting */
	I2C_InitStructure.I2C_Mode 				= I2C_Mode_I2C;
	//I2C_InitStructure.I2C_DutyCycle 	= I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;	
	I2C_InitStructure.I2C_Ack					= I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_Timing = 0x10805E89;
	//I2C_InitStructure.I2C_ClockSpeed 	= 100000 ;
	
	I2C_Init(I2C2, &I2C_InitStructure);
	I2C_Cmd(I2C2, ENABLE);
}

/* Application ---------------------------------------------------------------*/
/**
  * @brief  Test function, call ST HTS221 internal address "WHO_AM_I"
	*					See HTS221 spec description, Page.18 - Section 7.1 WHO_AM_I 
	*	@Addr		0x0F (Read only)
	*				-----------------------------------------------------------------
	*	byte	|		7		|		6		|		5		|		4		|		3		|		2		|		1		|		0		|
	*				-----------------------------------------------------------------
	*	BIN		|		1		|		0		|		1		|		1		|		1		|		1		|		0		|		0		|
	*				-----------------------------------------------------------------
  */
uint8_t WHO_AM_I(void){
	// SAK: Slave Acknowledge
	// After Master send SAD+W, Slave return SAK
	
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));
	
	I2C_TransferHandling(I2C2, SAD, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
	
	while(I2C_GetFlagStatus(I2C2, I2C_ISR_TXIS) == RESET);
	// After Master send SUB; Slave return SAK
	I2C_SendData(I2C2, REG_WHOAMI_ADDR);
  while(I2C_GetFlagStatus(I2C2, I2C_ISR_TC) == RESET);
	
	/* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(I2C2, SAD, 1, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);
	
	while(I2C_GetFlagStatus(I2C2, I2C_ISR_RXNE) == RESET);
  uint8_t data = I2C_ReceiveData(I2C2);
	//value = I2C_read_NMAK(I2C2);
	
	timeOut=undateNum<<1; 
  while(!I2C_GetFlagStatus(I2C2, I2C_FLAG_STOPF))
	{
		if(!(--timeOut))
			return data;
	}
	I2C_ClearFlag(I2C2, I2C_ICR_STOPCF);
	return data;
}

/**
  * @brief  Humidity and temperature resloution mode
	*					See HTS221 spec description, Page.17 & Page.18 - Section 7.2 AV_CONF
  * @Addr		0x10 (Read/Write)
	*	-----------------------------------------------------------------
	*	|		7		|		6		|		5		|		4		|		3		|		2		|		1		|		0		|
	*	-----------------------------------------------------------------
	*	|		Reserved		|	AVGT2	|	AVGT1	|	AVGT0	|	AVGH2	|	AVGH1	|	AVGH0	|
	*	-----------------------------------------------------------------
  */
uint8_t AV_CONF(void){
	//I2C_SAD(I2C2, SAD_Write, I2C_Direction_Transmitter);
	
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));
	I2C_TransferHandling(I2C2, SAD, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
	
	while(I2C_GetFlagStatus(I2C2, I2C_ISR_TXIS) == RESET);
	
	I2C_SendData(I2C2, REG_AV_CONF_ADDR);
  while(I2C_GetFlagStatus(I2C2, I2C_ISR_TC) == RESET);
	
	//I2C_SAD(I2C2, SAD_Read, I2C_Direction_Receiver);
	I2C_TransferHandling(I2C2, SAD, 1, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);
	
	while(I2C_GetFlagStatus(I2C2, I2C_ISR_RXNE) == RESET);
  uint8_t data = I2C_ReceiveData(I2C2);
	//value = I2C_read_NMAK(I2C2);
	
	timeOut=undateNum<<1; 
  while(!I2C_GetFlagStatus(I2C2, I2C_FLAG_STOPF))
	{
		if(!(--timeOut))
			return data;
	}
	I2C_ClearFlag(I2C2, I2C_ICR_STOPCF);
	return data;
}

/**
  * @brief  Control Register 1
	*					See HTS221 spec description,  Page.19 - Section 7.3
  * @Addr		0x20 (Read/Write)
	*	-----------------------------------------------------------------
	*	|		7		|		6		|		5		|		4		|		3		|		2		|		1		|		0		|
	*	-----------------------------------------------------------------
	*	|		PD	|					  Reserved						|	 BDU	|	 ODR1 |	 ODR0	|	
	*	-----------------------------------------------------------------
	*
	* @brief  Control Register 2
	*					See HTS221 spec description,  Page.20 - Section 7.4
  * @Addr		0x21 (Read/Write)
	*	-----------------------------------------------------------------
	*	|		7		|		6		|		5		|		4		|		3		|		2		|		1	 |		0		|
	*	-----------------------------------------------------------------
	*	|  BOOT	|					  					Reserved					|Heater|ONE_SHOT|
	*	-----------------------------------------------------------------
	*
	* @brief  Control Register 3
	*					See HTS221 spec description,  Page.21 - Section 7.5
	* @Addr		0x22 (Read/Write)
	*	------------------------------------------------------------------
	*	|		7		 |		6		|		5		|		4		|		3		|		2		|		1		|		0		|
	*	------------------------------------------------------------------
	*	|DRDY_H_L|	PP_OD	|				 Reserved				|	 DRDY	|			Reserved	|	
	*	------------------------------------------------------------------
  */
void SetCtrlReg(I2C_TypeDef* I2Cx, uint8_t reg, uint8_t data){

#ifdef HTS221_DEBUG
	printf("SetCtrlReg(%x, %x) I2C_GetFlagStatus(I2Cx, I2C_ISR_BUSY)\n\r", reg, data);
#endif	
	timeOut=undateNum<<1;
	while(I2C_GetFlagStatus(I2Cx, I2C_ISR_BUSY) != RESET){ 
		if(!(--timeOut)){
			err=TRUE;
			return;
		}
	}

#ifdef HTS221_DEBUG
	printf("CTRL_REG(%x, %x) I2C_GetFlagStatus(I2Cx, I2C_ISR_TXIS)\n\r", reg, data);
#endif    	
  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(I2Cx, SAD, 1, I2C_Reload_Mode, I2C_Generate_Start_Write);
  
	timeOut=undateNum<<1;
  while(I2C_GetFlagStatus(I2Cx, I2C_ISR_TXIS) == RESET){ 
		if(!(--timeOut)){
			err=TRUE;
			return;
		}
	}

#ifdef HTS221_DEBUG
	printf("SetCtrlReg(%x, %x) I2C_SendData()\n\r", reg, data);
#endif
  /* Send Register address */
  I2C_SendData(I2Cx, reg);

#ifdef HTS221_DEBUG
	printf("SetCtrlReg(%x, %x) I2C_GetFlagStatus(I2Cx, I2C_ISR_TCR)\n\r", reg, data);
#endif  
	timeOut=undateNum<<1;
  while(I2C_GetFlagStatus(I2Cx, I2C_ISR_TCR) == RESET){ 
		if(!(--timeOut)){
			err=TRUE;
			return;
		}
	}
	
  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(I2Cx, SAD, 1, I2C_AutoEnd_Mode, I2C_No_StartStop);

#ifdef HTS221_DEBUG
	printf("SetCtrlReg(%x, %x) I2C_GetFlagStatus(I2Cx, I2C_ISR_TXIS)\n\r", reg, data);
#endif     
	timeOut=undateNum<<1;
  while(I2C_GetFlagStatus(I2Cx, I2C_ISR_TXIS) == RESET){ 
		if(!(--timeOut)){
			err=TRUE;
			return;
		}
	}
   
  /* Write data to TXDR */
  I2C_SendData(I2Cx, data);

#ifdef HTS221_DEBUG
	printf("SetCtrlReg(%x, %x) I2C_GetFlagStatus(I2Cx, I2C_ISR_STOPF)\n\r", reg, data);
#endif     
  timeOut=undateNum<<1;
  while(I2C_GetFlagStatus(I2Cx, I2C_ISR_STOPF) == RESET){ 
		if(!(--timeOut)){
			err=TRUE;
			return;
		}
	}
 
  /* Clear STOPF flag */
  I2C_ClearFlag(I2Cx, I2C_ICR_STOPCF);
 
#ifdef HTS221_DEBUG
	printf("SetCtrlReg(%x, %x) Success.\n\r", reg, data);
#endif  	
}

void write_Register(uint8_t REG, uint8_t data){
	
	while(I2C_GetFlagStatus(I2C2, I2C_ISR_BUSY) != RESET);
#ifdef HTS221_DEBUG
	printf("write_Register()I2C_GetFlagStatus(I2C2, I2C_ISR_BUSY)\n\r");
#endif   
  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(I2C2, SAD, 2, I2C_AutoEnd_Mode, I2C_Generate_Start_Write);
  
  while(I2C_GetFlagStatus(I2C2, I2C_ISR_TXIS) == RESET);
#ifdef HTS221_DEBUG
	printf("write_Register()I2C_GetFlagStatus(I2C2, I2C_ISR_TXIS)\n\r");
#endif     
  I2C_SendData(I2C2, REG);
#ifdef HTS221_DEBUG
	printf("write_Register()I2C_SendData(I2C2, reg)\n\r");
#endif      
  while(I2C_GetFlagStatus(I2C2, I2C_ISR_TC) == RESET);
#ifdef HTS221_DEBUG
	printf("write_Register()I2C_GetFlagStatus(I2C2, I2C_ISR_TC)\n\r");
#endif    
  I2C_SendData(I2C2, data);
  while(I2C_GetFlagStatus(I2C2, I2C_ISR_TC) == RESET);
#ifdef HTS221_DEBUG
	printf("write_Register()I2C_GetFlagStatus(I2C2, I2C_ISR_TCR)\n\r");
#endif    
  while(I2C_GetFlagStatus(I2C2, I2C_ISR_STOPF) == RESET);
#ifdef HTS221_DEBUG
	printf("write_Register()I2C_GetFlagStatus(I2C2, I2C_ISR_STOPF)\n\r");
#endif        
  /* Clear STOPF flag */
  I2C_ClearFlag(I2C2, I2C_ICR_STOPCF);
}

void HTS221_Init(void){
	
	//I2CInit();
	err=FALSE;
	SetCtrlReg(I2C2, REG_CTRL1_ADDR, Bit7|Bit2|Bit0);
	//Bit7:Power On, Bit2:Updated after reading, (Bit1)Bit0:01 1 Hz
	if(err) return;
	SetCtrlReg(I2C2, REG_CTRL2_ADDR, Bit7);
	//Bit7:Reboot memory content
	
	//CTRL_REG(I2C2, REG_CTRL3_ADDR, 0x00);
	// CTRL_REG3
}

/**
  * @brief  Check HTS221 state
  * @param  None
  * @retval None
  */
uint8_t checkHTS221(void){

	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));
	I2C_TransferHandling(I2C2, SAD, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
	
	while(I2C_GetFlagStatus(I2C2, I2C_ISR_TXIS) == RESET);
	I2C_SendData(I2C2, REG_STATUS);
  while(I2C_GetFlagStatus(I2C2, I2C_ISR_TC) == RESET);
	
	I2C_TransferHandling(I2C2, SAD, 1, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);
	
	while(I2C_GetFlagStatus(I2C2, I2C_ISR_RXNE) == RESET);
  uint8_t data = I2C_ReceiveData(I2C2);
	
	timeOut=undateNum<<1; 
  while(!I2C_GetFlagStatus(I2C2, I2C_FLAG_STOPF))
	{
		if(!(--timeOut)){
			err=TRUE;
			return data;
		}
	}
 
  I2C_ClearFlag(I2C2, I2C_ICR_STOPCF);
	return data;
}


/**
  * @brief  STM32 I2C read register 
  * @param  None
  * @retval None
  */
uint8_t I2C_ReadTHSnr(uint8_t ThsReg, uint8_t nBytes){
	
	uint8_t num=0;
	memset(buff, 0, BUFF_SIZE);

	timeOut=undateNum<<1;
  while( I2C_GetFlagStatus(I2C2, I2C_ISR_BUSY) )
	{ 
		if(!(--timeOut)){
#ifdef HTS221_DEBUG
	printf("I2C_ReadTHSnr(%x) Failed!! @(I2C2, I2C_ISR_BUSY)\n\r",ThsReg);
#endif   	
			err=TRUE;
			return 0xff;
		}
	}
 
  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(I2C2, SAD, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
	     
  timeOut=undateNum<<1;
	while( !I2C_GetFlagStatus(I2C2, I2C_ISR_TXIS))
	{
		if(!(--timeOut)){
#ifdef HTS221_DEBUG
	printf("I2C_ReadTHSnr(%x) Failed!! @(I2C2, I2C_ISR_TXIS)\n\r",ThsReg);
#endif
			err=TRUE;
			return 0xff;
		}
	}

  I2C_SendData(I2C2, ThsReg|Bit7);
#ifdef HTS221_DEBUG
	printf("I2C_ReadTHSnr(%x) I2C_SendData(I2C2, %d)\n\r",ThsReg, ThsReg|Bit7);
#endif
  
	timeOut=undateNum<<1;
  while( !I2C_GetFlagStatus(I2C2, I2C_ISR_TC) )
	{
		if(!(--timeOut)){
#ifdef HTS221_DEBUG
	printf("I2C_ReadTHSnr(%x) Failes!! @(I2C2, I2C_ISR_TC)\n\r",ThsReg);
#endif      
			err=TRUE;
			return 0xff;
		}
	}

  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(I2C2, SAD, nBytes, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);
   
	timeOut=undateNum<<1; 
  while( I2C_GetFlagStatus(I2C2, I2C_ISR_RXNE) )
	{
			buff[num++]= I2C_ReceiveData(I2C2);
	}
#ifdef HTS221_DEBUG
	printf("I2C_ReadTHSnr(%x) receives %d bytes\n\r",ThsReg, num);
#endif    
	timeOut=undateNum<<1; 
  while(!I2C_GetFlagStatus(I2C2, I2C_FLAG_STOPF))
	{
		if(!(--timeOut)){
#ifdef HTS221_DEBUG
	printf("I2C_ReadTHSnr(%x) Failed!! @(I2C2, I2C_ISR_STOPF)\n\r",ThsReg);
#endif    
			err=TRUE;
			break;
		}
	}

  //Clear the stop flag for next transfers
  I2C_ClearFlag(I2C2, I2C_ICR_STOPCF);    

	#ifdef HTS221_DEBUG
	printf("I2C_ReadTHSnr(%x) success.\n\r",ThsReg);
#endif    	
	return num;
}

/**
  * @brief  Do HTS221 calibration 
	* @spec		Spec HTS221 
  * @param  None
  * @retval None
  */
void CalibHTS221(void){
	/* Temperature Variables declare */
	uint8_t T0_degC_x8_L, T0_degC_x8_H; 
	uint8_t T1_degC_x8_L, T1_degC_x8_H;
	uint8_t T0_out_H, T0_out_L;
	uint8_t T1_out_H, T1_out_L;
	/* Humidity Variables declare */
	uint8_t H0_T0_out_M, H0_T0_out_L; 
	uint8_t H1_T0_out_M, H1_T0_out_L;
	
	/* T0, T1 DegC calculate */
	T0_degC_x8_H = (uint16_t)(read_Register(REG_T1_T0_msb) & 0x03);
	T0_degC_x8_L = (uint16_t)read_Register(REG_T0_degC);
	
	T1_degC_x8_H = (uint16_t)(read_Register(REG_T1_T0_msb)& 0x0C);
	T1_degC_x8_H = T1_degC_x8_H >> 2;
	T1_degC_x8_L = (uint16_t)read_Register(REG_T1_degC);
	
	T0_degC = ((float)((T0_degC_x8_H<<8) | (T0_degC_x8_L)))/8;
	T1_degC = ((float)((T1_degC_x8_H<<8) | (T1_degC_x8_L)))/8;
	
	/* T0, T1 calculate */
	T0_out_H = read_Register(REG_T0_OUT_M);
	T0_out_L = read_Register(REG_T0_OUT_L);
	
	T1_out_H = read_Register(REG_T1_OUT_M);
	T1_out_L = read_Register(REG_T1_OUT_L);
	
	T0_out = (int16_t)((T0_out_H << 8) | T0_out_L);
	T1_out = (int16_t)((T1_out_H << 8) | T1_out_L);
	
	/* H0, H1 RH calculate */
	H0_rH = (float)read_Register(REG_H0_rH)/2;
	H1_rH = (float)read_Register(REG_H1_rH)/2;
	
	H0_T0_out_M =	read_Register(REG_H0_T0_OUT_M);
	H0_T0_out_L = read_Register(REG_H0_T0_OUT_L);
	H1_T0_out_M = read_Register(REG_H1_T0_OUT_M);
	H1_T0_out_L =	read_Register(REG_H1_T0_OUT_L);
	
	H0_out = (int16_t)((H0_T0_out_M << 8) | (H0_T0_out_L));
	H1_out = (int16_t)((H1_T0_out_M << 8) | (H1_T0_out_L));
	
	
#ifdef HTS221_DEBUG
	/* DEBUG variable clare */
	printf("----------HTS221 DEBUG Info-----------\n\r");
	printf("Temperature Calibration---------------\n\r");
	uint16_t temp = (T0_degC_x8_H<<8) | (T0_degC_x8_L);
	printf("T0_degC_H:%02d, T0_degC_L:%02d | T0_degC: %04d\n\r", T0_degC_x8_H, T0_degC_x8_L, temp);
	
	temp = (T1_degC_x8_H<<8) | (T1_degC_x8_L);
	printf("T1_degC_H:%02d, T1_degC_L:%02d | T1_degC: %04d\n\r", T1_degC_x8_H, T1_degC_x8_L, temp);
	
	printf("Temperature DegC T0: %.2f T1: %.2f\n\r", T0_degC, T1_degC);
	
	temp = (T0_out_H << 8) | (T0_out_L);
	printf("T0_out_H:%02d, T0_out_L:%02d | T0_out: %04d\n\r", T0_out_H, T0_out_L, temp);
	
	temp = (T1_out_H << 8) | (T1_out_L);
	printf("T1_out_H:%02d, T1_out_L:%02d | T1_out: %04d\n\r", T1_out_H, T1_out_L, temp);
	
	printf("Humidity Calibration------------------\n\r");
	printf("H0_rH: %.2f, H1_rH: %.2f\n\r", H0_rH, H1_rH);
	
	printf("---------------------\n\r");
	
#endif	
}

/**
  * @brief  Get Temperature information
	* @spec		Spec HTS221 
  * @param  None
  * @retval None
  */
float GetTemperature(void){
	err=FALSE;
	uint8_t T_out_M, T_out_L;
	int16_t T_out;

	/* T out calculate */
	T_out_M = read_Register(REG_T_OUT_M);
	if(err) 
		return 0xff;
	T_out_L = read_Register(REG_T_OUT_L);
	if(err) 
		return 0xff;
	
	T_out = (int16_t)((T_out_M << 8) | T_out_L);
	float T_degC = (((float)(T_out - T0_out))/(T1_out - T0_out)) * (T1_degC - T0_degC) + T0_degC;
	
#ifdef HTS221_DEBUG
	/* DEBUG variable clare */
	uint16_t temp = (T_out_M << 8) | (T_out_L);
	printf("T_out_M:%02d, T_out_L:%02d | T_out: %04d\n\r", T_out_M, T_out_L, temp);
	//printf("T0_OUT:%d, T1_OUT:%d, T_OUT:%d\n\r", T0_out, T1_out, T_out);
	printf("T_degC:%.6f\n\r", T_degC);
	printf("---------------------\n\r");
#endif
	return T_degC;
}

/**
  * @brief  Get Humidity information
	* @spec		Spec HTS221 
  * @param  None
  * @retval None
  */
float GetHumidity(void){
	err=FALSE;
	uint8_t H_out_L, H_out_M;
	int16_t H_out;
	
	H_out_M = read_Register(REG_H_OUT_M);
	if(err) 
		return 0xff;
	H_out_L = read_Register(REG_H_OUT_L);
	if(err) 
		return 0xff;
	
	H_out = (int16_t)(( H_out_M << 8) | H_out_L);
	float H_RH = ((float)(H_out - H1_out)/(H0_out - H1_out)) * (H1_rH - H0_rH ) + H1_rH; 

#ifdef HTS221_DEBUG	
	printf("T_out_M:%02d, T_out_L:%02d | T_out: %04d\n\r", H_out_M, H_out_L, H_out);
	printf("H_RH:%.2f\n\r", H_RH);
	printf("---------------------\n\r");
#endif
	
	return H_RH;
}

/**
  * @brief  Show HTS221 Temperture & Humidity data
  * @param  None
  * @retval None
  */
void showSensorInfo(void){
	float T_degC = GetTemperature();
	float H_RH = GetHumidity();
	sprintf(info, "%.1f", T_degC);
	DegC = T_degC;
	
	writeIns(0xCA);
	DisplayStr((unsigned char *)info); 

	sprintf(info, "%.1f", H_RH);
	writeIns(0x9A);
	DisplayStr((unsigned char *)info); 
}

/**
  * @brief  Show HTS221 all register data
  * @param  None
  * @retval None
  */
void showRegData(void){
	uint16_t temp;
	
	temp = read_Register(0x10);
	printf("\r\nReg 0x10:%02x\n\r",temp);
	temp = read_Register(0x1E);
	printf("Reg 0x1E:%02x\n\r",temp);
	temp = read_Register(0x1F);
	printf("Reg 0x1F:%02x\n\r",temp);
	temp = read_Register(0x20);
	printf("Reg 0x20:%02x\n\r",temp);
	temp = read_Register(0x21);
	printf("Reg 0x21:%02x\n\r",temp);
	temp = read_Register(0x27);
	printf("Reg 0x27:%02x\n\r",temp);
	temp = read_Register(0x28);
	printf("Reg 0x28:%02x\n\r",temp);
	temp = read_Register(0x29);
	printf("Reg 0x29:%02x\n\r",temp);
	temp = read_Register(0x2A);
	printf("Reg 0x2A:%02x\n\r",temp);
	temp = read_Register(0x2B);
	printf("Reg 0x2B:%02x\n\r",temp);
	
	printf("------------------\n\r");
	temp = read_Register(0x30);
	printf("Reg 0x30:%02x\n\r",temp);
	temp = read_Register(0x31);
	printf("Reg 0x31:%02x\n\r",temp);
	temp = read_Register(0x32);
	printf("Reg 0x32:%02x\n\r",temp);
	temp = read_Register(0x33);
	printf("Reg 0x33:%02x\n\r",temp);
	temp = read_Register(0x34);
	printf("Reg 0x34:%02x\n\r",temp);
	temp = read_Register(0x35);
	printf("Reg 0x35:%02x\n\r",temp);
	temp = read_Register(0x36);
	printf("Reg 0x36:%02x\n\r",temp);
	temp = read_Register(0x37);
	printf("Reg 0x37:%02x\n\r",temp);
	temp = read_Register(0x38);
	printf("Reg 0x38:%02x\n\r",temp);
	temp = read_Register(0x39);
	printf("Reg 0x39:%02x\n\r",temp);
	temp = read_Register(0x3A);
	printf("Reg 0x3A:%02x\n\r",temp);
	temp = read_Register(0x3B);
	printf("Reg 0x3B:%02x\n\r",temp);
	temp = read_Register(0x3C);
	printf("Reg 0x3C:%02x\n\r",temp);
	temp = read_Register(0x3D);
	printf("Reg 0x3D:%02x\n\r",temp);
	temp = read_Register(0x3E);
	printf("Reg 0x3E:%02x\n\r",temp);
	temp = read_Register(0x3F);
	printf("Reg 0x3F:%02x\n\r",temp);

}
