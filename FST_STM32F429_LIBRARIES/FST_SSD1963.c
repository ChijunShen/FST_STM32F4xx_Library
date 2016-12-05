#include "FST_SSD1963.h"
//DISPLAY ADDITIONAL COMMANDS


void FST_SSD1963_Port_Init(void)
{
	GPIO_InitTypeDef GPIO_InitDef;
	/**SET THE GPIOG OUTPUTS **/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	//SSD_BACKLIT 
	GPIO_InitDef.GPIO_Pin = SSD_BACKLIT;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOG, &GPIO_InitDef);
	
	//Common settings
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;


	//GPIOD pins for FMC
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource7, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FMC);
	//                      D2           D3           NOE -> /RD   NWE -> /WR   NE1 -> /CS   D13          D14          D15           A16 -> D/C    D0            D1
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOD, &GPIO_InitDef);


	//GPIOE pins for FMC
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	GPIO_PinAFConfig(GPIOE, GPIO_PinSource7, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource12, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource15, GPIO_AF_FMC);
	//                     D4          D5          D6          D7           D8           D9           D10          D11          D12
	GPIO_InitDef.GPIO_Pin =GPIO_Pin_7 |GPIO_Pin_8 |GPIO_Pin_9 |GPIO_Pin_10 |GPIO_Pin_11 |GPIO_Pin_12 |GPIO_Pin_13 |GPIO_Pin_14 |GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitDef);
}

