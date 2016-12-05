#include "FST_CAN_Library.h"
void CAN_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/*enable can gpio*/
	RCC_AHB1PeriphClockCmd(CAN_TX_GPIO_CLK|CAN_RX_GPIO_CLK, ENABLE);
	GPIO_PinAFConfig(CAN_TX_GPIO_PORT, CAN_RX_SOURCE, CAN_AF_PORT);
	GPIO_PinAFConfig(CAN_RX_GPIO_PORT, CAN_TX_SOURCE, CAN_AF_PORT);
	
	/*config can tx*/
	GPIO_InitStructure.GPIO_Pin = CAN_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(CAN_TX_GPIO_PORT, &GPIO_InitStructure);
	
	/*congig can rx*/
	GPIO_InitStructure.GPIO_Pin = CAN_RX_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(CAN_RX_GPIO_PORT, &GPIO_InitStructure);
}

void CAN_Mode_Config(void)
{
	CAN_InitTypeDef CAN_InitStructure;
	/* Enable CAN clock */
	RCC_APB1PeriphClockCmd(CAN_CLK, ENABLE);
	/*CAN Register init*/
	CAN_DeInit(CANx);
	CAN_StructInit(&CAN_InitStructure);
	/*CAN Unit Init*/
	CAN_InitStructure.CAN_TTCM=DISABLE; //MCR-TTCM
	CAN_InitStructure.CAN_ABOM=ENABLE;	//MCR-ABOM
	CAN_InitStructure.CAN_AWUM=ENABLE;	//MCR-AWUM
	CAN_InitStructure.CAN_NART=DISABLE;	//MCR-NART
	CAN_InitStructure.CAN_RFLM=DISABLE;	//MCR-RFLM
	CAN_InitStructure.CAN_TXFP=DISABLE; //MCR-TXFP
	CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack;
	CAN_InitStructure.CAN_SJW=CAN_SJW_2tq; //BTR-SJW
	
	/* ss=1 bs1=5 bs2=3 time length (1+5+3) Baudrate=tq*(1+3+5) */
	CAN_InitStructure.CAN_BS1=CAN_BS1_5tq;
	CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;
	/* CAN Baudrate = 1 MBps (1MBps is stm32 CAN max Baudrate) 45/(1+5+3)/5=1 Mbps*/
	CAN_InitStructure.CAN_Prescaler =5;
	CAN_Init(CANx, &CAN_InitStructure);
}

void CAN_Filter_Config(void)
{
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
	/*CAN Filter init*/
	CAN_FilterInitStructure.CAN_FilterNumber=0; //filter group 0
	//work in mask mode
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
	//filter length
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
	/**/
	CAN_FilterInitStructure.CAN_FilterIdHigh= ((((u32)0x1314<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF0000)>>16;
	CAN_FilterInitStructure.CAN_FilterIdLow= (((u32)0x1314<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh= 0xFFFF;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow= 0xFFFF;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0 ;
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	
	CAN_FilterInit(&CAN_FilterInitStructure);
	CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);
}

void CAN_NVIC_Config(void) 
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = CAN_RX_IRQ; //CAN RX Interrupt
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void CAN_SetMsg(CanTxMsg *TxMessage)
{
	uint8_t ubCounter = 0;
	//TxMessage.StdId=0x00;
	TxMessage->ExtId=0x1314;
	TxMessage->IDE=CAN_ID_EXT;
	TxMessage->RTR=CAN_RTR_DATA;
	TxMessage->DLC=8;
	for (ubCounter = 0; ubCounter < 8; ubCounter++)
	{
		TxMessage->Data[ubCounter] = ubCounter;
	}
}	

void CAN_Config(void) 
{
	CAN_GPIO_Config();
	CAN_Mode_Config();
	CAN_Filter_Config();
	CAN_NVIC_Config();
}