#include "FST_Stepper_Library.h"

extern u8 i;
extern Ablauf_t Maschine_Status;
extern char text2[Temp_Text_Length];//temp char 
Stepper_Property Stepper[3];
Timer_List Stepper_Timer[4];
uint8_t EncoderPos_Motor1[4]={3,2,0,1};
uint8_t EncoderNeg_Motor1[4]={2,3,1,0};
extern void _delay_ms(int Time);
extern volatile int32_t MotorRealPosition;
u8 i;//因为是在每次中断里取反，所以必须等到第二次取反后才去计算下一步的定时器比较值；
speedRampData srd;
extern uint8_t EncoderSoftwareFilter(uint8_t MotorNum,uint8_t EncoderStatus);

void EXTI3_IRQHandler(void)
{
	//Encoder PB3
	uint8_t MotorIndex=1;
	if(EXTI_GetITStatus(EXTI_Line3)!= RESET)//Encoder PB3
	{
		//Stepper[MotorIndex].NumOfEncoderIRQ++;
//		EXTI_ClearITPendingBit(EXTI_Line3);

		/*X4-Kodierung moglich*/
		if(Encoder2_Status()==1)
		{
			if(Encoder_Status()==0) 
			{
								//_delay_us(EncoderFilterTime);
								if(Encoder_Status()!=0)
								{
									EXTI_ClearITPendingBit(EXTI_Line10);
									return;
								}
				if(EncoderSoftwareFilter(1,1)) Stepper[MotorIndex].EncoderCount++;
				if(Stepper_Richtung_Negativ==Stepper[MotorIndex].Richtung) 
				{
//							Stepper[MotorIndex].NumOfErrorIRQ++;
				}
			}
			else
			{
								//_delay_us(EncoderFilterTime);
								if(Encoder_Status()!=1)
								{
									EXTI_ClearITPendingBit(EXTI_Line10);
									return;
								}
				if(EncoderSoftwareFilter(1,3)) Stepper[MotorIndex].EncoderCount--;
				if(Stepper_Richtung_Positiv==Stepper[MotorIndex].Richtung) 
				{
//							Stepper[MotorIndex].NumOfErrorIRQ++;
				}
			}
		}
		else
		{
			if(Encoder_Status()==1) 
			{
								//_delay_us(EncoderFilterTime);
								if(Encoder_Status()!=1)
								{
									EXTI_ClearITPendingBit(EXTI_Line10);
									return;
								}
				if(EncoderSoftwareFilter(1,2)) Stepper[MotorIndex].EncoderCount++;
				if(Stepper_Richtung_Negativ==Stepper[MotorIndex].Richtung) 
				{
//							Stepper[MotorIndex].NumOfErrorIRQ++;
				}
			}
			else
			{
								//_delay_us(EncoderFilterTime);
								if(Encoder_Status()!=0)
								{
									EXTI_ClearITPendingBit(EXTI_Line10);
									return;
								}
				if(EncoderSoftwareFilter(1,0)) Stepper[MotorIndex].EncoderCount--;
				if(Stepper_Richtung_Positiv==Stepper[MotorIndex].Richtung) 
				{
//							Stepper[MotorIndex].NumOfErrorIRQ++;
				}
			}
		}
		/*X4-Kodierung*/
		
		EXTI_ClearITPendingBit(EXTI_Line10);		
	}
}
void EXTI15_10_IRQHandler(void)
{
	uint8_t MotorIndex=1;
	uint32_t MotorZielAbstand;
	
	
	if(EXTI_GetITStatus(EXTI_Line15) != RESET)//Encoder PA15
	{
		GPIO_ResetBits(Stepper[MotorIndex].PUL_Port,Stepper[MotorIndex].PUL_Pin);
		//Stepper[MotorIndex].NumOfEncoderIRQ++;
			/*X2-Kodierung*/
			if(Encoder_Status()==1)
			{ 
				if(Encoder2_Status()==1) 
				{
					////_delay_us(EncoderFilterTime);
					if(Encoder2_Status()!=1)
					{
						EXTI_ClearITPendingBit(EXTI_Line11);
						return;
					}
					if(EncoderSoftwareFilter(1,3)) Stepper[MotorIndex].EncoderCount++;
					if(Stepper_Richtung_Negativ==Stepper[MotorIndex].Richtung) 
					{
//									Stepper[MotorIndex].NumOfErrorIRQ++;
//									Stepper[MotorIndex].EncoderCount--;
					}
				}
				else
				{
					//_delay_us(EncoderFilterTime);
					if(Encoder2_Status()!=0)
					{
//									Stepper[MotorIndex].NumOfErrorIRQ++;
						EXTI_ClearITPendingBit(EXTI_Line11);
						return;
					}
					if(EncoderSoftwareFilter(1,2)) Stepper[MotorIndex].EncoderCount--;
					if(Stepper_Richtung_Positiv==Stepper[MotorIndex].Richtung)
					{
//									Stepper[MotorIndex].NumOfErrorIRQ++;
//									Stepper[MotorIndex].EncoderCount++;
					}
				}
			}
			else
			{
				if(Encoder2_Status()==0) 
				{
					//_delay_us(EncoderFilterTime);
					if(Encoder2_Status()!=0)
					{
//									Stepper[MotorIndex].NumOfErrorIRQ++;
						EXTI_ClearITPendingBit(EXTI_Line11);
						return;
					}
					if(EncoderSoftwareFilter(1,0)) Stepper[MotorIndex].EncoderCount++;
					if(Stepper_Richtung_Negativ==Stepper[MotorIndex].Richtung)
					{
//									Stepper[MotorIndex].NumOfErrorIRQ++;
//									Stepper[MotorIndex].EncoderCount--;
					}
				}
				else
				{
					//_delay_us(EncoderFilterTime);
					if(Encoder2_Status()!=1)
					{
						//Stepper[MotorIndex].NumOfErrorIRQ++;
						EXTI_ClearITPendingBit(EXTI_Line11);
						return;
					}
					if(EncoderSoftwareFilter(1,1)) Stepper[MotorIndex].EncoderCount--;
					if(Stepper_Richtung_Positiv==Stepper[MotorIndex].Richtung)
					{
//									Stepper[MotorIndex].NumOfErrorIRQ++;
//									Stepper[MotorIndex].EncoderCount++;
					}
				}
			}
					
			EXTI_ClearITPendingBit(EXTI_Line15);
		}
	if(EXTI_GetITStatus(EXTI_Line12) != RESET)//PA12 w5500 interrupt
	{
//		network_busy=1;
//		//FST_ILI9341_PrintText(1,1,"w5500 interupt", &FST_Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_MAGENTA);
//		_delay_ms(1000);
//		//FST_ILI9341_ClearText(1,1,29, &FST_Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_MAGENTA);
		
		EXTI_ClearITPendingBit(EXTI_Line12);		
	}
}
void TIM2_IRQHandler(void)
{
	uint8_t MotorIndex=Stepper_Timer[2].MotorIndex;
	uint8_t TimerIndex=2;
	TIM_TypeDef *Timer=TIM2;
	int16_t ReglerVelocity;
	char testtext[10];
	
	/*Motor Linear Control*/
//	// Holds next delay period.
//  uint16_t new_step_delay;
//  // Remember the last step delay used when accelrating.
//  static uint16_t last_accel_delay;
//  // Counting steps when moving.
//  static uint16_t step_count = 0;
//  // Keep track of remainder from new_step-delay calculation to incrase accurancy
//  static uint16_t rest = 0;
	/*Motor Linear Control*/
//	//FST_ILI9341_PrintText(1,1,"b", &FST_Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_MAGENTA);
	
	if (TIM_GetITStatus(Timer, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(Timer, TIM_IT_Update);
		if(Stepper[MotorIndex].Laufmode!=Stepper_Mode_MoveAbsolutLC)
		{
			if(1==Stepper_Timer[TimerIndex].Bit_HighLevel)
			{
				Stepper_Timer[TimerIndex].Bit_HighLevel=0;
				GPIO_SetBits(Stepper[MotorIndex].PUL_Port,Stepper[MotorIndex].PUL_Pin);
				if(Stepper_Richtung_Positiv==Stepper[MotorIndex].Richtung)	Stepper[MotorIndex].GesamtSchritte=Stepper[MotorIndex].GesamtSchritte+Stepper[MotorIndex].DrehenAbstand*Stepper[MotorIndex].PulsesProMM;
				else Stepper[MotorIndex].GesamtSchritte=Stepper[MotorIndex].GesamtSchritte-Stepper[MotorIndex].DrehenAbstand*Stepper[MotorIndex].PulsesProMM;
			}
			else
			{
				Stepper_Timer[TimerIndex].Bit_HighLevel=1;
				GPIO_ResetBits(Stepper[MotorIndex].PUL_Port,Stepper[MotorIndex].PUL_Pin);
			}
		}
		switch(Stepper[MotorIndex].Laufmode)
		{
			case Stepper_Mode_Null:
				break;
			case Stepper_Mode_Referenzfahrt://motor fahrt in richtung referenzschalter
//				Stepper_DisableGrenzenSchutz();
				if(1==GPIO_ReadInputDataBit(Stepper[MotorIndex].Referenzschalter_Port,Stepper[MotorIndex].Referenzschalter_Pin))
				{
						Stepper[MotorIndex].Laufmode=Stepper_Mode_ReferenzfahrtRuck;
						if(Stepper_Richtung_Positiv==Stepper[MotorIndex].Richtung)
						{
							Stepper_SetNegativRichtung(MotorIndex);
						}
						else
						{
							Stepper_SetPositivRichtung(MotorIndex);
						}
				}
				break;
			case Stepper_Mode_ReferenzfahrtRuck:// nach referenzschalter motor fahrt in entgegende richtung bis schalter nicht schaltet
				if(0==GPIO_ReadInputDataBit(Stepper[MotorIndex].Referenzschalter_Port,Stepper[MotorIndex].Referenzschalter_Pin))
				{
						/*Schritte Motor Treiber LED High lassen*/
						//GPIO_ResetBits(Stepper[MotorIndex].PUL_Port,Stepper[MotorIndex].PUL_Pin);
						Stepper[MotorIndex].Laufmode=Stepper_Mode_Null;
						Stepper[MotorIndex].Status=Stepper_Status_ReferenzfahrtFertig;
						Stepper[MotorIndex].GesamtSchritte=0;
						Stepper[MotorIndex].EncoderCount=0;
						TIM_Cmd(Timer, DISABLE);
				}
				break;
			case Stepper_Mode_MoveAbsolut:
				if(Stepper[MotorIndex].VirtualEncoderEnable==1)//encoder not used
				{
					if(Stepper[MotorIndex].ZielSchritte==(int32_t)Stepper[MotorIndex].GesamtSchritte)//Position Erreicht
					{
						TIM_Cmd(Timer, DISABLE);
						/*Schritte Motor Treiber LED High lassen*/
						GPIO_ResetBits(Stepper[MotorIndex].PUL_Port,Stepper[MotorIndex].PUL_Pin);
						Stepper[MotorIndex].Laufmode=Stepper_Mode_Null;
						Stepper[MotorIndex].Status=Stepper_Status_MoveAbsolutFertig;
					}
					break;					
				}
				if(EncoderIsGenau==1)
				{
					if(abs(Stepper[MotorIndex].EncoderCount-Stepper[MotorIndex].ZielSchritte)<Motor_Toleranz)//Position Erreicht
					{
						TIM_Cmd(Stepper[MotorIndex].Timer, DISABLE);
						GPIO_ResetBits(Stepper[MotorIndex].PUL_Port,Stepper[MotorIndex].PUL_Pin);
						Stepper[MotorIndex].Laufmode=Stepper_Mode_Null;
						Stepper[MotorIndex].Status=Stepper_Status_MoveAbsolutFertig;
					}
				}
				else
				{
					if((int32_t)Stepper[MotorIndex].GesamtSchritte==Stepper[MotorIndex].ZielSchritte)//Position Erreicht
					{
						/*Schritte Motor Treiber LED High lassen*/
						TIM_Cmd(Stepper[MotorIndex].Timer, DISABLE);
						GPIO_ResetBits(Stepper[MotorIndex].PUL_Port,Stepper[MotorIndex].PUL_Pin);
						Stepper[MotorIndex].Laufmode=Stepper_Mode_Null;
						Stepper[MotorIndex].Status=Stepper_Status_MoveAbsolutFertig;
						Stepper[MotorIndex].EncoderCount=Stepper[MotorIndex].GesamtSchritte;
					}
					if(abs(Stepper[MotorIndex].GesamtSchritte-Stepper[MotorIndex].EncoderCount)>MotorCollision_Detection_Level)
					{
							TIM_Cmd(Stepper[MotorIndex].Timer, DISABLE);
							GPIO_ResetBits(Stepper[MotorIndex].PUL_Port,Stepper[MotorIndex].PUL_Pin);
							//FST_ILI9341_PrintText(1,20,"Kollision", &FST_Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_MAGENTA);
							_delay_ms(1000);
							//FST_ILI9341_ClearText(1,20,9, &FST_Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_MAGENTA);
							Stepper[MotorIndex].GesamtSchritte=Stepper[MotorIndex].EncoderCount;
							TIM_Cmd(Stepper[MotorIndex].Timer, ENABLE);
					}
				}
				break;
			case Stepper_Mode_MoveAbsolutLC:
				Timer->ARR=Stepper[MotorIndex].speedRampData.step_delay;
				if(Stepper[MotorIndex].speedRampData.run_state)
				{
					if(GPIO_ReadOutputDataBit(Stepper[MotorIndex].PUL_Port, Stepper[MotorIndex].PUL_Pin)==1)
					{
						GPIO_ResetBits(Stepper[MotorIndex].PUL_Port, Stepper[MotorIndex].PUL_Pin);
					}
					else
					{
						GPIO_SetBits(Stepper[MotorIndex].PUL_Port, Stepper[MotorIndex].PUL_Pin);
					}
				}
				Stepper[MotorIndex].i++;
				if(Stepper[MotorIndex].i==2)
				{
					Stepper[MotorIndex].i=0;
					switch(Stepper[MotorIndex].speedRampData.run_state) 
					{
						case STOP:
							////FST_ILI9341_PrintText(1,1,"s",&FST_Font_11x18,ILI9341_COLOR_BLUE,ILI9341_COLOR_WHITE);
							Stepper[MotorIndex].speedRampData.step_count = 0;
							Stepper[MotorIndex].speedRampData.rest = 0;
							// Stop Timer/Counter 1.
							Stepper[MotorIndex].Laufmode=Stepper_Mode_Null;
							Stepper[MotorIndex].Status=Stepper_Mode_MoveAbsolutLCFerig;
							TIM_Cmd(Timer, DISABLE);
							break;

						case ACCEL:
							//sm_driver_StepCounter(srd.dir);
							Stepper[MotorIndex].speedRampData.step_count++;
							Stepper[MotorIndex].speedRampData.accel_count++;
							Stepper[MotorIndex].speedRampData.new_step_delay = Stepper[MotorIndex].speedRampData.step_delay - (((2 * (long)Stepper[MotorIndex].speedRampData.step_delay) + Stepper[MotorIndex].speedRampData.rest)/(4 * Stepper[MotorIndex].speedRampData.accel_count + 1));
							Stepper[MotorIndex].speedRampData.rest = ((2 * (long)Stepper[MotorIndex].speedRampData.step_delay)+Stepper[MotorIndex].speedRampData.rest)%(4 * Stepper[MotorIndex].speedRampData.accel_count + 1);
							// Chech if we should start decelration.
							if(Stepper[MotorIndex].speedRampData.step_count >= Stepper[MotorIndex].speedRampData.decel_start)
							{
								Stepper[MotorIndex].speedRampData.accel_count=Stepper[MotorIndex].speedRampData.decel_val;
								Stepper[MotorIndex].speedRampData.run_state=DECEL;
								////FST_ILI9341_PrintText(1,1,"d",&FST_Font_11x18,ILI9341_COLOR_BLUE,ILI9341_COLOR_WHITE);
							}
							// Chech if we hitted max speed.
							else if(Stepper[MotorIndex].speedRampData.new_step_delay <= Stepper[MotorIndex].speedRampData.min_delay)
							{
								Stepper[MotorIndex].speedRampData.last_accel_delay = Stepper[MotorIndex].speedRampData.new_step_delay;
								Stepper[MotorIndex].speedRampData.new_step_delay = Stepper[MotorIndex].speedRampData.min_delay;
								Stepper[MotorIndex].speedRampData.rest = 0;
								Stepper[MotorIndex].speedRampData.run_state = RUN;
								////FST_ILI9341_PrintText(1,1,"r",&FST_Font_11x18,ILI9341_COLOR_BLUE,ILI9341_COLOR_WHITE);
							}
							break;
						case RUN:
							//sm_driver_StepCounter(srd.dir);
							Stepper[MotorIndex].speedRampData.step_count++;
							Stepper[MotorIndex].speedRampData.new_step_delay = Stepper[MotorIndex].speedRampData.min_delay;
							// Chech if we should start decelration.
							if(Stepper[MotorIndex].speedRampData.step_count >= Stepper[MotorIndex].speedRampData.decel_start)
//							if(Stepper[1].EncoderCount/Stepper[1].PulsesProMM/Stepper[1].DrehenAbstand >= srd.decel_start)
							{
								Stepper[MotorIndex].speedRampData.accel_count = Stepper[MotorIndex].speedRampData.decel_val;
								// Start decelration with same delay as accel ended with.
								Stepper[MotorIndex].speedRampData.new_step_delay = Stepper[MotorIndex].speedRampData.last_accel_delay;
								Stepper[MotorIndex].speedRampData.run_state = DECEL;
//								//FST_ILI9341_PrintText(1,1,"d",&FST_Font_11x18,ILI9341_COLOR_BLUE,ILI9341_COLOR_WHITE);
							}
//							//FST_ILI9341_PrintText(8,1,"run", &FST_Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_MAGENTA);
//							if(abs(Stepper[MotorIndex].EncoderCount-Stepper[MotorIndex].ZielSchritte)<LC1Toleranz)
//							{
//								Stepper[MotorIndex].speedRampData.run_state = STOP;
//							}
							break;

						case DECEL:
							Stepper[MotorIndex].speedRampData.step_count++;
							Stepper[MotorIndex].speedRampData.accel_count++;
							Stepper[MotorIndex].speedRampData.new_step_delay = Stepper[MotorIndex].speedRampData.step_delay - (((2 * (long)Stepper[MotorIndex].speedRampData.step_delay) + Stepper[MotorIndex].speedRampData.rest)/(4 * Stepper[MotorIndex].speedRampData.accel_count + 1));
							Stepper[MotorIndex].speedRampData.rest = ((2 * (long)Stepper[MotorIndex].speedRampData.step_delay)+Stepper[MotorIndex].speedRampData.rest)%(4 * Stepper[MotorIndex].speedRampData.accel_count + 1);
							//Check if we at last step
							break;
					 }
					Stepper[MotorIndex].speedRampData.step_delay = Stepper[MotorIndex].speedRampData.new_step_delay;
//					if(abs(Stepper[MotorIndex].EncoderCount-Stepper[MotorIndex].ZielSchritte)<LC1Toleranz)
					 if(abs(Stepper[MotorIndex].speedRampData.step_count-Stepper[MotorIndex].ZielSchritte)<LC1Toleranz)
//					 if(abs(Stepper[MotorIndex].EncoderCount>=Stepper[MotorIndex].ZielSchritte))
					 
					{
							Stepper[MotorIndex].speedRampData.run_state = STOP;
					}
				
				}
				break;
			case Stepper_Mode_MoveRelativ:
				if(Stepper[MotorIndex].ZielSchritte==(int32_t)Stepper[MotorIndex].GesamtSchritte)//Position Erreicht
				{
					TIM_Cmd(Timer, DISABLE);
					/*Schritte Motor Treiber LED High lassen*/
					GPIO_ResetBits(Stepper[MotorIndex].PUL_Port,Stepper[MotorIndex].PUL_Pin);
					Stepper[MotorIndex].Laufmode=Stepper_Mode_Null;
					Stepper[MotorIndex].Status=Stepper_Status_MoveRelativFertig;
				}
//				if(abs(Stepper[MotorIndex].GesamtSchritte-Stepper[MotorIndex].EncoderCount)>MotorCollision_Detection_Level)
//				{
//						Stepper[MotorIndex].GesamtSchritte=Stepper[MotorIndex].EncoderCount;
//				}
				break;
			case Stepper_Mode_MoveAbsolutPID:
				Stepper_Timer[TimerIndex].PIDRegler_Beat++;
				//if((Stepper[MotorIndex].ZielSchritte<=Stepper[MotorIndex].GesamtSchritte+PID_Regler_Toleranz)&&(Stepper[MotorIndex].ZielSchritte>=Stepper[MotorIndex].GesamtSchritte-PID_Regler_Toleranz))//Position Erreicht
				if(Stepper[MotorIndex].VirtualEncoderEnable==1)//encoder not used
				{
					if(Stepper[MotorIndex].ZielSchritte==(int32_t)Stepper[MotorIndex].GesamtSchritte)//Position Erreicht
					{
						TIM_Cmd(Timer, DISABLE);
						/*Schritte Motor Treiber LED High lassen*/
						GPIO_ResetBits(Stepper[MotorIndex].PUL_Port,Stepper[MotorIndex].PUL_Pin);
						Stepper[MotorIndex].Laufmode=Stepper_Mode_Null;
						Stepper[MotorIndex].Status=Stepper_Status_MoveAbsolutPIDFertig;
						Stepper_Regler_Reset(MotorIndex);
						break;
					}
					Stepper[MotorIndex].EncoderCount=Stepper[MotorIndex].GesamtSchritte;
				}
				if(EncoderIsGenau==1)
				{
						if(abs(Stepper[MotorIndex].EncoderCount-Stepper[MotorIndex].ZielSchritte)<PID_Regler_Toleranz)
						{
							TIM_Cmd(TIM2, DISABLE);
							/*Schritte Motor Treiber LED High lassen*/
							GPIO_ResetBits(Stepper[MotorIndex].PUL_Port,Stepper[MotorIndex].PUL_Pin);
							//Stepper[MotorIndex].GesamtSchritte=Stepper[MotorIndex].ZielSchritte;
							Stepper[MotorIndex].Laufmode=Stepper_Mode_Null;
							Stepper[MotorIndex].Status=Stepper_Status_MoveAbsolutPIDFertig;
							Stepper_Regler_Reset(MotorIndex);
						}
				}
				else
				{
						if(Stepper[MotorIndex].ZielSchritte==(int32_t)Stepper[MotorIndex].GesamtSchritte)//Position Erreicht
						{
							TIM_Cmd(Stepper[MotorIndex].Timer, DISABLE);
							/*Schritte Motor Treiber LED High lassen*/
							GPIO_ResetBits(Stepper[MotorIndex].PUL_Port,Stepper[MotorIndex].PUL_Pin);
							Stepper[MotorIndex].Laufmode=Stepper_Mode_Null;
							Stepper[MotorIndex].Status=Stepper_Status_MoveAbsolutPIDFertig;
						}
						if(abs(Stepper[MotorIndex].GesamtSchritte-Stepper[MotorIndex].EncoderCount)>MotorCollision_Detection_Level)
						{
							TIM_Cmd(Stepper[MotorIndex].Timer, DISABLE);
							GPIO_ResetBits(Stepper[MotorIndex].PUL_Port,Stepper[MotorIndex].PUL_Pin);
							//FST_ILI9341_PrintText(1,20,"Kollision", &FST_Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_MAGENTA);
							_delay_ms(1000);
							//FST_ILI9341_ClearText(1,20,9, &FST_Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_MAGENTA);
							Stepper[MotorIndex].GesamtSchritte=Stepper[MotorIndex].EncoderCount;
							TIM_Cmd(Stepper[MotorIndex].Timer, ENABLE);
						}
					
				}
				if(Stepper_Timer[TimerIndex].PIDRegler_Beat>=Stepper_Timer[TimerIndex].PIDRegler_Schritte)//PID Regler
				{
					ReglerVelocity=Stepper_GetPIDReglerOutput(MotorIndex);
					Stepper_Timer[TimerIndex].PIDRegler_Beat=0;
					Stepper_Timer[TimerIndex].PIDRegler_Schritte=Stepper[MotorIndex].Regler_AbtastZeit*ReglerVelocity/1000/Stepper[MotorIndex].DrehenAbstand;
					//Entscheiden Richtung
					if(Stepper_Richtung_Positiv==Stepper[MotorIndex].Richtung)
					{
						if(ReglerVelocity<=0)//Richtung change
						{
							ReglerVelocity=-1*ReglerVelocity;
							Stepper_SetNegativRichtung(MotorIndex);														
						}
					}
					else
					{
						if(ReglerVelocity<0)//Richtung change
						{
							ReglerVelocity=-1*ReglerVelocity;			
						}
						else//Richtung change
						{
							Stepper_SetPositivRichtung(MotorIndex);							
						}
					}
					//TIM3->ARR = (Stepper[MotorIndex].MCU_ArbeitsFrequenz/Stepper[MotorIndex].TIM_Prescaler)*Stepper[MotorIndex].DrehenAbstand/(2*ReglerVelocity);
					//TIM_Cmd(TIM3, ENABLE);
					Stepper_Timer_Init(MotorIndex, Timer, (Stepper[MotorIndex].MCU_ArbeitsFrequenz/Stepper[MotorIndex].TIM_Prescaler)*Stepper[MotorIndex].DrehenAbstand/(2*ReglerVelocity));
				}
				break;
			default:
				break;
		}
//		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
//	//FST_ILI9341_PrintText(1,1,"c", &FST_Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_MAGENTA);
}

void TIM3_IRQHandler(void)
{
	uint8_t MotorIndex=Stepper_Timer[3].MotorIndex;
	uint8_t TimerIndex=3;
	TIM_TypeDef *Timer=TIM3;
	int16_t ReglerVelocity;
	char testtext[10];
	
	/*Motor Linear Control*/
//	// Holds next delay period.
//  uint16_t new_step_delay;
//  // Remember the last step delay used when accelrating.
//  static uint16_t last_accel_delay;
//  // Counting steps when moving.
//  static uint16_t step_count = 0;
//  // Keep track of remainder from new_step-delay calculation to incrase accurancy
//  static uint16_t rest = 0;
	/*Motor Linear Control*/
//	//FST_ILI9341_PrintText(1,1,"b", &FST_Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_MAGENTA);
	
	if (TIM_GetITStatus(Timer, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(Timer, TIM_IT_Update);
		if(Stepper[MotorIndex].Laufmode!=Stepper_Mode_MoveAbsolutLC)
		{
			if(1==Stepper_Timer[TimerIndex].Bit_HighLevel)
			{
				Stepper_Timer[TimerIndex].Bit_HighLevel=0;
				GPIO_SetBits(Stepper[MotorIndex].PUL_Port,Stepper[MotorIndex].PUL_Pin);
//				if(Stepper[MotorIndex].Laufmode==Stepper_Mode_MoveRelativ)
//				{
					if(Stepper_Richtung_Positiv==Stepper[MotorIndex].Richtung)	Stepper[MotorIndex].GesamtSchritte=Stepper[MotorIndex].GesamtSchritte+Stepper[MotorIndex].DrehenAbstand*Stepper[MotorIndex].PulsesProMM;
					else Stepper[MotorIndex].GesamtSchritte=Stepper[MotorIndex].GesamtSchritte-Stepper[MotorIndex].DrehenAbstand*Stepper[MotorIndex].PulsesProMM;
//				}
			}
			else
			{
				Stepper_Timer[TimerIndex].Bit_HighLevel=1;
				GPIO_ResetBits(Stepper[MotorIndex].PUL_Port,Stepper[MotorIndex].PUL_Pin);
			}
		}
		switch(Stepper[MotorIndex].Laufmode)
		{
			case Stepper_Mode_Null:
				break;
			case Stepper_Mode_Referenzfahrt://motor fahrt in richtung referenzschalter
//				Stepper_DisableGrenzenSchutz();
				if(1==GPIO_ReadInputDataBit(Stepper[MotorIndex].Referenzschalter_Port,Stepper[MotorIndex].Referenzschalter_Pin))
				{
						Stepper[MotorIndex].Laufmode=Stepper_Mode_ReferenzfahrtRuck;
						if(Stepper_Richtung_Positiv==Stepper[MotorIndex].Richtung)
						{
							Stepper_SetNegativRichtung(MotorIndex);
						}
						else
						{
							Stepper_SetPositivRichtung(MotorIndex);
						}
				}
				break;
			case Stepper_Mode_ReferenzfahrtRuck:// nach referenzschalter motor fahrt in entgegende richtung bis schalter nicht schaltet
				if(0==GPIO_ReadInputDataBit(Stepper[MotorIndex].Referenzschalter_Port,Stepper[MotorIndex].Referenzschalter_Pin))
				{
						/*Schritte Motor Treiber LED High lassen*/
						//GPIO_ResetBits(Stepper[MotorIndex].PUL_Port,Stepper[MotorIndex].PUL_Pin);
						Stepper[MotorIndex].Laufmode=Stepper_Mode_Null;
						Stepper[MotorIndex].Status=Stepper_Status_ReferenzfahrtFertig;
						Stepper[MotorIndex].GesamtSchritte=0;
						Stepper[MotorIndex].EncoderCount=0;
						TIM_Cmd(Timer, DISABLE);
				}
				break;
			case Stepper_Mode_MoveAbsolut:
				if(Stepper[MotorIndex].VirtualEncoderEnable==1)//encoder not used
				{
					if(Stepper[MotorIndex].ZielSchritte==(int32_t)Stepper[MotorIndex].GesamtSchritte)//Position Erreicht
					{
						TIM_Cmd(Timer, DISABLE);
						/*Schritte Motor Treiber LED High lassen*/
						GPIO_ResetBits(Stepper[MotorIndex].PUL_Port,Stepper[MotorIndex].PUL_Pin);
						Stepper[MotorIndex].Laufmode=Stepper_Mode_Null;
						Stepper[MotorIndex].Status=Stepper_Status_MoveAbsolutFertig;
					}
					break;					
				}
				if(EncoderIsGenau==1)
				{
					if(abs(Stepper[MotorIndex].EncoderCount-Stepper[MotorIndex].ZielSchritte)<Motor_Toleranz)//Position Erreicht
					{
						TIM_Cmd(Stepper[MotorIndex].Timer, DISABLE);
						GPIO_ResetBits(Stepper[MotorIndex].PUL_Port,Stepper[MotorIndex].PUL_Pin);
						Stepper[MotorIndex].Laufmode=Stepper_Mode_Null;
						Stepper[MotorIndex].Status=Stepper_Status_MoveAbsolutFertig;
					}
				}
				else
				{
					if((int32_t)Stepper[MotorIndex].GesamtSchritte==Stepper[MotorIndex].ZielSchritte)//Position Erreicht
					{
						/*Schritte Motor Treiber LED High lassen*/
						TIM_Cmd(Stepper[MotorIndex].Timer, DISABLE);
						GPIO_ResetBits(Stepper[MotorIndex].PUL_Port,Stepper[MotorIndex].PUL_Pin);
						Stepper[MotorIndex].Laufmode=Stepper_Mode_Null;
						Stepper[MotorIndex].Status=Stepper_Status_MoveAbsolutFertig;
						Stepper[MotorIndex].EncoderCount=Stepper[MotorIndex].GesamtSchritte;
					}
					if(abs(Stepper[MotorIndex].GesamtSchritte-Stepper[MotorIndex].EncoderCount)>MotorCollision_Detection_Level)
					{
							TIM_Cmd(Stepper[MotorIndex].Timer, DISABLE);
							GPIO_ResetBits(Stepper[MotorIndex].PUL_Port,Stepper[MotorIndex].PUL_Pin);
							//FST_ILI9341_PrintText(1,20,"Kollision", &FST_Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_MAGENTA);
							_delay_ms(1000);
							//FST_ILI9341_ClearText(1,20,9, &FST_Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_MAGENTA);
							Stepper[MotorIndex].GesamtSchritte=Stepper[MotorIndex].EncoderCount;
							TIM_Cmd(Stepper[MotorIndex].Timer, ENABLE);
					}
				}
				break;
			case Stepper_Mode_MoveAbsolutLC:
				Timer->ARR=Stepper[MotorIndex].speedRampData.step_delay;
				if(Stepper[MotorIndex].speedRampData.run_state)
				{
					if(GPIO_ReadOutputDataBit(Stepper[MotorIndex].PUL_Port, Stepper[MotorIndex].PUL_Pin)==1)
					{
						GPIO_ResetBits(Stepper[MotorIndex].PUL_Port, Stepper[MotorIndex].PUL_Pin);
					}
					else
					{
						GPIO_SetBits(Stepper[MotorIndex].PUL_Port, Stepper[MotorIndex].PUL_Pin);
					}
				}
				Stepper[MotorIndex].i++;
				if(Stepper[MotorIndex].i==2)
				{
					Stepper[MotorIndex].i=0;
					switch(Stepper[MotorIndex].speedRampData.run_state) 
					{
						case STOP:
							////FST_ILI9341_PrintText(1,1,"s",&FST_Font_11x18,ILI9341_COLOR_BLUE,ILI9341_COLOR_WHITE);
							Stepper[MotorIndex].speedRampData.step_count = 0;
							Stepper[MotorIndex].speedRampData.rest = 0;
							// Stop Timer/Counter 1.
							Stepper[MotorIndex].Laufmode=Stepper_Mode_Null;
							Stepper[MotorIndex].Status=Stepper_Mode_MoveAbsolutLCFerig;
							TIM_Cmd(Timer, DISABLE);
							break;

						case ACCEL:
							//sm_driver_StepCounter(srd.dir);
							Stepper[MotorIndex].speedRampData.step_count++;
							Stepper[MotorIndex].speedRampData.accel_count++;
							Stepper[MotorIndex].speedRampData.new_step_delay = Stepper[MotorIndex].speedRampData.step_delay - (((2 * (long)Stepper[MotorIndex].speedRampData.step_delay) + Stepper[MotorIndex].speedRampData.rest)/(4 * Stepper[MotorIndex].speedRampData.accel_count + 1));
							Stepper[MotorIndex].speedRampData.rest = ((2 * (long)Stepper[MotorIndex].speedRampData.step_delay)+Stepper[MotorIndex].speedRampData.rest)%(4 * Stepper[MotorIndex].speedRampData.accel_count + 1);
							// Chech if we should start decelration.
							if(Stepper[MotorIndex].speedRampData.step_count >= Stepper[MotorIndex].speedRampData.decel_start)
							{
								Stepper[MotorIndex].speedRampData.accel_count=Stepper[MotorIndex].speedRampData.decel_val;
								Stepper[MotorIndex].speedRampData.run_state=DECEL;
								////FST_ILI9341_PrintText(1,1,"d",&FST_Font_11x18,ILI9341_COLOR_BLUE,ILI9341_COLOR_WHITE);
							}
							// Chech if we hitted max speed.
							else if(Stepper[MotorIndex].speedRampData.new_step_delay <= Stepper[MotorIndex].speedRampData.min_delay)
							{
								Stepper[MotorIndex].speedRampData.last_accel_delay = Stepper[MotorIndex].speedRampData.new_step_delay;
								Stepper[MotorIndex].speedRampData.new_step_delay = Stepper[MotorIndex].speedRampData.min_delay;
								Stepper[MotorIndex].speedRampData.rest = 0;
								Stepper[MotorIndex].speedRampData.run_state = RUN;
								////FST_ILI9341_PrintText(1,1,"r",&FST_Font_11x18,ILI9341_COLOR_BLUE,ILI9341_COLOR_WHITE);
							}
							break;
						case RUN:
							//sm_driver_StepCounter(srd.dir);
							Stepper[MotorIndex].speedRampData.step_count++;
							Stepper[MotorIndex].speedRampData.new_step_delay = Stepper[MotorIndex].speedRampData.min_delay;
							// Chech if we should start decelration.
							if(Stepper[MotorIndex].speedRampData.step_count >= Stepper[MotorIndex].speedRampData.decel_start)
//							if(Stepper[1].EncoderCount/Stepper[1].PulsesProMM/Stepper[1].DrehenAbstand >= srd.decel_start)
							{
								Stepper[MotorIndex].speedRampData.accel_count = Stepper[MotorIndex].speedRampData.decel_val;
								// Start decelration with same delay as accel ended with.
								Stepper[MotorIndex].speedRampData.new_step_delay = Stepper[MotorIndex].speedRampData.last_accel_delay;
								Stepper[MotorIndex].speedRampData.run_state = DECEL;
//								//FST_ILI9341_PrintText(1,1,"d",&FST_Font_11x18,ILI9341_COLOR_BLUE,ILI9341_COLOR_WHITE);
							}
//							//FST_ILI9341_PrintText(8,1,"run", &FST_Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_MAGENTA);
//							if(abs(Stepper[MotorIndex].EncoderCount-Stepper[MotorIndex].ZielSchritte)<LC1Toleranz)
//							{
//								Stepper[MotorIndex].speedRampData.run_state = STOP;
//							}
							break;

						case DECEL:
							Stepper[MotorIndex].speedRampData.step_count++;
							Stepper[MotorIndex].speedRampData.accel_count++;
							Stepper[MotorIndex].speedRampData.new_step_delay = Stepper[MotorIndex].speedRampData.step_delay - (((2 * (long)Stepper[MotorIndex].speedRampData.step_delay) + Stepper[MotorIndex].speedRampData.rest)/(4 * Stepper[MotorIndex].speedRampData.accel_count + 1));
							Stepper[MotorIndex].speedRampData.rest = ((2 * (long)Stepper[MotorIndex].speedRampData.step_delay)+Stepper[MotorIndex].speedRampData.rest)%(4 * Stepper[MotorIndex].speedRampData.accel_count + 1);
							//Check if we at last step
							break;
					 }
					Stepper[MotorIndex].speedRampData.step_delay = Stepper[MotorIndex].speedRampData.new_step_delay;
//					if(abs(Stepper[MotorIndex].EncoderCount-Stepper[MotorIndex].ZielSchritte)<LC1Toleranz)
					 if(abs(Stepper[MotorIndex].speedRampData.step_count-Stepper[MotorIndex].ZielSchritte)<LC1Toleranz)
//					 if(abs(Stepper[MotorIndex].EncoderCount>=Stepper[MotorIndex].ZielSchritte))
					 
					{
							Stepper[MotorIndex].speedRampData.run_state = STOP;
					}
				
				}
				break;
			case Stepper_Mode_MoveRelativ:
				if(Stepper[MotorIndex].ZielSchritte==(int32_t)Stepper[MotorIndex].GesamtSchritte)//Position Erreicht
				{
					TIM_Cmd(Timer, DISABLE);
					/*Schritte Motor Treiber LED High lassen*/
					GPIO_ResetBits(Stepper[MotorIndex].PUL_Port,Stepper[MotorIndex].PUL_Pin);
					Stepper[MotorIndex].Laufmode=Stepper_Mode_Null;
					Stepper[MotorIndex].Status=Stepper_Status_MoveRelativFertig;
				}
//				if(abs(Stepper[MotorIndex].GesamtSchritte-Stepper[MotorIndex].EncoderCount)>MotorCollision_Detection_Level)
//				{
//						Stepper[MotorIndex].GesamtSchritte=Stepper[MotorIndex].EncoderCount;
//				}
				break;
			case Stepper_Mode_MoveAbsolutPID:
				Stepper_Timer[TimerIndex].PIDRegler_Beat++;
				//if((Stepper[MotorIndex].ZielSchritte<=Stepper[MotorIndex].GesamtSchritte+PID_Regler_Toleranz)&&(Stepper[MotorIndex].ZielSchritte>=Stepper[MotorIndex].GesamtSchritte-PID_Regler_Toleranz))//Position Erreicht
				if(Stepper[MotorIndex].VirtualEncoderEnable==1)//encoder not used
				{
					if(Stepper[MotorIndex].ZielSchritte==(int32_t)Stepper[MotorIndex].GesamtSchritte)//Position Erreicht
					{
						TIM_Cmd(Timer, DISABLE);
						/*Schritte Motor Treiber LED High lassen*/
						GPIO_ResetBits(Stepper[MotorIndex].PUL_Port,Stepper[MotorIndex].PUL_Pin);
						Stepper[MotorIndex].Laufmode=Stepper_Mode_Null;
						Stepper[MotorIndex].Status=Stepper_Status_MoveAbsolutPIDFertig;
						Stepper_Regler_Reset(MotorIndex);
						break;
					}
					Stepper[MotorIndex].EncoderCount=Stepper[MotorIndex].GesamtSchritte;
				}
				if(EncoderIsGenau==1)
				{
						if(abs(Stepper[MotorIndex].EncoderCount-Stepper[MotorIndex].ZielSchritte)<PID_Regler_Toleranz)
						{
							TIM_Cmd(TIM2, DISABLE);
							/*Schritte Motor Treiber LED High lassen*/
							GPIO_ResetBits(Stepper[MotorIndex].PUL_Port,Stepper[MotorIndex].PUL_Pin);
							//Stepper[MotorIndex].GesamtSchritte=Stepper[MotorIndex].ZielSchritte;
							Stepper[MotorIndex].Laufmode=Stepper_Mode_Null;
							Stepper[MotorIndex].Status=Stepper_Status_MoveAbsolutPIDFertig;
							Stepper_Regler_Reset(MotorIndex);
						}
				}
				else
				{
						if(Stepper[MotorIndex].ZielSchritte==(int32_t)Stepper[MotorIndex].GesamtSchritte)//Position Erreicht
						{
							TIM_Cmd(Stepper[MotorIndex].Timer, DISABLE);
							/*Schritte Motor Treiber LED High lassen*/
							GPIO_ResetBits(Stepper[MotorIndex].PUL_Port,Stepper[MotorIndex].PUL_Pin);
							Stepper[MotorIndex].Laufmode=Stepper_Mode_Null;
							Stepper[MotorIndex].Status=Stepper_Status_MoveAbsolutPIDFertig;
						}
						if(abs(Stepper[MotorIndex].GesamtSchritte-Stepper[MotorIndex].EncoderCount)>MotorCollision_Detection_Level)
						{
							TIM_Cmd(Stepper[MotorIndex].Timer, DISABLE);
							GPIO_ResetBits(Stepper[MotorIndex].PUL_Port,Stepper[MotorIndex].PUL_Pin);
							//FST_ILI9341_PrintText(1,20,"Kollision", &FST_Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_MAGENTA);
							_delay_ms(1000);
							//FST_ILI9341_ClearText(1,20,9, &FST_Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_MAGENTA);
							Stepper[MotorIndex].GesamtSchritte=Stepper[MotorIndex].EncoderCount;
							TIM_Cmd(Stepper[MotorIndex].Timer, ENABLE);
						}
					
				}
				if(Stepper_Timer[TimerIndex].PIDRegler_Beat>=Stepper_Timer[TimerIndex].PIDRegler_Schritte)//PID Regler
				{
					ReglerVelocity=Stepper_GetPIDReglerOutput(MotorIndex);
					Stepper_Timer[TimerIndex].PIDRegler_Beat=0;
					Stepper_Timer[TimerIndex].PIDRegler_Schritte=Stepper[MotorIndex].Regler_AbtastZeit*ReglerVelocity/1000/Stepper[MotorIndex].DrehenAbstand;
					//Entscheiden Richtung
					if(Stepper_Richtung_Positiv==Stepper[MotorIndex].Richtung)
					{
						if(ReglerVelocity<=0)//Richtung change
						{
							ReglerVelocity=-1*ReglerVelocity;
							Stepper_SetNegativRichtung(MotorIndex);														
						}
					}
					else
					{
						if(ReglerVelocity<0)//Richtung change
						{
							ReglerVelocity=-1*ReglerVelocity;			
						}
						else//Richtung change
						{
							Stepper_SetPositivRichtung(MotorIndex);							
						}
					}
					//TIM3->ARR = (Stepper[MotorIndex].MCU_ArbeitsFrequenz/Stepper[MotorIndex].TIM_Prescaler)*Stepper[MotorIndex].DrehenAbstand/(2*ReglerVelocity);
					//TIM_Cmd(TIM3, ENABLE);
					Stepper_Timer_Init(MotorIndex, Timer, (Stepper[MotorIndex].MCU_ArbeitsFrequenz/Stepper[MotorIndex].TIM_Prescaler)*Stepper[MotorIndex].DrehenAbstand/(2*ReglerVelocity));
				}
				break;
			default:
				break;
		}
//		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
//	//FST_ILI9341_PrintText(1,1,"c", &FST_Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_MAGENTA);

}

uint8_t	Stepper_Init(uint8_t MotorNum, Richtung_t PositivClockRichtung, uint8_t SubDivision, double SM_Degree, double GetriebeDurchmesser, uint32_t MCU_ArbeitsFrequenz, uint16_t TIM_Prescaler, TIM_TypeDef* Timer)
{
	if(MotorNum>9) return 0;//MotorNum 0-9
	Stepper[MotorNum].PositivClockRichtung=PositivClockRichtung;
	Stepper[MotorNum].SubDivision=SubDivision;
	Stepper[MotorNum].SM_Degree=SM_Degree;
	Stepper[MotorNum].DrehenAbstand=GetriebeDurchmesser*PI/(360/SM_Degree*SubDivision);
	Stepper[MotorNum].MCU_ArbeitsFrequenz=MCU_ArbeitsFrequenz;
	Stepper[MotorNum].TIM_Prescaler=TIM_Prescaler;
	Stepper[MotorNum].Timer=Timer;
	Stepper[MotorNum].Laufmode=Stepper_Mode_Null;
	Stepper[MotorNum].GesamtSchritte=0;
	Stepper[MotorNum].Status=Stepper_Status_Null;
	Stepper[MotorNum].GetriebeDurchmesser=GetriebeDurchmesser;
	return 1;
}
uint8_t Stepper_ShowInfo(uint8_t MotorNum)
{
	char Info[20];
	sprintf(Info,"MotorNum:%3d",MotorNum);
	//FST_ILI9341_PrintText(1,1,Info,&FST_Font_11x18,ILI9341_COLOR_BLUE,ILI9341_COLOR_WHITE);
	sprintf(Info,"StepAngle:%5f",Stepper[MotorNum].SM_Degree);
	//FST_ILI9341_PrintText(2,1,Info,&FST_Font_11x18,ILI9341_COLOR_BLUE,ILI9341_COLOR_WHITE);
	sprintf(Info,"DreAbsatnd:%5f",Stepper[MotorNum].DrehenAbstand);
	//FST_ILI9341_PrintText(3,1,Info,&FST_Font_11x18,ILI9341_COLOR_BLUE,ILI9341_COLOR_WHITE);
	sprintf(Info,"EncoResol:%4d",Stepper[MotorNum].EncoderResolution);
	//FST_ILI9341_PrintText(4,1,Info,&FST_Font_11x18,ILI9341_COLOR_BLUE,ILI9341_COLOR_WHITE);
	sprintf(Info,"PulsesProMM:%5f",Stepper[MotorNum].PulsesProMM);
	//FST_ILI9341_PrintText(5,1,Info,&FST_Font_11x18,ILI9341_COLOR_BLUE,ILI9341_COLOR_WHITE);
	while(1);
}

uint8_t Stepper_AttachReferenzschalter(uint8_t MotorNum, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	if(MotorNum>9) return 0;//MotorNum 0-9
	Stepper[MotorNum].Referenzschalter_Port=GPIOx;
	Stepper[MotorNum].Referenzschalter_Pin=GPIO_Pin;
	return 1;
}

uint8_t Stepper_EnableGrenzenSchutz(void)
{
//	EXTI_InitTypeDef EXTI_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	/*Referenzschalter Grenze PC2*/		
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource2);  // PC2
//  EXTI_InitStructure.EXTI_Line = EXTI_Line2;
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure); 
//	
//	/*NOT AUS PC2*/		      
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn  ;    
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);   

	
}
uint8_t Stepper_DisableGrenzenSchutz(void)
{
//		EXTI_InitTypeDef EXTI_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	/*Referenzschalter Grenze PC2*/		
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource2);  // PC2
//  EXTI_InitStructure.EXTI_Line = EXTI_Line2;
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
//  EXTI_InitStructure.EXTI_LineCmd = DISABLE;
//  EXTI_Init(&EXTI_InitStructure); 
	
//	/*NOT AUS PC2*/		      
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn  ;    
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);  
	
}

