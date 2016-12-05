#include "FST_Flash_Library.h"


uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_Sector_0; 
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_Sector_1; 
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_Sector_2; 
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_Sector_3; 
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_Sector_4; 
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_Sector_5; 
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_Sector_6; 
  }
  else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
  {
    sector = FLASH_Sector_7; 
  }
  else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
  {
    sector = FLASH_Sector_8; 
  }
  else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
  {
    sector = FLASH_Sector_9; 
  }
  else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
  {
    sector = FLASH_Sector_10; 
  }
	else if((Address < ADDR_FLASH_SECTOR_12) && (Address >= ADDR_FLASH_SECTOR_11))
  {
    sector = FLASH_Sector_11; 
  }
	else if((Address < ADDR_FLASH_SECTOR_13) && (Address >= ADDR_FLASH_SECTOR_12))
  {
    sector = FLASH_Sector_12; 
  }
	else if((Address < ADDR_FLASH_SECTOR_14) && (Address >= ADDR_FLASH_SECTOR_13))
  {
    sector = FLASH_Sector_13; 
  }
	else if((Address < ADDR_FLASH_SECTOR_15) && (Address >= ADDR_FLASH_SECTOR_14))
  {
    sector = FLASH_Sector_14; 
  }
	else if((Address < ADDR_FLASH_SECTOR_16) && (Address >= ADDR_FLASH_SECTOR_15))
  {
    sector = FLASH_Sector_15; 
  }
	else if((Address < ADDR_FLASH_SECTOR_17) && (Address >= ADDR_FLASH_SECTOR_16))
  {
    sector = FLASH_Sector_16; 
  }
	else if((Address < ADDR_FLASH_SECTOR_18) && (Address >= ADDR_FLASH_SECTOR_17))
  {
    sector = FLASH_Sector_17; 
  }
	else if((Address < ADDR_FLASH_SECTOR_19) && (Address >= ADDR_FLASH_SECTOR_18))
  {
    sector = FLASH_Sector_18; 
  }
	else if((Address < ADDR_FLASH_SECTOR_20) && (Address >= ADDR_FLASH_SECTOR_19))
  {
    sector = FLASH_Sector_19; 
  }
	else if((Address < ADDR_FLASH_SECTOR_21) && (Address >= ADDR_FLASH_SECTOR_20))
  {
    sector = FLASH_Sector_20; 
  }
	else if((Address < ADDR_FLASH_SECTOR_22) && (Address >= ADDR_FLASH_SECTOR_21))
  {
    sector = FLASH_Sector_21; 
  }
	else if((Address < ADDR_FLASH_SECTOR_23) && (Address >= ADDR_FLASH_SECTOR_22))
  {
    sector = FLASH_Sector_22; 
  }
  else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11))*/
  {
    sector = FLASH_Sector_23; 
  }
  return sector;
}

void STMF429FLASH_WriteHalfWord(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite)
{
	uint32_t StartSector = GetSector(WriteAddr);
	uint32_t EndSector = GetSector(WriteAddr+NumToWrite-1);
	uint32_t Address;
	uint8_t i; 
	
	taskENTER_CRITICAL();
	FLASH_Unlock(); 
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 

	for (i = StartSector; i <= EndSector; i += 8)  //wipe flash
	{
	/* Device voltage range supposed to be [2.7V to 3.6V], the operation will
		 be done by word */
	if (FLASH_EraseSector(i, VoltageRange_3) != FLASH_COMPLETE)
	{
		while (1)
		{
		}
	}
	}
	
	Address = WriteAddr;
 
  for(i=0;i<NumToWrite;i++)
  {
    if (FLASH_ProgramHalfWord(Address, pBuffer[i]) == FLASH_COMPLETE)
    {
      Address = Address + 2;
    }
    else
    {
      /* Error occurred while writing data in Flash memory.
         User can add here some code to deal with this error */
			TM_DISCO_LedOn(LED_RED);
      while (1)
      {
      }
    }
  }
	FLASH_Lock();
	taskEXIT_CRITICAL();
}
void STMF429FLASH_WriteWord(uint32_t WriteAddr,uint32_t *pBuffer,uint16_t NumToWrite)
{
	uint32_t StartSector = GetSector(WriteAddr);
	uint32_t EndSector = GetSector(WriteAddr+NumToWrite-1);
	uint32_t Address;
	uint8_t i; 
	
	taskENTER_CRITICAL();
	FLASH_Unlock(); 
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 

	for (i = StartSector; i <= EndSector; i += 8)  //wipe flash
	{
		/* Device voltage range supposed to be [2.7V to 3.6V], the operation will
			 be done by word */
		if (FLASH_EraseSector(i, VoltageRange_3) != FLASH_COMPLETE)
		{
			while (1)
			{
			}
		}
	}
	
	Address = WriteAddr;
 
  for(i=0;i<NumToWrite;i++)
  {
    if (FLASH_ProgramWord(Address, pBuffer[i]) == FLASH_COMPLETE)
    {
      Address = Address + 4;
    }
    else
    {
      /* Error occurred while writing data in Flash memory.
         User can add here some code to deal with this error */
			TM_DISCO_LedOn(LED_RED);
      while (1)
      {
      }
    }
  }
	FLASH_Lock();
	taskEXIT_CRITICAL();
}
void STMF429FLASH_ReadHalfWord(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead)
{
	uint16_t* DataToRead=pBuffer;
	uint32_t Addr=ReadAddr;
	uint16_t i;
	taskENTER_CRITICAL();
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=*(__IO uint16_t*)Addr;
		Addr=Addr+2;
	}
	taskEXIT_CRITICAL();	
}
void STMF429FLASH_ReadWord(uint32_t ReadAddr,uint32_t *pBuffer,uint16_t NumToRead)
{
	uint32_t* DataToRead=pBuffer;
	uint32_t Addr=ReadAddr;
	uint16_t i;
	taskENTER_CRITICAL();
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=*(__IO uint32_t*)Addr;
		Addr=Addr+4;
	}
	taskEXIT_CRITICAL();	
}