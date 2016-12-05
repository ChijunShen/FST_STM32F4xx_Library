#include "FST_SystemGPIO.h"

void FST_SystemGPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	//Motor1_REF_Port
  GPIO_InitStructure.GPIO_Pin=Motor1_REF_Pin;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz ;	  
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(Motor1_REF_Port, &GPIO_InitStructure);
	
	//DruckerIstBereit
  GPIO_InitStructure.GPIO_Pin=DruckerIstBereit_Pin;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz ;	  
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(DruckerIstBereit_Port, &GPIO_InitStructure);
	
	//DruckerHatAuftrag
	GPIO_InitStructure.GPIO_Pin=DruckerHatAuftrag_Pin;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz ;	  
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(DruckerHatAuftrag_Port, &GPIO_InitStructure);
	
	//DruckerHatLuft
	GPIO_InitStructure.GPIO_Pin=DruckerHatLuft_Pin;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz ;	  
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(DruckerHatLuft_Port, &GPIO_InitStructure);
	
	//MagazinInPosition
	GPIO_InitStructure.GPIO_Pin=MagazinInPosition_Pin;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz ;	  
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(MagazinInPosition_Port, &GPIO_InitStructure);
		
	//SicherTur SicherTur_PIN
	 GPIO_InitStructure.GPIO_Pin=SicherTur_PIN;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz ;	  
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(SicherTur_PORT, &GPIO_InitStructure);
	
}
void FST_SystemGPIO_Motor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//Motor 1 PUL:PF6 DIR:PF11
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
  GPIO_InitStructure.GPIO_Pin=Motor1_PUL_PIN;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz ;	  
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(Motor1_PUL_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=Motor1_DIR_PIN;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz ;	  
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(Motor1_DIR_PORT, &GPIO_InitStructure);
	//Motor 2 PUL:PE5 DIR:PF12
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  GPIO_InitStructure.GPIO_Pin=Motor2_PUL_PIN;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz ;	  
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(Motor2_PUL_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=Motor2_DIR_PIN;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz ;	  
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(Motor2_DIR_PORT, &GPIO_InitStructure);
}
void FST_SystemGPIO_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); 
	
	/*Encoder PA15 PB3*/		      
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn  ;    
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
		
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn  ;    
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);  
}
void FST_SystemGPIO_EXTI_Configuration(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB ,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
//	/*NOT AUS PA3*/		
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource3);  // PA3 
//  EXTI_InitStructure.EXTI_Line = EXTI_Line3;
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure); 
	
	/*Encoder PA15*/		
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, GPIO_PinSource15);  // PA15 good encoder white line
  EXTI_InitStructure.EXTI_Line = EXTI_Line15;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
	
	/*Encoder PB3*/		
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, GPIO_PinSource3);  // PB3 good encoder green line
  EXTI_InitStructure.EXTI_Line = EXTI_Line3;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
	
}