uint8_t Stepper_AttachEncoder(uint8_t MotorNum, uint16_t PinNumA, GPIO_TypeDef* PortNumA, uint16_t PinNumB, GPIO_TypeDef* PortNumB, uint16_t Resolution)
{
	if(MotorNum>9) return 0;//MotorNum 0-9
	Stepper[MotorNum].EncoderLineA_Pin=PinNumA;
	Stepper[MotorNum].EncoderLineA_Port=PortNumA;
	Stepper[MotorNum].EncoderLineB_Pin=PinNumB;
	Stepper[MotorNum].EncoderLineB_Port=PortNumB;
	Stepper[MotorNum].PulsesProMM=((double)Resolution)/(PI*Stepper[MotorNum].GetriebeDurchmesser);
	Stepper[MotorNum].EncoderResolution=Resolution;
	Stepper[MotorNum].VirtualEncoderEnable=0;
	return 1;
}
uint8_t Stepper_AttachVirtualEncoder(uint8_t MotorNum,uint16_t Resolution)
{
	if(MotorNum>9) return 0;//MotorNum 0-9
	Stepper[MotorNum].PulsesProMM=((double)Resolution)/(PI*Stepper[MotorNum].GetriebeDurchmesser);
	Stepper[MotorNum].EncoderResolution=Resolution;
	Stepper[MotorNum].VirtualEncoderEnable=1;
	
}

