/**
	 Stepper_Library
	*@treiber	HY-DIV268N-5A
	*@MCU			STM32F4XX
	*@author	Frank Shen
	*@email		shenchijun@gmail.com
	*@version	V1.0
	*@history
						v1.0 every timer controls only one motor TIM2 TIM3 TIM4 controls 3 Motrors
 */
#ifndef	FST_Stepper
#define	FST_Stepper 1
#include "main.h"
//#define PI 3.14159
#define StatusZeigen_Mode 0//used in unit test 1:Aktivieren Status auf LCD zeigen 0:Deaktivieren
#define EncoderIsGenau 1//1 Encoder ist genau 0 Encoder ist nicht genau
#define MotorCollision_Detection_Level 300
#define Motor_Toleranz 10
#define PID_Regler_Toleranz 10
#define LC1Toleranz 10
#define LC2Toleranz 10

/*Linear Control Settings*/
/*! \Brief Frequency of timer3 in [Hz].
 *
 * Modify this according to frequency used. Because of the prescaler setting,
 * the timer1 frequency is the clock frequency divided by 8.
 */
#define T1_FREQ 2000000
#define SPR (200*16) //number of steps per round
// Maths constants. To simplify maths when calculating in speed_cntr_Move().
#define ALPHA (2*3.14159/SPR)                    // 2*pi/spr step angle
#define A_T_x100 ((long)(ALPHA*T1_FREQ*100))     // (ALPHA / T1_FREQ)*100
#define T1_FREQ_148 ((int)((T1_FREQ*0.676)/100)) // divided by 100 and scaled by 0.676
#define A_SQ (long)(ALPHA*2*100000*100000)         // 
#define A_x20000 (int)(ALPHA*20000)              // ALPHA*20000
// Speed ramp states
#define STOP  0
#define ACCEL 1
#define DECEL 2
#define RUN   3
// Direction of stepper motor movement
#define CW  0
#define CCW 1

#define MOTOR_COTR_PORT 		GPIOC
#define MOTOR_COTR_DIR  		GPIO_Pin_6
#define MOTOR_COTR_PULSE 		GPIO_Pin_7
/*Linear Control Settings*/


typedef enum
{
	Stepper_Mode_Null					 = 0,//No Mode choosed
	Stepper_Mode_Referenzfahrt    ,//Referenzfahrt
	Stepper_Mode_ReferenzfahrtRuck,//Referenzfahrt Fertig
	Stepper_Mode_MoveAbsolut			,//Move Absolut
	Stepper_Mode_MoveAbsolutLC		,//Move Absolut mit Linear Control
	Stepper_Mode_MoveAbsolutPID		,//Move Absolut mit PID regler
	Stepper_Mode_MoveRelativ			,//Move Relativ
	Stepper_Mode_Test							,//Nur Zu Test
}LaufMode_t;
typedef enum
{
	Stepper_Status_Null				 						= 0					,//not defined
	Stepper_Status_ReferenzfahrtFertig								,
	Stepper_Status_MoveAbsolutFertig									,
	Stepper_Mode_MoveAbsolutLCFerig										,
	Stepper_Status_MoveAbsolutPIDFertig								,
	Stepper_Status_MoveRelativFertig									,
	Stepper_Status_Stop																,
}Status_t;
typedef enum
{
	Stepper_Richtung_Negativ   = 0,//negativ Richtung
	Stepper_Richtung_Positiv      ,//positiv Richtung
}Richtung_t;

/*Motion Control Structure used in Queue*/
typedef enum
{
	Target_Null				 						= 0					,//not defined
	Target_SM1																,//schrittemotor
	Target_SM2																,
	Target_SM3																,
	Target_SM1SM2															,//sm1 Abstand sm2 Abstand2
	Target_LM1																,//linearmotor
	Target_LM2																,
	Target_IMSMotor														,//IMS Motor
}Target_t;
typedef struct
{
	LaufMode_t Mode;
	double Abstand;
	double Abstand2;
	double Abstand3;
	uint16_t Speed;
	uint16_t Speed2;
	uint16_t Speed3;
	Target_t Target;
}MotionControl_Task;

/*! \brief Holding data used by timer interrupt for speed ramp calculation.
 *
 *  Contains data used by timer interrupt to calculate speed profile.
 *  Data is written to it by move(), when stepper motor is moving (timer
 *  interrupt running) data is read/updated when calculating a new step_delay
 */

