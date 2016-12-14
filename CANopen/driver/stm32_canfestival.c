#include "main.h"
unsigned int TimeCNT=0;//time count
unsigned int NextTime=0;//next touch time count
unsigned int TIMER_MAX_COUNT=70000;//biggest time count
static TIMEVAL last_time_set = TIMEVAL_MAX;//last time count
//Set the next alarm //
void setTimer(TIMEVAL value)
{
	 NextTime=(TimeCNT+value)%TIMER_MAX_COUNT;
}

// Get the elapsed time since the last occured alarm //
TIMEVAL getElapsedTime(void)
{
	int ret=0;
	ret = TimeCNT> last_time_set ? TimeCNT - last_time_set : TimeCNT + TIMER_MAX_COUNT - last_time_set;
	last_time_set = TimeCNT;
	return ret;
}
void timerForCan(void)
{
	TimeCNT++;
	if (TimeCNT>=TIMER_MAX_COUNT)
	{
			TimeCNT=0;
	}
	if (TimeCNT==NextTime)
	{
			TimeDispatch();
	}
}

unsigned char canSend(CAN_PORT notused, Message *m)
{
	uint8_t i;
	CanTxMsg *CAN_TxMessage;
	CAN_TxMessage->StdId=m->cob_id;
	if(m->rtr)
	{
		CAN_TxMessage->RTR=CAN_RTR_REMOTE;
	}
	else
	{
		CAN_TxMessage->RTR=CAN_RTR_DATA;
	}
	CAN_TxMessage->IDE=CAN_ID_STD;
	CAN_TxMessage->DLC=m->len;
	for(i=0;i<m->len;i++)
	{
		CAN_TxMessage->Data[i]=m->data[i];		
	}
	if(CAN_Transmit(CANx, CAN_TxMessage)==CAN_NO_MB)
  {
		return 0xff;
	}
	else
	{
		return 0x00;
	}
}