uint8_t FST_SSD1963_SLOW_Init(void)
{
	FMC_NORSRAMInitTypeDef  FMC_NORSRAMInitDef;
	FMC_NORSRAMTimingInitTypeDef  FMC_NORSRAMTimingInitDef;

	/* Initialize FMC pins */
	FST_SSD1963_Port_Init();

	/* Enable FMC clock */
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FMC, ENABLE);

	/* FMC SSD1963 as a NORSRAM device initialization sequence ---*/
	/* Step 1 ----------------------------------------------------------*/
	/* Timing configuration for 84 Mhz of SD clock frequency (168Mhz/2) */
	FMC_NORSRAMTimingInitDef.FMC_AddressSetupTime		= 0x0F;
	FMC_NORSRAMTimingInitDef.FMC_AddressHoldTime		= 0x00;
	FMC_NORSRAMTimingInitDef.FMC_DataSetupTime			= 0xFF;
	FMC_NORSRAMTimingInitDef.FMC_BusTurnAroundDuration  = 0x00;
	FMC_NORSRAMTimingInitDef.FMC_CLKDivision    		= 0x00;
	FMC_NORSRAMTimingInitDef.FMC_DataLatency        = 0x00;
	FMC_NORSRAMTimingInitDef.FMC_AccessMode         = FMC_AccessMode_A;

	/* FMC SDRAM control configuration */
	FMC_NORSRAMInitDef.FMC_Bank 						= FMC_Bank1_NORSRAM1;
  FMC_NORSRAMInitDef.FMC_DataAddressMux 				= FMC_DataAddressMux_Disable;
	FMC_NORSRAMInitDef.FMC_MemoryType 					= FMC_MemoryType_SRAM;
	FMC_NORSRAMInitDef.FMC_MemoryDataWidth 				= FMC_NORSRAM_MemoryDataWidth_16b;
	FMC_NORSRAMInitDef.FMC_BurstAccessMode 				= FMC_BurstAccessMode_Disable;
	FMC_NORSRAMInitDef.FMC_AsynchronousWait 			= FMC_AsynchronousWait_Disable;
	FMC_NORSRAMInitDef.FMC_WaitSignalPolarity 			= FMC_WaitSignalPolarity_Low;
	FMC_NORSRAMInitDef.FMC_WrapMode 					= FMC_WrapMode_Disable;
	FMC_NORSRAMInitDef.FMC_WaitSignalActive 			= FMC_WaitSignalActive_BeforeWaitState;
	FMC_NORSRAMInitDef.FMC_WriteOperation 				= FMC_WriteOperation_Enable;
	FMC_NORSRAMInitDef.FMC_WaitSignal 			    	= FMC_WaitSignal_Disable;
	FMC_NORSRAMInitDef.FMC_ExtendedMode 				= FMC_ExtendedMode_Disable;
	FMC_NORSRAMInitDef.FMC_WriteBurst 					= FMC_WriteBurst_Disable;
	FMC_NORSRAMInitDef.FMC_ContinousClock 				= FMC_CClock_SyncOnly;  //FMC_CClock_SyncAsync
	FMC_NORSRAMInitDef.FMC_ReadWriteTimingStruct		= &FMC_NORSRAMTimingInitDef;
	FMC_NORSRAMInitDef.FMC_WriteTimingStruct			= &FMC_NORSRAMTimingInitDef;
	/* FMC NORSRAM bank initialization */
	FMC_NORSRAMInit(&FMC_NORSRAMInitDef);
	/* FMC NORSRAM bank enable */
	FMC_NORSRAMCmd(FMC_Bank1_NORSRAM1, ENABLE);

	return 1;
}
uint8_t FST_SSD1963_FAST_Init(void)
{
	FMC_NORSRAMInitTypeDef  FMC_NORSRAMInitDef;
	FMC_NORSRAMTimingInitTypeDef  FMC_NORSRAMTimingInitDef_Write, FMC_NORSRAMTimingInitDef_Read;

	/* Enable FMC clock */
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FMC, ENABLE);

	/* FMC SSD1963 as a NORSRAM device initialization sequence ---*/
	/* Step 1 ----------------------------------------------------------*/
	/* Timing configuration for 84 Mhz of SD clock frequency (168Mhz/2) */
	FMC_NORSRAMTimingInitDef_Write.FMC_AddressSetupTime			= 0x01;
	FMC_NORSRAMTimingInitDef_Write.FMC_AddressHoldTime			= 0x00;
	FMC_NORSRAMTimingInitDef_Write.FMC_DataSetupTime			= 0x01;
	FMC_NORSRAMTimingInitDef_Write.FMC_BusTurnAroundDuration  	= 0x00;
	FMC_NORSRAMTimingInitDef_Write.FMC_CLKDivision    			= 0x01;
	FMC_NORSRAMTimingInitDef_Write.FMC_DataLatency           	= 0x00;
	FMC_NORSRAMTimingInitDef_Write.FMC_AccessMode             	= FMC_AccessMode_A;

	/* Timing configuration for 84 Mhz of SD clock frequency (168Mhz/2) */
	FMC_NORSRAMTimingInitDef_Read.FMC_AddressSetupTime			= 0x0A;
	FMC_NORSRAMTimingInitDef_Read.FMC_AddressHoldTime			= 0x00;
	FMC_NORSRAMTimingInitDef_Read.FMC_DataSetupTime				= 0x0A;
	FMC_NORSRAMTimingInitDef_Read.FMC_BusTurnAroundDuration 	= 0x00;
	FMC_NORSRAMTimingInitDef_Read.FMC_CLKDivision    			= 0x01;
	FMC_NORSRAMTimingInitDef_Read.FMC_DataLatency           	= 0x00;
	FMC_NORSRAMTimingInitDef_Read.FMC_AccessMode            	= FMC_AccessMode_A;

	/* FMC SDRAM control configuration */
	FMC_NORSRAMInitDef.FMC_Bank 						= FMC_Bank1_NORSRAM1;
  FMC_NORSRAMInitDef.FMC_DataAddressMux 				= FMC_DataAddressMux_Disable;
	FMC_NORSRAMInitDef.FMC_MemoryType 					= FMC_MemoryType_SRAM;
	FMC_NORSRAMInitDef.FMC_MemoryDataWidth 				= FMC_NORSRAM_MemoryDataWidth_16b;
	FMC_NORSRAMInitDef.FMC_BurstAccessMode 				= FMC_BurstAccessMode_Disable;
	FMC_NORSRAMInitDef.FMC_AsynchronousWait 			= FMC_AsynchronousWait_Disable;
	FMC_NORSRAMInitDef.FMC_WaitSignalPolarity 			= FMC_WaitSignalPolarity_Low;
	FMC_NORSRAMInitDef.FMC_WrapMode 					= FMC_WrapMode_Disable;
	FMC_NORSRAMInitDef.FMC_WaitSignalActive 			= FMC_WaitSignalActive_BeforeWaitState;
	FMC_NORSRAMInitDef.FMC_WriteOperation 				= FMC_WriteOperation_Enable;
	FMC_NORSRAMInitDef.FMC_WaitSignal 			    	= FMC_WaitSignal_Disable;
	FMC_NORSRAMInitDef.FMC_ExtendedMode 				= FMC_ExtendedMode_Disable;
	FMC_NORSRAMInitDef.FMC_WriteBurst 					= FMC_WriteBurst_Disable;
	FMC_NORSRAMInitDef.FMC_ContinousClock 				= FMC_CClock_SyncOnly;  //FMC_CClock_SyncAsync
	FMC_NORSRAMInitDef.FMC_ReadWriteTimingStruct		= &FMC_NORSRAMTimingInitDef_Read;
	FMC_NORSRAMInitDef.FMC_WriteTimingStruct			= &FMC_NORSRAMTimingInitDef_Write;
	/* DISABLING THE NORSRAM BANK WHILE CHANGING SPEED */
	FMC_NORSRAMCmd(FMC_Bank1_NORSRAM1, DISABLE);
	/* FMC NORSRAM bank initialization */
	FMC_NORSRAMInit(&FMC_NORSRAMInitDef);
	/* FMC NORSRAM bank enable */
	FMC_NORSRAMCmd(FMC_Bank1_NORSRAM1, ENABLE);
	return 1;
}