uint8_t Stepper_AttachDriverSignal(uint8_t MotorNum, uint16_t PUL_Pin, GPIO_TypeDef* PUL_Port, uint16_t DIR_Pin, GPIO_TypeDef* DIR_Port)
{
	if(MotorNum>9) return 0;//MotorNum 0-9
	Stepper[MotorNum].PUL_Pin=PUL_Pin;
	Stepper[MotorNum].PUL_Port=PUL_Port;
	Stepper[MotorNum].DIR_Pin=DIR_Pin;
	Stepper[MotorNum].DIR_Port=DIR_Port;
	return 1;	
}

uint8_t Stepper_Referenzfahrt(uint8_t MotorNum, Richtung_t Richtung, uint16_t Velocity)
{
	uint32_t TIM_Period=(Stepper[MotorNum].MCU_ArbeitsFrequenz/Stepper[MotorNum].TIM_Prescaler)*Stepper[MotorNum].DrehenAbstand/(2*Velocity);
	
	//Bremsen_High();
	
	if(TIM_Period>65535) TIM_Period=65530;//Velocity too slow
	if(MotorNum>9) return 0;//MotorNum 0-9
	Stepper[MotorNum].LastEncoderStatus=2*Encoder_Status()+1*Encoder2_Status();
	if(Stepper_Richtung_Positiv==Richtung)//Postiv Richtung
	{
		Stepper_SetPositivRichtung(MotorNum);
	}
	else//Negativ Richtung
	{
		Stepper_SetNegativRichtung(MotorNum);			
	}
	
	if(Stepper_Status_Stop==Stepper[MotorNum].Status) return 1;//motor nicht bewegen lassen
	Stepper[MotorNum].Laufmode=Stepper_Mode_Referenzfahrt;
	Stepper[MotorNum].Status=Stepper_Status_Null;
	Stepper_Timer_Init(MotorNum, Stepper[MotorNum].Timer, TIM_Period);
	
	Stepper_WaitForStatus(MotorNum, Stepper_Status_ReferenzfahrtFertig);
	
	return 1;
}