typedef struct {
  //! What part of the speed ramp we are in.
  uint8_t run_state ;
  //! Direction stepper motor should move.
  uint8_t dir ;
  //! Peroid of next timer delay. At start this value set the accelration rate.
  uint16_t step_delay;
  //! What step_pos to start decelaration
  uint16_t decel_start;
  //! Sets deceleration rate.
  int16_t decel_val;
  //! Minimum time delay (max speed)
  int16_t min_delay;
  //! Counter used when accelerateing/decelerateing to calculate step_delay.
  int16_t accel_count;
	// Holds next delay period.
  uint16_t new_step_delay;
  // Remember the last step delay used when accelrating.
 	uint16_t last_accel_delay;
  // Counting steps when moving.
  uint16_t step_count;
  // Keep track of remainder from new_step-delay calculation to incrase accurancy
  uint16_t rest;

} speedRampData;




typedef struct{
	/*Basic Settings*/
	uint8_t SubDivision;//subdivision settings in the driver
	double SM_Degree;//step angle of the stepper
	double DrehenAbstand;//how long a step goes
	uint32_t MCU_ArbeitsFrequenz;//MCU Frequenz
	uint16_t TIM_Prescaler;//Prescaler of the timer
	double GetriebeDurchmesser;
	uint32_t EncoderResolution;
	/*Move Settings*/
	double GesamtSchritte;//Steps without Encoder
	int32_t ZielSchritte;//Steps to arrive
	LaufMode_t Laufmode;
	Richtung_t PositivClockRichtung;//Positiv Richtung
	Richtung_t Richtung;
	Status_t Status;
	/*Stepper Dynamik*/
	uint32_t Max_Velocity;//mm/s
	uint32_t Min_Velocity;//mm/s
	uint32_t Max_Beschleunigung;//mm/s^2
	uint32_t Min_Beschleunigung;//mm/s^2
	double	Regler_P;
	double	Regler_I;
	double	Regler_D;
	double  Regler_Differenz;//intern used
	double  Regler_Differenz_old;//intern used
	double  Regler_Differenz_Summe;//intern used
	uint16_t Regler_AbtastZeit;
	/*Driver Settings*/
	/*Driver PUL DIR Signal*/
	uint16_t PUL_Pin;//pulses
	GPIO_TypeDef* PUL_Port;	
	uint16_t DIR_Pin;//directions
	GPIO_TypeDef* DIR_Port;
	TIM_TypeDef* Timer;//which timer used
	/*Referenzschalter*/
	GPIO_TypeDef* Referenzschalter_Port;
	uint16_t Referenzschalter_Pin;
	/*Encoder*/
	GPIO_TypeDef* EncoderLineA_Port;
	uint16_t EncoderLineA_Pin;
	GPIO_TypeDef* EncoderLineB_Port;
	uint16_t EncoderLineB_Pin;
	int32_t	EncoderCount;
	uint8_t	VirtualEncoderEnable;
//	uint16_t NumOfEncoderIRQ;
//	uint16_t NumOfErrorIRQ;
  uint8_t LastEncoderStatus;
	int32_t ThreadholdPos; 
	double	PulsesProMM;
	/*Linear Speed Control*/
	speedRampData speedRampData;
	uint8_t i;
}Stepper_Property;

typedef struct{
	uint8_t MotorIndex;
	uint16_t TIM_Period;
	uint8_t Bit_HighLevel;
	uint16_t PIDRegler_Schritte;//wie viele Schritte eine PID_Regler ausgelost wird
	uint16_t PIDRegler_Beat;
}Timer_List;


/**
 * @brief  
 * @param  
 * @retval result: 1:success 0:failure
 */




/**
 * @brief  attach a referenzschalter to Motor. The port setting should first be done by the user.
 * @param  MotorNum: the ID of the Stepper (0-9)
 * @param  GPIOx: Referenzschalter
 * @param	 GPIO_Pin: Referenzschalter
 * @retval result 1:success 0:failure
 */