void FST_SSD1963_Write_cmd(uint16_t value) {
	*(uint16_t*) (0x60000000) = value;
}
void FST_SSD1963_Write_data(uint16_t value) {
	*(uint16_t*) (0x60020000) = value;
}

void delay_time(unsigned int i)
{
    unsigned int a;
    unsigned int b;
    for(b=0;b<i;b++)
    for(a=0;a<1000;a++);
}
void FST_SSD1963_Clear(uint16_t color){

	uint16_t x,y = 800;
	FST_SSD1963_SetPos(0,799,0,479);
	while(y--){
		for(x=0; x<480; x++)
			FST_SSD1963_Write_data(color);
	}

}
void FST_SSD1963_SetPos(uint16_t xs, uint16_t xe, uint16_t ys, uint16_t ye){

	//SET COLUMN ADDRESS
	FST_SSD1963_Write_cmd(0x2A);
	FST_SSD1963_Write_data((xs>>8)&0x00FF);		//Highest byte SC
	FST_SSD1963_Write_data(xs & 0x00FF);		//Lowest byte SC, Start Column
	FST_SSD1963_Write_data((xe>>8)&0x00FF);		//Highest byte EC
	FST_SSD1963_Write_data(xe & 0x00FF);		//Lowest byte EC, End Column

	//SET COLUMN ADDRESS
	FST_SSD1963_Write_cmd(0x2B);
	FST_SSD1963_Write_data((ys>>8)&0x00FF);		//Highest byte SP
	FST_SSD1963_Write_data(ys & 0x00FF);		//Lowest byte SP, Start Page
	FST_SSD1963_Write_data((ye>>8)&0x00FF);		//Highest byte EP
	FST_SSD1963_Write_data(ye & 0x00FF);		//Lowest byte EP, End Page

	//WRITE MEMORY START
	FST_SSD1963_Write_cmd(0x2C);

}
uint16_t FST_SSD1963_ConvertColor(uint32_t color){

	uint16_t color565;

	color565=(((color>>19)&0x1F)<<11 | ((color>>10)&0x3F)<<5 | ((color>>3)&0x1F));

	return color565;

}