uint8_t Stepper_SetNullPunkt(uint8_t MotorNum)
{
	Stepper[MotorNum].GesamtSchritte=0;
	Stepper[MotorNum].EncoderCount=0;
}
uint8_t Stepper_MoveAbsolut(int MotorNum, double Abstand, uint16_t Velocity)
{
	uint32_t TIM_Period=(Stepper[MotorNum].MCU_ArbeitsFrequenz/Stepper[MotorNum].TIM_Prescaler)*Stepper[MotorNum].DrehenAbstand/(2*Velocity);
	if(MotorNum>9) return 0;//MotorNum 0-9
	if(Stepper[MotorNum].VirtualEncoderEnable==0)
	{
		//use encoder
		if(EncoderIsGenau)
		{
			Stepper[MotorNum].GesamtSchritte=Stepper[MotorNum].EncoderCount;
		}
	}
	Stepper[MotorNum].ZielSchritte=(uint32_t)(Abstand*Stepper[MotorNum].PulsesProMM);	
	if(TIM_Period>65536) return 0;//Velocity too slow
	/*Richtung Entscheiden*/
	if(Stepper[MotorNum].VirtualEncoderEnable==0)
	{
		//use encoder
		if(Stepper[MotorNum].ZielSchritte==Stepper[MotorNum].EncoderCount)
		{
			Stepper[MotorNum].Status=Stepper_Status_MoveAbsolutFertig;
			return 1;
		}
		if(Stepper[MotorNum].ZielSchritte>Stepper[MotorNum].EncoderCount)//Positiv Richtung
		{
			Stepper_SetPositivRichtung(MotorNum);		
		}
		else//Negativ Richtung
		{
			Stepper_SetNegativRichtung(MotorNum);			
		}
	}
	else
	{
		//not use encoder
		if(Stepper[MotorNum].ZielSchritte==(int32_t)Stepper[MotorNum].GesamtSchritte)
		{
			Stepper[MotorNum].Status=Stepper_Status_MoveAbsolutFertig;
			return 1;
		}
		if(Stepper[MotorNum].ZielSchritte>Stepper[MotorNum].GesamtSchritte)//Positiv Richtung
		{
			Stepper_SetPositivRichtung(MotorNum);		
		}
		else//Negativ Richtung
		{
			Stepper_SetNegativRichtung(MotorNum);			
		}
	}
	if(Stepper_Status_Stop==Stepper[MotorNum].Status) return 1;//motor nicht bewegen lassen
	Stepper_Timer_Init(MotorNum, Stepper[MotorNum].Timer, TIM_Period);
	Stepper[MotorNum].Laufmode=Stepper_Mode_MoveAbsolut;
	Stepper[MotorNum].Status=Stepper_Status_Null;
	
	Stepper_WaitForStatus(MotorNum, Stepper_Status_MoveAbsolutFertig);
	return 1;	
}
uint8_t Stepper_MoveAbsolutWithoutWait(int MotorNum, double Abstand, uint16_t Velocity)
{
	uint32_t TIM_Period=(Stepper[MotorNum].MCU_ArbeitsFrequenz/Stepper[MotorNum].TIM_Prescaler)*Stepper[MotorNum].DrehenAbstand/(2*Velocity);
	if(MotorNum>9) return 0;//MotorNum 0-9
	if(Stepper[MotorNum].VirtualEncoderEnable==0)
	{
		//use encoder
		if(EncoderIsGenau)
		{
			Stepper[MotorNum].GesamtSchritte=Stepper[MotorNum].EncoderCount;
		}
	}
	Stepper[MotorNum].ZielSchritte=(uint32_t)(Abstand*Stepper[MotorNum].PulsesProMM);	
	if(TIM_Period>65536) return 0;//Velocity too slow
	/*Richtung Entscheiden*/
	if(Stepper[MotorNum].VirtualEncoderEnable==0)
	{
		//use encoder
		if(Stepper[MotorNum].ZielSchritte==Stepper[MotorNum].EncoderCount) 
		{
			Stepper[MotorNum].Status=Stepper_Status_MoveAbsolutFertig;
			return 1;
		}
		if(Stepper[MotorNum].ZielSchritte>Stepper[MotorNum].EncoderCount)//Positiv Richtung
		{
			Stepper_SetPositivRichtung(MotorNum);		
		}
		else//Negativ Richtung
		{
			Stepper_SetNegativRichtung(MotorNum);			
		}
	}
	else
	{
		//not use encoder
		if(Stepper[MotorNum].ZielSchritte==(int32_t)Stepper[MotorNum].GesamtSchritte)
		{
			Stepper[MotorNum].Status=Stepper_Status_MoveAbsolutFertig;
			return 1;
		}
		if(Stepper[MotorNum].ZielSchritte>Stepper[MotorNum].GesamtSchritte)//Positiv Richtung
		{
			Stepper_SetPositivRichtung(MotorNum);		
		}
		else//Negativ Richtung
		{
			Stepper_SetNegativRichtung(MotorNum);			
		}
	}
	if(Stepper_Status_Stop==Stepper[MotorNum].Status) return 1;//motor nicht bewegen lassen
	Stepper_Timer_Init(MotorNum, Stepper[MotorNum].Timer, TIM_Period);
	Stepper[MotorNum].Laufmode=Stepper_Mode_MoveAbsolut;
	Stepper[MotorNum].Status=Stepper_Status_Null;
	return 1;
	
}

