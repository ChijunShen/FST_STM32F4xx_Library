/******************************************************************************
  * @file    FST System Driver
	* @Typ		 Allgemein
  * @author  Chijun Shen
  * @version V1.0.0
  * @date    11/2016
  * @brief   configuration of the System
  ******************************************************************************/
#ifndef FST_SystemGPIO_H
#define FST_SystemGPIO_H 1
#include "main.h"

typedef enum
{
	Ablauf_Null										=0,
	Ablauf_StartBedingung						,
	Ablauf_Referenzfahrt						,
	Ablauf_Stop											,
}Ablauf_t;


void FST_SystemGPIO_Init(void);
void FST_SystemGPIO_Motor_Init(void);
void FST_SystemGPIO_NVIC_Configuration(void);
void FST_SystemGPIO_EXTI_Configuration(void);

/*position*/
#define SM1_AnfangsPosition										100
#define SM2_AnfangsPosition										200
#define SM1_ApplicationsPosition							400
#define SM2_ApplicationsPosition							300
#define IMSMotor_ApplicationsPosition					300					 
#define SM1_Speed															200
#define SM2_Speed															200
#define IMSMotor_Speed												200

/*Eingangsbyte1 PF 0 1 2 3 4 5 6 7*/
#define DruckerIstBereit_Port									GPIOF
#define DruckerIstBereit_Pin									GPIO_Pin_0
#define DruckerIstBereit_Status()							GPIO_ReadInputDataBit(DruckerIstBereit_Port,DruckerIstBereit_Pin)

#define DruckerHatAuftrag_Port								GPIOF
#define DruckerHatAuftrag_Pin									GPIO_Pin_0
#define DruckerHatAuftrag_Status()						GPIO_ReadInputDataBit(DruckerHatAuftrag_Port,DruckerHatAuftrag_Pin)

#define DruckerHatLuft_Port										GPIOF
#define DruckerHatLuft_Pin										GPIO_Pin_0
#define DruckerHatLuft_Status()								GPIO_ReadInputDataBit(DruckerHatLuft_Port,DruckerHatLuft_Pin)

#define MagazinInPosition_Port								GPIOF
#define MagazinInPosition_Pin									GPIO_Pin_0
#define MagazinInPosition_Status()						GPIO_ReadInputDataBit(MagazinInPosition_Port,MagazinInPosition_Pin)

#define Motor1_REF_Pin												GPIO_Pin_0
#define Motor1_REF_Port												GPIOF
#define Motor1_REF_Status()										GPIO_ReadInputDataBit(Motor1_REF_PORT,Motor1_REF_PIN)

#define SicherTur_PIN													GPIO_Pin_1
#define SicherTur_PORT												GPIOF
#define SicherTur_Status()										GPIO_ReadInputDataBit(SicherTur_PORT,SicherTur_PIN)
//#define IMS_Motor_RF_PIN											GPIO_Pin_0
//#define IMS_Motor_RF_PORT											GPIOF
//#define IMS_Motor_RF_Status()									GPIO_ReadInputDataBit(IMS_Motor_RF_PORT,IMS_Motor_RF_PIN)
//#define IMS_Motor_MV_PIN											GPIO_Pin_0
//#define IMS_Motor_MV_PORT											GPIOF
//#define IMS_Motor_MV_Status()									GPIO_ReadInputDataBit(IMS_Motor_MV_PORT,IMS_Motor_MV_PIN)
//#define Trigger_PORT													GPIOF
//#define Trigger_PIN														GPIO_Pin_0
//#define Trigger_Status()											GPIO_ReadInputDataBit(Trigger_PORT,Trigger_PIN)
//#define Zylinder1_Sensor_PORT									GPIOF
//#define Zylinder1_Sensor_PIN									GPIO_Pin_0
//#define Zylinder1_Sensor_Status()							GPIO_ReadInputDataBit(Zylinder1_Sensor_PORT,Zylinder1_Sensor_PIN)
//#define Etikette_VakumSensor_PORT							GPIOF
//#define Etikette_VakumSensor_PIN							GPIO_Pin_0
//#define Etikette_VakumSensor_Status()					GPIO_ReadInputDataBit(Etikette_VakumSensor_PORT,Etikette_VakumSensor_PIN)



/*Eingangsbyte2 PF 8 9 10 3 PC 2 3 4 5*/
/*Eingangsbyte3 PC 12 PA 10 PC10 PA9 12 8 11 PC9*/
/*Eingangsbyte4 PC8 PG8 7 6 5 4 PB0 1*/

/*Ausgangsbyte1 PE0 1 2 3 4 6 PB9 PG15*/
#define Zylinder1_PORT								GPIOE
#define Zylinder1_PIN									GPIO_Pin_0
#define Zylinder1_High()							GPIO_SetBits(Zylinder1_PORT,Zylinder1_PIN);
#define Zylinder1_Low()								GPIO_ResetBits(Zylinder1_PORT,Zylinder1_PIN);
/*Ausgangsbyte2 PG14 13 11 10 9 PD6 3 2*/
/*Ausgangsbyte3 PG1 PE15 PB10 11 12 13 14 15*/

/*Motor1*/
#define Motor1_PUL_PIN						GPIO_Pin_6
#define Motor1_PUL_PORT						GPIOF	
#define Motor1_DIR_PIN						GPIO_Pin_11
#define Motor1_DIR_PORT						GPIOF	
/*Motor2*/
#define Motor2_PUL_PIN						GPIO_Pin_5
#define Motor2_PUL_PORT						GPIOE	
#define Motor2_DIR_PIN						GPIO_Pin_12
#define Motor2_DIR_PORT						GPIOF	
/*Motor3*/

/*Encoder1*/
#define	 Encoder_PIN							GPIO_Pin_15
#define	 Encoder_PIN_PORT					GPIOA
#define	 Encoder_Status()					GPIO_ReadInputDataBit(Encoder_PIN_PORT,Encoder_PIN)	
#define  Encoder_PIN2							GPIO_Pin_3
#define	 Encoder_PIN2_PORT				GPIOB
#define  Encoder2_Status()				GPIO_ReadInputDataBit(Encoder_PIN2_PORT,Encoder_PIN2)
/*Encoder2*/
/*Encoder3*/

/*KraftSensor*/






#endif