uint8_t Stepper_AttachReferenzschalter(uint8_t MotorNum, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

/**
 * @brief  initialize the Stepper
 * @param  MotorNum: the ID of the Stepper (0-9)
 * @param  PositivClockRichtung: Default Positiv Drehenrichtung Stepper_Richtung_Positiv:Clockweise Stepper_Richtung_Negativ:Anticlockweise
 * @param	 SubDivision: Settings in the Steppertreiber 1,2,4,8,16
 * @param  SM_Degree: step angle hier 1.8
 * @param	 GetriebeDurchmesser: Durchmesser von Motor Achsegetriebe
 * @param  MCU_ArbeitsFrequenz: Einheit in HZ
 * @param  TIM_Prescaler
 * @param  Timer: the timer Motor uses
 * @retval result: 1:success 0:failure
 */
uint8_t	Stepper_Init(uint8_t MotorNum, Richtung_t PositivClockRichtung, uint8_t SubDivision, double SM_Degree, double GetriebeDurchmesser, uint32_t MCU_ArbeitsFrequenz, uint16_t TIM_Prescaler, TIM_TypeDef* Timer);

uint8_t Stepper_ShowInfo(uint8_t MotorNum);
uint8_t Stepper_EnableGrenzenSchutz(void);

uint8_t Stepper_DisableGrenzenSchutz(void);


/**
 * @brief attach an encoder to Motor. The port setting should first be done by the user. 
 * @param MotorNum: the ID of the Stepper (0-9)
 * @param PinNumA: Pin of Line A
 * @param PortNumA: Port of Line A
 * @param PinNumB: Pin of Line B
 * @param PortNumB: Port of Line B
 * @param	 Resolution: how many Pulses encoder generated Pro Step(considering x2 x4 Codierung)
 * @retval result: 1:success 0:failure
 */
uint8_t Stepper_AttachEncoder(uint8_t MotorNum, uint16_t PinNumA, GPIO_TypeDef* PortNumA, uint16_t PinNumB, GPIO_TypeDef* PortNumB, uint16_t Resolution);

/**
 * @brief attach an virtual encoder to Motor 
 * @param MotorNum: the ID of the Stepper (0-9)
 * @param	 Resolution: how many Pulses encoder generated Pro Step(considering x2 x4 Codierung)
 * @retval result: 1:success 0:failure
 */
uint8_t Stepper_AttachVirtualEncoder(uint8_t MotorNum,uint16_t Resolution);

/**
 * @brief attach pwm line and dirction line to Motor. The port setting should first be done by the user.
 * @param MotorNum: the ID of the Stepper (0-9)
 * @param PUL_Pin: connect PUL- in Treiber
 * @param PUL_Port
 * @param DIR_Pin: connect DIR- in Treiber
 * @param DIR_Port
 * @retval result: 1:success 0:failure
 */
uint8_t Stepper_AttachDriverSignal(uint8_t MotorNum, uint16_t PUL_Pin, GPIO_TypeDef* PUL_Port, uint16_t DIR_Pin, GPIO_TypeDef* DIR_Port);


/**
 * @brief Referenzfahrt von Motor 
 * @param MotorNum: the ID of the Stepper (0-9)
 * @param Richtung: Stepper_Richtung_Positiv:Positiv Richtung Stepper_Richtung_Negativ:Negativ Richtung
 * @param Velocity: mm/s
 * @retval result: 1:success 0:failure Velocity too slow
 */
uint8_t Stepper_Referenzfahrt(uint8_t MotorNum, Richtung_t Richtung, uint16_t Velocity);

/**
 * @brief set Null Punkt von Motor 
 * @param MotorNum: the ID of the Stepper (0-9)
 * @retval result: 1:success 0:failure
 */
uint8_t Stepper_SetNullPunkt(uint8_t MotorNum);

	
/**
 * @brief set PID Regler in the Motor 
 * @param MotorNum: the ID of the Stepper (0-9)
 * @param Max_Velocity: Einheit mm/s
 * @param Min_Velocity: Einheit mm/s
 * @param Max_Beschleunigung: Einheit mm/s^2
 * @param Min_Beschleunigune: Einheit mm/s^2
 * @param Regler_P Regler_I Regler_D 
 * @param Regler_AbtastZeit: Einheit ms
 * @retval result: 1:success 0:failure
 */
uint8_t Stepper_SetRegler(uint8_t MotorNum, uint32_t Max_Velocity, uint32_t Min_Velocity, uint32_t Max_Beschleunigung, uint32_t Min_Beschleunigung, double Regler_P, double Regler_I, double Regler_D, uint16_t Regler_AbtastZeit);

/**
 * @brief wait Status of the Motor  
 * @param MotorNum: the ID of the Stepper (0-9) 
 * @param Status:
 * @retval result: 1:success 0:failure
 */
uint8_t Stepper_WaitForStatus(uint8_t MotorNum, Status_t Status);

/**
 * @brief Stepper moves to a positiv position accourding to null Punkt   
 * @param MotorNum: the ID of the Stepper (0-9)
 * @param Abstand: positiv Absatand Einheit:mm
 * @param Velocity: mm/s
 * @retval result: 1:success 0:failure
 */
uint8_t Stepper_MoveAbsolut(int MotorNum, double Abstand, uint16_t Velocity);

/**
 * @brief Stepper moves to a positiv position accourding to null Punkt   
 * @param MotorNum: the ID of the Stepper (0-9)
 * @param Abstand: positiv Absatand Einheit:mm
 * @param Velocity: mm/s
 * @retval result: 1:success 0:failure
 */
uint8_t Stepper_MoveAbsolutWithoutWait(int MotorNum, double Abstand, uint16_t Velocity);

/**
 * @brief Stepper moves to a positiv position accourding to null Punkt   
 * @param MotorNum: the ID of the Stepper (0-9)
 * @param Abstand: positiv Absatand Einheit:mm
 * @param Velocity: mm/s
 * @retval result: 1:success 0:failure
 */
uint8_t Stepper_MoveAbsolutLC(int MotorNum, double Abstand, uint16_t accel, uint16_t decel, uint16_t speed);
uint8_t Stepper_MoveAbsolutLCWithoutWait(int MotorNum, double Abstand, uint16_t accel, uint16_t decel, uint16_t speed);

/**
 * @brief Stepper moves to a positiv position accourding to null Punkt with PID. user should use Stepper_SetRegler Function first
 * @param MotorNum: the ID of the Stepper (0-9)
 * @param Abstand: positiv Absatand Einheit:mm
 * @retval result: 1:success 0:failure
 */
uint8_t Stepper_MoveAbsolut_PID(int MotorNum, double Abstand);

/**
 * @brief Stepper moves to a Relativ position accourding to current Punkt   
 * @param MotorNum: the ID of the Stepper (0-9)
 * @param Abstand: wenn Positv in Positiv Richtung; wenn Negativ in Negativ Richtung
 * @param Velocity: mm/s
 * @retval result: 1:success 0:failure
 */
uint8_t Stepper_MoveRelativ(int MotorNum, double Abstand, uint16_t Velocity);
uint8_t Stepper_MoveRelativWithoutWait(int MotorNum, double Abstand, uint16_t Velocity);

/**
 * @brief Stepper moves to a Relativ position accourding to current Punkt with PID. user should use Stepper_SetRegler Function first
 * @param XAchseMotorNum: the ID of the Stepper used as X Achse(0-9)
 * @param YAchseMotorNum: the ID of the Stepper used as Y Achse(0-9)
 * @param XAchseAbstand: wenn Positv in Positiv Richtung; wenn Negativ in Negativ Richtung
 * @param YAchseAbstand: wenn Positv in Positiv Richtung; wenn Negativ in Negativ Richtung
 * @param XAchseVelocity: mm/s
 * @retval result: 1:success 0:failure
 */
uint8_t Stepper_KurveAnfahren(int XAchseMotorNum,int YAchseMotorNum, double XAchseAbstand, double YAchseAbstand, uint16_t XAchseVelocity);

/*Linear speed control*/
void speed_cntr_Move(signed int step, unsigned int accel, unsigned int decel, unsigned int speed);//nou used
//u32 c(u32 x);
/**
 * @brief Timer init used in Motor Linear Control  
 * @param MotorNum: the ID of the Stepper (0-9)
 * @param Abstand: wenn Positv in Positiv Richtung; wenn Negativ in Negativ Richtung
 * @param Velocity: mm/s
 * @retval result: 1:success 0:failure
 */
void LCStepperTimer_Init(uint8_t MotorNum);

//System Used Function, not open to User

/**
 * @brief calculate the soll Velocity using PID Algorithnm 
 * @param MotorNum: the ID of the Stepper (0-9)
 * @retval soll Velocity(Einheit: mm/s)
 */
int16_t Stepper_GetPIDReglerOutput(uint8_t MotorNum);
/**
 * @brief  initialize the timer the motor uses to generate PUL
 * @param  MotorNum: the ID of the Stepper (0-9)
 * @param  Timer: the Timer the Motor uses TIM2-TIM7 mainly TIM2 TIM3 TIM4
 * @param  TIM_Period: timer adds up to (TIM_Period-1) and generates an Interrupt
 * @retval none
 */
void Stepper_Timer_Init(uint8_t MotorNum, TIM_TypeDef* Timer, uint16_t TIM_Period);

/**
 * @brief  set that stepper runs in user-set positiv richtung 
 * @param  MotorNum: the ID of the Stepper (0-9)  
 * @retval none
 */
void Stepper_SetPositivRichtung(uint8_t MotorNum);

/**
 * @brief  set that stepper runs in user-set negativ richtung 
 * @param  MotorNum: the ID of the Stepper (0-9)
 * @retval none
 */
void Stepper_SetNegativRichtung(uint8_t MotorNum);

/**
 * @brief  time delay
 * @param  i: out cycle
 * @param  j: in cycle
 * @retval none
 */
void Stepper_Delay(uint32_t i,uint32_t j);

/**
 * @brief  nach jede Bewegung Parameter in Regler reset
 *	Regler_Differenz	Regler_Differenz_old Regler_Differenz_Summe in Stepper[MotorNum] 0 setzen;
 * @param  MotorNum: the ID of the Stepper (0-9) 
 * @retval none
 */
void Stepper_Regler_Reset(uint8_t MotorNum);

uint8_t EncoderSoftwareFilter(uint8_t MotorNum,uint8_t EncoderStatus);








#endif