/**
 * @brief Stepper moves to a positiv position accourding to null Punkt   
 * @param MotorNum: the ID of the Stepper (0-9)
 * @param Abstand: positiv Absatand Einheit:mm
 * @param Velocity: mm/s
 * @retval result: 1:success 0:failure
 */
uint8_t Stepper_MoveAbsolutLC(int MotorNum, double Abstand, uint16_t accel, uint16_t decel, uint16_t speed)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TypeDef* TIMx=Stepper[MotorNum].Timer;
	uint16_t tmpcr1;
	double step;
	double W=2*PI*speed/(Stepper[MotorNum].DrehenAbstand*360/Stepper[MotorNum].SM_Degree*Stepper[MotorNum].SubDivision);
	//! Number of steps before we hit max speed.
  uint32_t max_s_lim;
  //! Number of steps before we must start deceleration (if accel does not hit max speed).
  uint32_t accel_lim;
	if(MotorNum>9) return 0;//MotorNum 0-9
	
	Stepper[MotorNum].GesamtSchritte=Stepper[MotorNum].EncoderCount;
	Stepper[MotorNum].ZielSchritte=(uint32_t)(Abstand*Stepper[MotorNum].PulsesProMM);
	speed=W*100;
	LCStepperTimer_Init(MotorNum);
	if(Stepper[MotorNum].ZielSchritte>Stepper[MotorNum].GesamtSchritte)	
	{
//		GPIO_ResetBits(MOTOR_COTR_PORT, MOTOR_COTR_DIR);
		Stepper_SetPositivRichtung(MotorNum);
		step=(Abstand-Stepper[MotorNum].EncoderCount/Stepper[MotorNum].PulsesProMM)/Stepper[MotorNum].DrehenAbstand;
//		speed_cntr_Move(step, accel, decel, W*100);
	}
	else
	{
//		GPIO_SetBits(MOTOR_COTR_PORT, MOTOR_COTR_DIR);
		Stepper_SetNegativRichtung(MotorNum);
		step=(Stepper[MotorNum].EncoderCount/Stepper[MotorNum].PulsesProMM-Abstand)/Stepper[MotorNum].DrehenAbstand;
//		speed_cntr_Move(step, accel, decel, W*100);		
	}
	/*init*/
	Stepper[MotorNum].speedRampData.accel_count=0;
	Stepper[MotorNum].speedRampData.decel_start=0;
	Stepper[MotorNum].speedRampData.decel_val=0;
	Stepper[MotorNum].speedRampData.dir=0;
	Stepper[MotorNum].speedRampData.last_accel_delay=0;
	Stepper[MotorNum].speedRampData.min_delay=0;
	Stepper[MotorNum].speedRampData.new_step_delay=0;
	Stepper[MotorNum].speedRampData.rest=0;
	Stepper[MotorNum].speedRampData.run_state=0;
	Stepper[MotorNum].speedRampData.step_count=0;
	Stepper[MotorNum].speedRampData.step_delay=0;
	
  // If moving only 1 step.
  if(step == 1)
  {
    // Move one step...
//    srd.accel_count = -1;
		Stepper[MotorNum].speedRampData.accel_count=-1;
    // ...in DECEL state.
    Stepper[MotorNum].speedRampData.run_state = DECEL;
    // Just a short delay so main() can act on 'running'.
    Stepper[MotorNum].speedRampData.step_delay = 1000;
		Stepper[MotorNum].Timer->ARR=10;
		TIM_Cmd(Stepper[MotorNum].Timer, ENABLE);
  }
  // Only move if number of steps to move is not zero.
  else if(step != 0)
  {
    // Refer to documentation for detailed information about these calculations.

    // Set max speed limit, by calc min_delay to use in timer.
    // min_delay = (alpha / tt)/ w
    Stepper[MotorNum].speedRampData.min_delay = A_T_x100 / speed;

    // Set accelration by calc the first (c0) step delay .
    // step_delay = 1/tt * sqrt(2*alpha/accel)
    // step_delay = ( tfreq*0.676/100 )*100 * sqrt( (2*alpha*10000000000) / (accel*100) )/10000
    Stepper[MotorNum].speedRampData.step_delay = ((long)T1_FREQ_148 * sqrt(A_SQ / accel))/100;
    // Find out after how many steps does the speed hit the max speed limit.
    // max_s_lim = speed^2 / (2*alpha*accel)
    max_s_lim = (long)speed*speed/(long)(((long)A_x20000*accel)/100);
    // If we hit max speed limit before 0,5 step it will round to 0.
    // But in practice we need to move atleast 1 step to get any speed at all.
    if(max_s_lim == 0){
      max_s_lim = 1;
    }

    // Find out after how many steps we must start deceleration.
    // n1 = (n1+n2)decel / (accel + decel)
    accel_lim = ((long)step*decel) / (accel+decel);
    // We must accelrate at least 1 step before we can start deceleration.
    if(accel_lim == 0){
      accel_lim = 1;
    }

    // Use the limit we hit first to calc decel.
    if(accel_lim <= max_s_lim){
      Stepper[MotorNum].speedRampData.decel_val = accel_lim - step;
    }
    else{
      Stepper[MotorNum].speedRampData.decel_val =-(s16)(max_s_lim*accel/decel);
    }
    // We must decelrate at least 1 step to stop.
    if(Stepper[MotorNum].speedRampData.decel_val == 0){
      Stepper[MotorNum].speedRampData.decel_val = -1;
    }

    // Find step to start decleration.
		if(step>0)
		{
			Stepper[MotorNum].speedRampData.decel_start = step + Stepper[MotorNum].speedRampData.decel_val;
		}
		else 
		{
			Stepper[MotorNum].speedRampData.decel_start = -1*step + Stepper[MotorNum].speedRampData.decel_val;
		}

    // If the maximum speed is so low that we dont need to go via accelration state.
    if(Stepper[MotorNum].speedRampData.step_delay <= Stepper[MotorNum].speedRampData.min_delay)
	{
      Stepper[MotorNum].speedRampData.step_delay = Stepper[MotorNum].speedRampData.min_delay;
      Stepper[MotorNum].speedRampData.run_state = RUN;
    }
    else{
      Stepper[MotorNum].speedRampData.run_state = ACCEL;
    }

    // Reset counter.
    Stepper[MotorNum].speedRampData.accel_count = 0;
		//    status.running = TRUE;
    //OCR1A = 10;
		Stepper[MotorNum].Timer->ARR=10;
		//TIM3->ARR=10;
    // Set Timer/Counter to divide clock by 8
    //TCCR1B |= ((0<<CS12)|(1<<CS11)|(0<<CS10));
		Stepper[MotorNum].Laufmode=Stepper_Mode_MoveAbsolutLC;
		Stepper[MotorNum].Status=Stepper_Status_Null;
		TIM_Cmd(Stepper[MotorNum].Timer, ENABLE);
  }
	
	while(Stepper[MotorNum].speedRampData.run_state!=0)
	{
		_delay_ms(100);
		if(Stepper_Status_Stop==Stepper[MotorNum].Status) 
		{
			break;
		}
	};
}
uint8_t Stepper_MoveAbsolutLCWithoutWait(int MotorNum, double Abstand, uint16_t accel, uint16_t decel, uint16_t speed)
{
	double step;
	double W=2*PI*speed/(Stepper[MotorNum].DrehenAbstand*360/Stepper[MotorNum].SM_Degree*Stepper[MotorNum].SubDivision);
	//! Number of steps before we hit max speed.
  uint32_t max_s_lim;
  //! Number of steps before we must start deceleration (if accel does not hit max speed).
  uint32_t accel_lim;
	if(MotorNum>9) return 0;//MotorNum 0-9
	Stepper[MotorNum].GesamtSchritte=Stepper[MotorNum].EncoderCount;
	Stepper[MotorNum].ZielSchritte=(uint32_t)(Abstand*Stepper[MotorNum].PulsesProMM);
	speed=W*100;
	LCStepperTimer_Init(MotorNum);
	if(Stepper[MotorNum].ZielSchritte>Stepper[MotorNum].GesamtSchritte)	
	{
//		GPIO_ResetBits(MOTOR_COTR_PORT, MOTOR_COTR_DIR);
		Stepper_SetPositivRichtung(MotorNum);
		step=(Abstand-Stepper[MotorNum].EncoderCount/Stepper[MotorNum].PulsesProMM)/Stepper[MotorNum].DrehenAbstand;
//		speed_cntr_Move(step, accel, decel, W*100);
	}
	else
	{
//		GPIO_SetBits(MOTOR_COTR_PORT, MOTOR_COTR_DIR);
		Stepper_SetNegativRichtung(MotorNum);
		step=(Stepper[MotorNum].EncoderCount/Stepper[MotorNum].PulsesProMM-Abstand)/Stepper[MotorNum].DrehenAbstand;
//		speed_cntr_Move(step, accel, decel, W*100);		
	}
  // If moving only 1 step.
  if(step == 1)
  {
    // Move one step...
//    srd.accel_count = -1;
		Stepper[MotorNum].speedRampData.accel_count=-1;
    // ...in DECEL state.
    Stepper[MotorNum].speedRampData.run_state = DECEL;
    // Just a short delay so main() can act on 'running'.
    Stepper[MotorNum].speedRampData.step_delay = 1000;
		Stepper[MotorNum].Timer->ARR=10;
		TIM_Cmd(Stepper[MotorNum].Timer, ENABLE);
  }
  // Only move if number of steps to move is not zero.
  else if(step != 0)
  {
    // Refer to documentation for detailed information about these calculations.

    // Set max speed limit, by calc min_delay to use in timer.
    // min_delay = (alpha / tt)/ w
    Stepper[MotorNum].speedRampData.min_delay = A_T_x100 / speed;

    // Set accelration by calc the first (c0) step delay .
    // step_delay = 1/tt * sqrt(2*alpha/accel)
    // step_delay = ( tfreq*0.676/100 )*100 * sqrt( (2*alpha*10000000000) / (accel*100) )/10000
    Stepper[MotorNum].speedRampData.step_delay = ((long)T1_FREQ_148 * sqrt(A_SQ / accel))/100;
    // Find out after how many steps does the speed hit the max speed limit.
    // max_s_lim = speed^2 / (2*alpha*accel)
    max_s_lim = (long)speed*speed/(long)(((long)A_x20000*accel)/100);
    // If we hit max speed limit before 0,5 step it will round to 0.
    // But in practice we need to move atleast 1 step to get any speed at all.
    if(max_s_lim == 0){
      max_s_lim = 1;
    }

    // Find out after how many steps we must start deceleration.
    // n1 = (n1+n2)decel / (accel + decel)
    accel_lim = ((long)step*decel) / (accel+decel);
    // We must accelrate at least 1 step before we can start deceleration.
    if(accel_lim == 0){
      accel_lim = 1;
    }

    // Use the limit we hit first to calc decel.
    if(accel_lim <= max_s_lim){
      Stepper[MotorNum].speedRampData.decel_val = accel_lim - step;
    }
    else{
      Stepper[MotorNum].speedRampData.decel_val =-(s16)(max_s_lim*accel/decel);
    }
    // We must decelrate at least 1 step to stop.
    if(Stepper[MotorNum].speedRampData.decel_val == 0){
      Stepper[MotorNum].speedRampData.decel_val = -1;
    }

    // Find step to start decleration.
		if(step>0)
		{
			Stepper[MotorNum].speedRampData.decel_start = step + Stepper[MotorNum].speedRampData.decel_val;
		}
		else 
		{
			Stepper[MotorNum].speedRampData.decel_start = -1*step + Stepper[MotorNum].speedRampData.decel_val;
		}

    // If the maximum speed is so low that we dont need to go via accelration state.
    if(Stepper[MotorNum].speedRampData.step_delay <= Stepper[MotorNum].speedRampData.min_delay)
	{
      Stepper[MotorNum].speedRampData.step_delay = Stepper[MotorNum].speedRampData.min_delay;
      Stepper[MotorNum].speedRampData.run_state = RUN;
    }
    else{
      Stepper[MotorNum].speedRampData.run_state = ACCEL;
    }

    // Reset counter.
    Stepper[MotorNum].speedRampData.accel_count = 0;
		//    status.running = TRUE;
    //OCR1A = 10;
		Stepper[MotorNum].Timer->ARR=10;
		//TIM3->ARR=10;
    // Set Timer/Counter to divide clock by 8
    //TCCR1B |= ((0<<CS12)|(1<<CS11)|(0<<CS10));
		Stepper[MotorNum].Laufmode=Stepper_Mode_MoveAbsolutLC;
		Stepper[MotorNum].Status=Stepper_Status_Null;
		TIM_Cmd(Stepper[MotorNum].Timer, ENABLE);
  }
}


uint8_t Stepper_MoveAbsolut_PID(int MotorNum, double Abstand)
{
	uint32_t AnfangsVelocity;
	uint32_t TIM_Period;
	uint8_t TimerIndex;
	if(MotorNum>9) return 0;//MotorNum 0-9
	if(EncoderIsGenau)
	{
		Stepper[MotorNum].GesamtSchritte=Stepper[MotorNum].EncoderCount;
	}
	Stepper[MotorNum].ZielSchritte=(uint32_t)(Abstand*Stepper[MotorNum].PulsesProMM);
	
	AnfangsVelocity=Stepper[MotorNum].Min_Velocity;
	TIM_Period=(Stepper[MotorNum].MCU_ArbeitsFrequenz/Stepper[MotorNum].TIM_Prescaler)*Stepper[MotorNum].DrehenAbstand/(2*AnfangsVelocity);
	if(TIM_Period>65536) return 0;
	
	if(TIM2==Stepper[MotorNum].Timer) TimerIndex=2;
	else if(TIM3==Stepper[MotorNum].Timer) TimerIndex=3;
	else if(TIM4==Stepper[MotorNum].Timer) TimerIndex=4;
	else return 0;
	Stepper_Timer[TimerIndex].PIDRegler_Schritte=Stepper[MotorNum].Regler_AbtastZeit*AnfangsVelocity/1000/Stepper[MotorNum].DrehenAbstand;
	Stepper_Timer[TimerIndex].PIDRegler_Beat=0;
	if(Stepper_Timer[TimerIndex].PIDRegler_Schritte<=1) Stepper_Timer[TimerIndex].PIDRegler_Schritte=1;
	
	
if((Stepper[MotorNum].ZielSchritte<=Stepper[MotorNum].EncoderCount+PID_Regler_Toleranz)&&(Stepper[MotorNum].ZielSchritte>=Stepper[MotorNum].EncoderCount-PID_Regler_Toleranz))//Position Erreicht 
{
	Stepper[MotorNum].Laufmode=Stepper_Mode_MoveAbsolutPID;
	Stepper[MotorNum].Status=Stepper_Status_MoveAbsolutPIDFertig;
	return 1;
}
	if(abs(Stepper[MotorNum].EncoderCount-Stepper[MotorNum].ZielSchritte)<PID_Regler_Toleranz)
	{
		TIM_Cmd(Stepper[MotorNum].Timer, DISABLE);
		/*Schritte Motor Treiber LED High lassen*/
		GPIO_ResetBits(Stepper[MotorNum].PUL_Port,Stepper[MotorNum].PUL_Pin);
		//Stepper[MotorIndex].GesamtSchritte=Stepper[MotorIndex].ZielSchritte;
		Stepper[MotorNum].Laufmode=Stepper_Mode_Null;
		Stepper[MotorNum].Status=Stepper_Status_MoveAbsolutPIDFertig;
		Stepper_Regler_Reset(MotorNum);
	}
	else
	{
		if(Stepper[MotorNum].ZielSchritte>=Stepper[MotorNum].EncoderCount)//Positiv Richtung
		{
			Stepper_SetPositivRichtung(MotorNum);		
		}
		else//Negativ Richtung
		{
			Stepper_SetNegativRichtung(MotorNum);			
		}
//		if(Stepper_Status_Stop==Stepper[MotorNum].Status) return 1;//motor nicht bewegen lassen
		Stepper_Timer_Init(MotorNum, Stepper[MotorNum].Timer, TIM_Period);
		Stepper[MotorNum].Laufmode=Stepper_Mode_MoveAbsolutPID;
		Stepper[MotorNum].Status=Stepper_Status_Null;
		Stepper_WaitForStatus(MotorNum, Stepper_Status_MoveAbsolutPIDFertig);
		return 1;
	}	
}

uint8_t Stepper_MoveRelativ(int MotorNum, double Abstand, uint16_t Velocity)
{
	uint32_t TIM_Period=(Stepper[MotorNum].MCU_ArbeitsFrequenz/Stepper[MotorNum].TIM_Prescaler)*Stepper[MotorNum].DrehenAbstand/(2*Velocity);
	if(MotorNum>9) return 0;//MotorNum 0-9
	if(TIM_Period>65535) TIM_Period=65535;//Velocity too slow
	if(0==Abstand) 
	{
		/*Schritte Motor Treiber LED High lassen*/
		GPIO_ResetBits(Stepper[MotorNum].PUL_Port,Stepper[MotorNum].PUL_Pin);
		Stepper[MotorNum].Laufmode=Stepper_Mode_Null;
		Stepper[MotorNum].Status=Stepper_Status_MoveRelativFertig;
		return 1;
	}
	else if(Abstand>0)//Positiv Richtung
	{
		Stepper[MotorNum].GesamtSchritte=Stepper[MotorNum].EncoderCount;
		Stepper[MotorNum].ZielSchritte=Stepper[MotorNum].EncoderCount+(int32_t)Abstand*Stepper[MotorNum].PulsesProMM;
		Stepper_SetPositivRichtung(MotorNum);	
	}
	else//Negativ Richtung
	{
		Stepper[MotorNum].GesamtSchritte=Stepper[MotorNum].EncoderCount;
		Stepper[MotorNum].ZielSchritte=Stepper[MotorNum].EncoderCount+(int32_t)(Abstand*Stepper[MotorNum].PulsesProMM);
		//if(Stepper[MotorNum].ZielSchritte<0) Stepper[MotorNum].ZielSchritte=0;
		Stepper_SetNegativRichtung(MotorNum);	
	}
	if(Stepper_Status_Stop==Stepper[MotorNum].Status) return 1;//motor nicht bewegen lassen
	Stepper_Timer_Init(MotorNum, Stepper[MotorNum].Timer, TIM_Period);
	Stepper[MotorNum].Laufmode=Stepper_Mode_MoveRelativ;
	Stepper[MotorNum].Status=Stepper_Status_Null;
	Stepper_WaitForStatus(MotorNum, Stepper_Status_MoveRelativFertig);
	return 1;	
}
uint8_t Stepper_MoveRelativWithoutWait(int MotorNum, double Abstand, uint16_t Velocity)
{
		uint32_t TIM_Period=(Stepper[MotorNum].MCU_ArbeitsFrequenz/Stepper[MotorNum].TIM_Prescaler)*Stepper[MotorNum].DrehenAbstand/(2*Velocity);
	if(MotorNum>9) return 0;//MotorNum 0-9
	if(TIM_Period>65535) TIM_Period=65535;//Velocity too slow
	if(0==Abstand) 
	{
		/*Schritte Motor Treiber LED High lassen*/
		GPIO_ResetBits(Stepper[MotorNum].PUL_Port,Stepper[MotorNum].PUL_Pin);
		Stepper[MotorNum].Laufmode=Stepper_Mode_Null;
		Stepper[MotorNum].Status=Stepper_Status_MoveRelativFertig;
		return 1;
	}
	else if(Abstand>0)//Positiv Richtung
	{
		Stepper[MotorNum].GesamtSchritte=Stepper[MotorNum].EncoderCount;
		Stepper[MotorNum].ZielSchritte=Stepper[MotorNum].EncoderCount+(int32_t)Abstand*Stepper[MotorNum].PulsesProMM;
		Stepper_SetPositivRichtung(MotorNum);	
	}
	else//Negativ Richtung
	{
		Stepper[MotorNum].GesamtSchritte=Stepper[MotorNum].EncoderCount;
		Stepper[MotorNum].ZielSchritte=Stepper[MotorNum].EncoderCount+(int32_t)(Abstand*Stepper[MotorNum].PulsesProMM);
		if(Stepper[MotorNum].ZielSchritte<0) Stepper[MotorNum].ZielSchritte=0;
		Stepper_SetNegativRichtung(MotorNum);	
	}
	if(Stepper_Status_Stop==Stepper[MotorNum].Status) return 1;//motor nicht bewegen lassen
	Stepper_Timer_Init(MotorNum, Stepper[MotorNum].Timer, TIM_Period);
	Stepper[MotorNum].Laufmode=Stepper_Mode_MoveRelativ;
	Stepper[MotorNum].Status=Stepper_Status_Null;
//	Stepper_WaitForStatus(MotorNum, Stepper_Status_MoveRelativFertig);
	return 1;	
	
}

uint8_t Stepper_KurveAnfahren(int XAchseMotorNum,int YAchseMotorNum, double XAchseAbstand, double YAchseAbstand, uint16_t XAchseVelocity)
{
	//Trajectorie y=Cx^2
	uint16_t time=0;
	double C=YAchseAbstand/(XAchseAbstand*XAchseAbstand);
//	Stepper_MoveAbsolut(XAchseMotorNum,XAchseAbstand,XAchseVelocity);
	uint32_t TIM_Period=(Stepper[XAchseMotorNum].MCU_ArbeitsFrequenz/Stepper[XAchseMotorNum].TIM_Prescaler)*Stepper[XAchseMotorNum].DrehenAbstand/(2*XAchseVelocity);
	if(XAchseMotorNum>9) return 0;//MotorNum 0-9
	Stepper[XAchseMotorNum].GesamtSchritte=Stepper[XAchseMotorNum].EncoderCount;
	Stepper[XAchseMotorNum].ZielSchritte=(uint32_t)(XAchseAbstand*Stepper[XAchseMotorNum].PulsesProMM);	
	if(TIM_Period>65536) return 0;//Velocity too slow
	/*Richtung Entscheiden*/
	else if(Stepper[XAchseMotorNum].ZielSchritte>Stepper[XAchseMotorNum].EncoderCount)//Positiv Richtung
	{
		Stepper_SetPositivRichtung(XAchseMotorNum);		
	}
	else//Negativ Richtung
	{
		Stepper_SetNegativRichtung(XAchseMotorNum);			
	}
	if(Stepper_Status_Stop==Stepper[XAchseMotorNum].Status) return 1;//motor nicht bewegen lassen
	Stepper_Timer_Init(XAchseMotorNum, Stepper[XAchseMotorNum].Timer, TIM_Period);
	Stepper[XAchseMotorNum].Laufmode=Stepper_Mode_MoveAbsolut;
	Stepper[XAchseMotorNum].Status=Stepper_Status_Null;
	while(Stepper[XAchseMotorNum].Status!=Stepper_Status_MoveAbsolutFertig)
	{
		_delay_ms(6);
		time++;
		Stepper_MoveAbsolut(YAchseMotorNum,YAchseAbstand,2*C*time*XAchseVelocity*XAchseVelocity/100);
	}
	Stepper_MoveAbsolut(YAchseMotorNum,YAchseAbstand,2*C*time*XAchseVelocity*XAchseVelocity/100);
	return 1;
}

uint8_t Stepper_WaitForStatus(uint8_t MotorNum, Status_t Status)
{
	uint8_t 	TimerIndex;
	uint16_t 	StepperVelocity;
	uint16_t  ZielPos=(int16_t)(Stepper[MotorNum].ZielSchritte*Stepper[MotorNum].DrehenAbstand);
	do
	{
		if(Maschine_Status==Ablauf_Stop) break;
		_delay_ms(1);

	}while(Stepper[MotorNum].Status!=Status);
	//if(Stepper_Status_Stop!=Stepper[MotorNum].Status) Stepper[MotorNum].Status=Stepper_Status_Null;
	Stepper[MotorNum].Laufmode=Stepper_Mode_Null;
	return 1;
}

uint8_t Stepper_SetRegler(uint8_t MotorNum, uint32_t Max_Velocity, uint32_t Min_Velocity, uint32_t Max_Beschleunigung, uint32_t Min_Beschleunigung, double Regler_P, double Regler_I, double Regler_D, uint16_t Regler_AbtastZeit)
{
	if(MotorNum>9) return 0;//MotorNum 0-9
	Stepper[MotorNum].Max_Velocity=Max_Velocity;
	Stepper[MotorNum].Min_Velocity=Min_Velocity;
	Stepper[MotorNum].Max_Beschleunigung=Max_Beschleunigung;
	Stepper[MotorNum].Min_Beschleunigung=Min_Beschleunigung;
	Stepper[MotorNum].Regler_P=Regler_P;
	Stepper[MotorNum].Regler_I=Regler_I;
	Stepper[MotorNum].Regler_D=Regler_D;
	Stepper[MotorNum].Regler_Differenz=0;
	Stepper[MotorNum].Regler_Differenz_old=0;
	Stepper[MotorNum].Regler_Differenz_Summe=0;
	Stepper[MotorNum].Regler_AbtastZeit=Regler_AbtastZeit;
	return 1;
}

int16_t Stepper_GetPIDReglerOutput(uint8_t MotorNum)
{
	int16_t SollVelocity;
	int16_t StepperVelocity;
	int16_t VelocityDifferenz;
	uint8_t TimerIndex;
	uint16_t DeltaT=Stepper[MotorNum].Regler_AbtastZeit;
	uint16_t Regler_MaxBeschleunigung;
	uint16_t Regler_MinBeschleunigung;
//	char sollVelocity_text[5];
	if(MotorNum>9) return 0;//MotorNum 0-9
	//Stepper[MotorNum].Regler_Differenz=Stepper[MotorNum].ZielSchritte-Stepper[MotorNum].GesamtSchritte;
	if(EncoderIsGenau!=1) Stepper[MotorNum].EncoderCount=Stepper[MotorNum].GesamtSchritte;
	Stepper[MotorNum].Regler_Differenz=Stepper[MotorNum].ZielSchritte-Stepper[MotorNum].EncoderCount;
	Stepper[MotorNum].Regler_Differenz_Summe+=Stepper[MotorNum].Regler_Differenz;
	SollVelocity=Stepper[MotorNum].Regler_P*Stepper[MotorNum].Regler_Differenz+Stepper[MotorNum].Regler_I*Stepper[MotorNum].Regler_Differenz_Summe*DeltaT/1000+Stepper[MotorNum].Regler_D*(Stepper[MotorNum].Regler_Differenz-Stepper[MotorNum].Regler_Differenz_old)*1000/DeltaT;
//	sprintf(sollVelocity_text,"%4d",SollVelocity);
//	//FST_ILI9341_PrintText(8,1,sollVelocity_text, &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_WHITE);
//	SERVER_SendCommand(1,sollVelocity_text);
	Stepper[MotorNum].Regler_Differenz_old=Stepper[MotorNum].Regler_Differenz;
	Regler_MaxBeschleunigung=Stepper[MotorNum].Max_Beschleunigung*DeltaT/1000;
	Regler_MinBeschleunigung=Stepper[MotorNum].Min_Beschleunigung*DeltaT/1000;
	/*Regler Prinzip: ich beschrankt nur Beschleunigungpahse, Regler beschrankt andere phase*/ 
	//SollVelocity Beschrankung
	if(SollVelocity>=0)//Positiv
	{
		if(SollVelocity>=Stepper[MotorNum].Max_Velocity)
		{
			SollVelocity=Stepper[MotorNum].Max_Velocity;
		}
		else if(SollVelocity<=30)
		{
//			if(Stepper_Richtung_Positiv==Stepper[MotorNum].Richtung) SollVelocity=10;
//			else SollVelocity=-10;
			SollVelocity=20;
		}
		else if(SollVelocity<=Stepper[MotorNum].Min_Velocity)
		{
			SollVelocity=Stepper[MotorNum].Min_Velocity;
		}
	}
	else
	{
		if(SollVelocity<=-1*Stepper[MotorNum].Max_Velocity)
		{
			SollVelocity=-1*Stepper[MotorNum].Max_Velocity;
		}
		else if(SollVelocity>=-40)
		{
//			if(Stepper_Richtung_Negativ==Stepper[MotorNum].Richtung) SollVelocity=-10;
//			else SollVelocity=10;
			SollVelocity=-40;
		}
		else if(SollVelocity>=-1*Stepper[MotorNum].Min_Velocity)
		{
			SollVelocity=-1*Stepper[MotorNum].Min_Velocity;
		}
	}
	


	
	
	if(TIM2==Stepper[MotorNum].Timer) TimerIndex=2;
	else if(TIM3==Stepper[MotorNum].Timer) TimerIndex=3;
	else if(TIM4==Stepper[MotorNum].Timer) TimerIndex=4;
	else return 0;
		
	StepperVelocity=(Stepper[MotorNum].MCU_ArbeitsFrequenz/Stepper[MotorNum].TIM_Prescaler)*Stepper[MotorNum].DrehenAbstand/(2*Stepper_Timer[TimerIndex].TIM_Period);
	if(Stepper_Richtung_Negativ==Stepper[MotorNum].Richtung) StepperVelocity=-1*StepperVelocity;//StepperVelocity ist jetzt signed
	VelocityDifferenz=SollVelocity-StepperVelocity;
	if(StepperVelocity>0)//Positiv Richtung
	{
		if(SollVelocity>StepperVelocity) 
		{	
//			STM32vldiscovery_LEDOff(LED3);
//			STM32vldiscovery_LEDOn(LED4);
			if(VelocityDifferenz>Regler_MaxBeschleunigung)
			{
				SollVelocity=StepperVelocity+Regler_MaxBeschleunigung;
			}
			else if(VelocityDifferenz<Regler_MinBeschleunigung)
			{
				SollVelocity=StepperVelocity+Regler_MinBeschleunigung;				
			}
		}
		else
		{
//			STM32vldiscovery_LEDOff(LED3);
//			STM32vldiscovery_LEDOff(LED4);			
		}
	}
	else//Negativ Richtung
	{
		if(SollVelocity<StepperVelocity)
		{
//			STM32vldiscovery_LEDOff(LED3);
//			STM32vldiscovery_LEDOn(LED4);
			if(VelocityDifferenz<-1*Regler_MaxBeschleunigung)
			{
				SollVelocity=StepperVelocity-Regler_MaxBeschleunigung;
			}
			else if(VelocityDifferenz>-1*Regler_MinBeschleunigung)
			{
				SollVelocity=StepperVelocity-Regler_MinBeschleunigung;				
			}
		}
		else
		{
//			STM32vldiscovery_LEDOff(LED3);
//			STM32vldiscovery_LEDOff(LED4);			
		}
			
	}

	
//	if(SollVelocity>=0&&StepperVelocity>=0)
//	{
//		if(SollVelocity>StepperVelocity) 
//		{
//			STM32vldiscovery_LEDOff(LED3);
//			STM32vldiscovery_LEDOn(LED4);
//		}
//		else if(SollVelocity<StepperVelocity)
//		{
//			STM32vldiscovery_LEDOn(LED3);
//			STM32vldiscovery_LEDOff(LED4);		
//		}
//		else
//		{
//			STM32vldiscovery_LEDOff(LED3);
//			STM32vldiscovery_LEDOff(LED4);		
//		}
//	}
//	else if(SollVelocity<=0&&StepperVelocity<=0)
//	{
//		if(SollVelocity>-1*StepperVelocity) 
//		{
//			STM32vldiscovery_LEDOff(LED3);
//			STM32vldiscovery_LEDOn(LED4);
//		}
//		else if(SollVelocity<-1*StepperVelocity)
//		{
//			STM32vldiscovery_LEDOn(LED3);
//			STM32vldiscovery_LEDOff(LED4);		
//		}
//		else
//		{
//			STM32vldiscovery_LEDOff(LED3);
//			STM32vldiscovery_LEDOff(LED4);		
//		}		
//	}
	
//	if(VelocityDifferenz>=0)
//	{
//		if(StepperVelocity>=0)//Beschleunigung in POS Richtung, SollVelocity Positiv
//		{
//			STM32vldiscovery_LEDOn(LED3);
//			STM32vldiscovery_LEDOn(LED4);
//			if(VelocityDifferenz>=Regler_MaxBeschleunigung) SollVelocity=StepperVelocity+Regler_MaxBeschleunigung;//Maximal Beschleunigung Begrenzung
//			else if(VelocityDifferenz<=Regler_MinBeschleunigung) SollVelocity=StepperVelocity+Regler_MinBeschleunigung;//Minimal Beschleunigung Begrenzung

//			//			if(SollVelocity>=Stepper[MotorNum].Max_Velocity) SollVelocity=Stepper[MotorNum].Max_Velocity;
////			else if(SollVelocity<=Stepper[MotorNum].Min_Velocity) SollVelocity=Stepper[MotorNum].Min_Velocity;
//		}
//		else//bremsen in neg richtung, SollVelocity Positiv oder Negativ
//		{
//			STM32vldiscovery_LEDOff(LED3);
//			STM32vldiscovery_LEDOn(LED4);
//			if(VelocityDifferenz>=Regler_MaxBeschleunigung) SollVelocity=StepperVelocity-Regler_MaxBeschleunigung;//Maximal Beschleunigung Begrenzung
//			else if(VelocityDifferenz<=Regler_MinBeschleunigung) SollVelocity=StepperVelocity-Regler_MinBeschleunigung;//Minimal Beschleunigung Begrenzung
////			if(SollVelocity>=0)//Velocity Begrenzung
////			{
////				if(SollVelocity>=Stepper[MotorNum].Max_Velocity) 
////				{
////					SollVelocity=Stepper[MotorNum].Max_Velocity;
//////					STM32vldiscovery_LEDOff(LED3);
//////          STM32vldiscovery_LEDOn(LED4);
////					
////				}
////				else if(SollVelocity<=Stepper[MotorNum].Min_Velocity) 
////				{
////					SollVelocity=Stepper[MotorNum].Min_Velocity;
//////					STM32vldiscovery_LEDOn(LED3);
//////          STM32vldiscovery_LEDOff(LED4);
////				}
//////				else
//////				{
//////					STM32vldiscovery_LEDOff(LED3);
//////          STM32vldiscovery_LEDOff(LED4);
//////				}
////			}
////			else
////			{
////				if(SollVelocity<=(-1*Stepper[MotorNum].Max_Velocity)) 
////				{
////					SollVelocity=-1*Stepper[MotorNum].Max_Velocity;
//////					STM32vldiscovery_LEDOff(LED3);
//////          STM32vldiscovery_LEDOn(LED4);
////				}
////				else if(SollVelocity>=(-1*Stepper[MotorNum].Min_Velocity)) 
////				{
////					SollVelocity=-1*Stepper[MotorNum].Min_Velocity;
//////					STM32vldiscovery_LEDOn(LED3);
//////          STM32vldiscovery_LEDOff(LED4);
////				}
//////				else
//////				{
//////					STM32vldiscovery_LEDOff(LED3);
//////          STM32vldiscovery_LEDOff(LED4);
//////				}
////			}
//		}
//	}
//	else//VelocityDifferenz negativ
//	{
//		if(StepperVelocity<=0)//Beschleunigung in neg richtung SollVelocity nagativ
//		{
//			STM32vldiscovery_LEDOn(LED3);
//			STM32vldiscovery_LEDOff(LED4);	
//			VelocityDifferenz=-1*VelocityDifferenz;
//			if(VelocityDifferenz>=Regler_MaxBeschleunigung) SollVelocity=StepperVelocity-Regler_MaxBeschleunigung;//Maximal Beschleunigung Begrenzung
//			else if(VelocityDifferenz<=Regler_MinBeschleunigung) SollVelocity=StepperVelocity-Regler_MinBeschleunigung;//Minimal Beschleunigung Begrenzung
////			SollVelocity=StepperVelocity-VelocityDifferenz;//VelocityDifferenz pos, ganze Negativ
//			
////			if(SollVelocity<=(-1*Stepper[MotorNum].Max_Velocity)) SollVelocity=-1*Stepper[MotorNum].Max_Velocity;
////			else if(SollVelocity>=(-1*Stepper[MotorNum].Min_Velocity)) SollVelocity=-1*Stepper[MotorNum].Min_Velocity;
//		}
//		else//bremsen in positiv richtung SollVelocity pos oder neg
//		{			
//			STM32vldiscovery_LEDOff(LED3);
//			STM32vldiscovery_LEDOff(LED4);
//			VelocityDifferenz=-1*VelocityDifferenz;//VelocityDifferenz negativ
//			if(VelocityDifferenz>=Regler_MaxBeschleunigung) VelocityDifferenz=StepperVelocity-Regler_MaxBeschleunigung;//Maximal Beschleunigung Begrenzung
//			else if(VelocityDifferenz<=Regler_MinBeschleunigung) VelocityDifferenz=StepperVelocity-Regler_MinBeschleunigung;//Minimal Beschleunigung Begrenzung
//			
//			
////			SollVelocity=StepperVelocity-VelocityDifferenz;
////			if(SollVelocity>=0)//Velocity Begrenzung
////			{
////				if(SollVelocity>=Stepper[MotorNum].Max_Velocity)
////				{
////					SollVelocity=Stepper[MotorNum].Max_Velocity;
//////					STM32vldiscovery_LEDOff(LED3);
//////          STM32vldiscovery_LEDOn(LED4);
////				}
////				else if(SollVelocity<=Stepper[MotorNum].Min_Velocity) 
////				{
////					SollVelocity=Stepper[MotorNum].Min_Velocity;
//////					STM32vldiscovery_LEDOn(LED3);
//////          STM32vldiscovery_LEDOff(LED4);
////				}
//////				else
//////				{
//////					STM32vldiscovery_LEDOff(LED3);
//////          STM32vldiscovery_LEDOff(LED4);
//////				}
////			}
////			else
////			{
////				if(SollVelocity<(-1*Stepper[MotorNum].Max_Velocity))
////				{
////					SollVelocity=-1*Stepper[MotorNum].Max_Velocity;
//////					STM32vldiscovery_LEDOff(LED3);
//////          STM32vldiscovery_LEDOn(LED4);
////				}
////				else if(SollVelocity>(-1*Stepper[MotorNum].Min_Velocity))
////				{
////					SollVelocity=-1*Stepper[MotorNum].Min_Velocity;
//////					STM32vldiscovery_LEDOn(LED3);
//////          STM32vldiscovery_LEDOff(LED4);
////				}
//////				else
//////				{
//////					STM32vldiscovery_LEDOff(LED3);
//////          STM32vldiscovery_LEDOff(LED4);
//////				}
////			}
//		}
//	}




	return SollVelocity;
}


//System Used Function, not open to User


void Stepper_Timer_Init(uint8_t MotorNum, TIM_TypeDef* Timer, uint16_t TIM_Period)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	NVIC_InitTypeDef NVIC_InitStructure;
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
  /* TIM clock enable */
  if(TIM2==Timer)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
		
		/*Stepper Timer Settings*/
		Stepper_Timer[2].MotorIndex=MotorNum;
		Stepper_Timer[2].TIM_Period=TIM_Period;
		Stepper_Timer[2].Bit_HighLevel=1;
		
		/* Enable the TIM2 gloabal Interrupt */
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}
	else if(TIM3==Timer) 
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		
		/*Stepper Timer Settings*/
		Stepper_Timer[3].MotorIndex=MotorNum;
		Stepper_Timer[3].TIM_Period=TIM_Period;
		Stepper_Timer[3].Bit_HighLevel=1;
		
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}
//	else if(TIM4==Timer)
//	{
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
//		
//		/*Stepper Timer Settings*/
//		Stepper_Timer[4].MotorIndex=MotorNum;
//		Stepper_Timer[4].TIM_Period=TIM_Period;
//		Stepper_Timer[4].Bit_HighLevel=1;
//		
//		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
//		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
//		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//		NVIC_Init(&NVIC_InitStructure);
//	}
//	else if(TIM5==Timer) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
//	else if(TIM6==Timer) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
//	else if(TIM7==Timer) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
 
	/*Reload 自动重装载寄存器周期的值(计数值) */ 
  TIM_TimeBaseStructure.TIM_Period = (TIM_Period - 1);

	/* TIM_Prescaler 累计 TIM_Period个频率后产生一个更新或者中断 */
  /* 这个就是预分频系数*/
  TIM_TimeBaseStructure.TIM_Prescaler = (Stepper[MotorNum].TIM_Prescaler - 1);
  /* Sample Division 采样分频 */
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  /* Count up mode 向上计数模式 */
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	/* init Timer2 初始化定时器2 */
  TIM_TimeBaseInit(Timer, &TIM_TimeBaseStructure);

  /* clear Flag 清除溢出中断标志 */
  TIM_ClearITPendingBit(Timer, TIM_IT_Update);
  /* enable Interupt 溢出中断使能 */
  TIM_ITConfig(Timer, TIM_IT_Update, ENABLE);
  /* enable counter 计数器使能 */
  TIM_Cmd(Timer, ENABLE); 
}

void Stepper_SetPositivRichtung(uint8_t MotorNum)
{
	if(Stepper_Richtung_Positiv==Stepper[MotorNum].PositivClockRichtung)
	{
		GPIO_ResetBits(Stepper[MotorNum].DIR_Port,Stepper[MotorNum].DIR_Pin);						
	}
	else
	{
		GPIO_SetBits(Stepper[MotorNum].DIR_Port,Stepper[MotorNum].DIR_Pin);		
	}
	Stepper[MotorNum].Richtung=Stepper_Richtung_Positiv;
	
}

void Stepper_SetNegativRichtung(uint8_t MotorNum)
{
	if(Stepper_Richtung_Positiv==Stepper[MotorNum].PositivClockRichtung)
	{
		GPIO_SetBits(Stepper[MotorNum].DIR_Port,Stepper[MotorNum].DIR_Pin);						
	}
	else
	{
		GPIO_ResetBits(Stepper[MotorNum].DIR_Port,Stepper[MotorNum].DIR_Pin);		
	}
	Stepper[MotorNum].Richtung=Stepper_Richtung_Negativ;
	
}
void Stepper_Delay(uint32_t i,uint32_t j)
{
	uint32_t l,m;
	for(l=0;l<i;l++)
	{
		for(m=0;m<j;m++);
	}
}
void Stepper_Regler_Reset(uint8_t MotorNum)
{
	Stepper[MotorNum].Regler_Differenz=0;
	Stepper[MotorNum].Regler_Differenz_old=0;
	Stepper[MotorNum].Regler_Differenz_Summe=0;
}

/*! \brief Move the stepper motor a given number of steps.
 *
 *  Makes the stepper motor move the given number of steps.
 *  It accelrate with given accelration up to maximum speed and decelerate
 *  with given deceleration so it stops at the given step.
 *  If accel/decel is to small and steps to move is to few, speed might not
 *  reach the max speed limit before deceleration starts.
 *
 *  \param step  Number of steps to move (pos - CW, neg - CCW).
 *  \param accel  Accelration to use, in 0.01*rad/sec^2.
 *  \param decel  Decelration to use, in 0.01*rad/sec^2.
 *  \param speed  Max speed, in 0.01*rad/sec.
 */


void speed_cntr_Move(signed int step, unsigned int accel, unsigned int decel, unsigned int speed)
{
  //! Number of steps before we hit max speed.
  u32 max_s_lim;
  //! Number of steps before we must start deceleration (if accel does not hit max speed).
  u32 accel_lim;

//  // Set direction from sign on step value.
//   if(step <0)
//    {
//      srd.dir = CCW;
//      GPIO_SetBits(MOTOR_COTR_PORT, MOTOR_COTR_DIR);
//    }
//  else
//   {
//      srd.dir = CW;
//	    GPIO_ResetBits(MOTOR_COTR_PORT, MOTOR_COTR_DIR);
//   }
  // If moving only 1 step.
  if(step == 1)
  {
    // Move one step...
    srd.accel_count = -1;
    // ...in DECEL state.
    srd.run_state = DECEL;
    // Just a short delay so main() can act on 'running'.
    srd.step_delay = 1000;
    TIM3->CCR1=10;
		TIM3->ARR=10;
		TIM_Cmd(TIM3, ENABLE);
  }
  // Only move if number of steps to move is not zero.
  else if(step != 0)
  {
    // Refer to documentation for detailed information about these calculations.

    // Set max speed limit, by calc min_delay to use in timer.
    // min_delay = (alpha / tt)/ w
    srd.min_delay = A_T_x100 / speed;

    // Set accelration by calc the first (c0) step delay .
    // step_delay = 1/tt * sqrt(2*alpha/accel)
    // step_delay = ( tfreq*0.676/100 )*100 * sqrt( (2*alpha*10000000000) / (accel*100) )/10000
    srd.step_delay = ((long)T1_FREQ_148 * sqrt(A_SQ / accel))/100;
    // Find out after how many steps does the speed hit the max speed limit.
    // max_s_lim = speed^2 / (2*alpha*accel)
    max_s_lim = (long)speed*speed/(long)(((long)A_x20000*accel)/100);
    // If we hit max speed limit before 0,5 step it will round to 0.
    // But in practice we need to move atleast 1 step to get any speed at all.
    if(max_s_lim == 0){
      max_s_lim = 1;
    }

    // Find out after how many steps we must start deceleration.
    // n1 = (n1+n2)decel / (accel + decel)
    accel_lim = ((long)step*decel) / (accel+decel);
    // We must accelrate at least 1 step before we can start deceleration.
    if(accel_lim == 0){
      accel_lim = 1;
    }

    // Use the limit we hit first to calc decel.
    if(accel_lim <= max_s_lim){
      srd.decel_val = accel_lim - step;
    }
    else{
      srd.decel_val =-(s16)(max_s_lim*accel/decel);
    }
    // We must decelrate at least 1 step to stop.
    if(srd.decel_val == 0){
      srd.decel_val = -1;
    }

    // Find step to start decleration.
		if(step>0)
		{
			srd.decel_start = step + srd.decel_val;
		}
		else 
		{
			srd.decel_start = -1*step + srd.decel_val;
		}

    // If the maximum speed is so low that we dont need to go via accelration state.
    if(srd.step_delay <= srd.min_delay)
	{
      srd.step_delay = srd.min_delay;
      srd.run_state = RUN;
    }
    else{
      srd.run_state = ACCEL;
    }

    // Reset counter.
    srd.accel_count = 0;
		//    status.running = TRUE;
    //OCR1A = 10;
		TIM3->CCR1=10;
		TIM3->ARR=10;
    // Set Timer/Counter to divide clock by 8
    //TCCR1B |= ((0<<CS12)|(1<<CS11)|(0<<CS10));
		TIM_Cmd(TIM3, ENABLE);
  }
}

/*! \brief Timer/Counter1 Output Compare A Match Interrupt.
 *
 *  Timer/Counter1 Output Compare A Match Interrupt.
 *  Increments/decrements the position of the stepper motor
 *  exept after last position, when it stops.
 *  The \ref step_delay defines the period of this interrupt
 *  and controls the speed of the stepper motor.
 *  A new step delay is calculated to follow wanted speed profile
 *  on basis of accel/decel parameters.
 */

// void TIM3_IRQHandler(void)
//{ 
//  // Holds next delay period.
//  u16 new_step_delay;
//  // Remember the last step delay used when accelrating.
//  static u16 last_accel_delay;
//  // Counting steps when moving.
//  static u16 step_count = 0;
//  // Keep track of remainder from new_step-delay calculation to incrase accurancy
//  static u16 rest = 0;
//  //if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
//	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
//  {
//    /* Clear TIM2 Capture Compare1 interrupt pending bit*/
//    //TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
//		 TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
//  }
//  TIM3->CCR1=srd.step_delay;
//	TIM3->ARR=srd.step_delay;
// // OCR1A = srd.step_delay;	//
//  if(srd.run_state)
//  {
//	  if(GPIO_ReadOutputDataBit(MOTOR_COTR_PORT, MOTOR_COTR_PULSE)==1)
//		{
//		  GPIO_ResetBits(MOTOR_COTR_PORT, MOTOR_COTR_PULSE);
//		}
//	  else
//	  {
//			GPIO_SetBits(MOTOR_COTR_PORT, MOTOR_COTR_PULSE);
//	  }


//	}
//	//PORTD^=BIT(5);		   
//  i++;
//  if(i==2)
//	{i=0;
//  switch(srd.run_state) 
//  {
//    case STOP:
//      step_count = 0;
//      rest = 0;
//      // Stop Timer/Counter 1.
//			TIM_Cmd(TIM3, DISABLE);
//      //TCCR1B &= ~((1<<CS12)|(1<<CS11)|(1<<CS10));
//      //status.running = FALSE;
//      break;

//    case ACCEL:
//      //sm_driver_StepCounter(srd.dir);
//      step_count++;
//      srd.accel_count++;
//      new_step_delay = srd.step_delay - (((2 * (long)srd.step_delay) + rest)/(4 * srd.accel_count + 1));
//      rest = ((2 * (long)srd.step_delay)+rest)%(4 * srd.accel_count + 1);
//      // Chech if we should start decelration.
//      if(step_count >= srd.decel_start)
//			//if(Stepper[1].EncoderCount>=srd.decel_start)
//			{
//        srd.accel_count=srd.decel_val;
//        srd.run_state=DECEL;
//      }
//      // Chech if we hitted max speed.
//      else if(new_step_delay <= srd.min_delay)
//			{
//        last_accel_delay = new_step_delay;
//        new_step_delay = srd.min_delay;
//        rest = 0;
//        srd.run_state = RUN;
//      }
//      break;

//    case RUN:
//      //sm_driver_StepCounter(srd.dir);
//      step_count++;
//      new_step_delay = srd.min_delay;
//      // Chech if we should start decelration.
//      if(step_count >= srd.decel_start)
//			//if(Stepper[1].EncoderCount/Stepper[1].PulsesProMM/Stepper[1].DrehenAbstand >= srd.decel_start)
//	    {
//        srd.accel_count = srd.decel_val;
//        // Start decelration with same delay as accel ended with.
//        new_step_delay = last_accel_delay;
//        srd.run_state = DECEL;
//      }
//      break;

//    case DECEL:
//      //sm_driver_StepCounter(srd.dir);
//      step_count++;
//      srd.accel_count++;
//      new_step_delay = srd.step_delay - (((2 * (long)srd.step_delay) + rest)/(4 * srd.accel_count + 1));
//      rest = ((2 * (long)srd.step_delay)+rest)%(4 * srd.accel_count + 1);
//      //Check if we at last step
//      //if(srd.accel_count >= 0)
//			if((Stepper[1].EncoderCount>Stepper[1].ZielSchritte-2)&&(Stepper[1].EncoderCount<Stepper[1].ZielSchritte+2))
//			{
//        srd.run_state = STOP;
//      }
//      break;
//   }
//		srd.step_delay = new_step_delay;
//  
//  }
//}


/*! \brief Square root routine.
 *
 * sqrt routine 'grupe', from comp.sys.ibm.pc.programmer
 * Subject: Summary: SQRT(int) algorithm (with profiling)
 *    From: warwick@cs.uq.oz.au (Warwick Allison)
 *    Date: Tue Oct 8 09:16:35 1991
 *
 *  \param x  Value to find square root of.
 *  \return  Square root of x.
 */
//static u32 sqrt(u32 x)
//{
//  register u32 xr;  // result register
//  register u32 q2;  // scan-bit register
//  register u8 f;   // flag (one bit)

//  xr = 0;                     // clear result
//  q2 = 0x40000000L;           // higest possible result bit
//  do
//  {
//    if((xr + q2) <= x)
//    {
//      x -= xr + q2;
//      f = 1;                  // set flag
//    }
//    else{
//      f = 0;                  // clear flag
//    }
//    xr >>= 1;
//    if(f){
//      xr += q2;               // test flag
//    }
//  } while(q2 >>= 2);          // shift twice
//  if(xr < x){
//    return xr +1;             // add for rounding
//  }
//  else{
//    return xr;
//	  }
//}

/*! \brief Find minimum value.
 *
 *  Returns the smallest value.
 *
 *  \return  Min(x,y).
 */
unsigned int min(unsigned int x, unsigned int y)
{
  if(x < y)
  {
    return x;
  }
  else
  {
    return y;
  }
}

void LCStepperTimer_Init(uint8_t MotorNum)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	if(Stepper[MotorNum].Timer==TIM2) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	else if(Stepper[MotorNum].Timer==TIM3) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  TIM_TimeBaseStructure.TIM_Period =65535;
  TIM_TimeBaseStructure.TIM_Prescaler =11;    //2 MHz 
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//FST_ILI9341_PrintText(1, 1, "g", &FST_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);

  TIM_TimeBaseInit(Stepper[MotorNum].Timer, &TIM_TimeBaseStructure);
	TIM_ITConfig(Stepper[MotorNum].Timer, TIM_IT_Update, ENABLE);
	//FST_ILI9341_PrintText(1, 1, "h", &FST_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);

  if(Stepper[MotorNum].Timer==TIM2) NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	else if(Stepper[MotorNum].Timer==TIM3) NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);	
	
}
uint8_t EncoderSoftwareFilter(uint8_t MotorNum,uint8_t EncoderStatus)
{
	uint8_t i;
	uint8_t index=5;
	if(1==MotorNum)
	{
		/*check pos dirction*/
		for(i=0;i<4;i++)
		{
			if(Stepper[MotorNum].LastEncoderStatus==EncoderPos_Motor1[i])
			index=i;
		}
//		if(5==index) FST_ILI9341_PrintText(2,1,"Index Wrong", &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
		if(index<=2)
		{
			if(EncoderPos_Motor1[index+1]==EncoderStatus) 
			{
				Stepper[MotorNum].LastEncoderStatus=EncoderStatus;
				return 1;//encoder correct
			}
		}
		else if(EncoderPos_Motor1[0]==EncoderStatus)
		{
				Stepper[MotorNum].LastEncoderStatus=EncoderStatus;
				return 1;//encoder correct
		}
		/*check pos dirction*/
		
		/*check neg dirction*/
		for(i=0;i<4;i++)
		{
			if(Stepper[MotorNum].LastEncoderStatus==EncoderNeg_Motor1[i])
			index=i;
		}
		if(index<=2)
		{
			if(EncoderNeg_Motor1[index+1]==EncoderStatus)
			{
				Stepper[MotorNum].LastEncoderStatus=EncoderStatus;
				return 1;//encoder correct
			}
		}
		else if(EncoderNeg_Motor1[0]==EncoderStatus)
		{
			Stepper[MotorNum].LastEncoderStatus=EncoderStatus;
			return 1;//encoder correct
		}
		
		/*check neg dirction*/
//		EncoderError[EncoderErrorIndex++]=Stepper[MotorNum].EncoderCount;
		//Stepper[MotorNum].NumOfErrorIRQ++;
		return 0;//encoder not correct
				
	}
	
}
