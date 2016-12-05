#include "FST_SDDriver_Library.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint8_t SD_Type=0;
static u8 flag_SDHC = 0;
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/


uint8_t SPI_ReadWriteByte(uint8_t TxData)
{
			uint8_t RxData;
			RxData=TM_SPI_Send(SPI1, TxData);
			
    
    //wait Transmissionbuffer to be clear
//		//FST_ILI9341_PrintText(7,1,"wait Transmissionbuffer", &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
//    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
    //send a byte
//		//FST_ILI9341_PrintText(8,1,"Send Datai", &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
//    SPI_I2S_SendData(SPI2, TxData);

    //wait data to be received
//		//FST_ILI9341_PrintText(9,1,"wait to receive datei", &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
//    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
    //read the data
//    RxData = SPI_I2S_ReceiveData(SPI2);
//	
//			sprintf(Text,"SPI_Read: %x",RxData);
//			//FST_ILI9341_PrintText(1,1,Text, &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
	
		

    return RxData;
}


void SPI_SetSpeed(uint8_t SpeedSet)
{
    SPI_InitTypeDef SPI_InitStructure;

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    if(SpeedSet==SPI_SPEED_LOW)
    {
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
				////FST_ILI9341_PrintText(5,1,"Set_Speed_Low", &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
    }
    else
    {
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
				////FST_ILI9341_PrintText(5,1,"Set_Speed_High", &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
    }
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
		SPI_InitStructure.SPI_CRCPolynomial = 7;
		SPI_Init(SPI1, &SPI_InitStructure);
    return;
}


uint8_t SD_WaitReady(void)
{
    uint8_t r1;
    uint16_t retry;
    retry = 0;
    do
    {
        r1 = SPI_ReadWriteByte(0xFF);
				retry++;
        if(retry==0xfffe)
        {
            return 1;
        }
    }while(r1!=0xFF);

    return 0;
}

uint8_t SD_SendCommand(uint8_t cmd, uint32_t arg, uint8_t crc)
{
    unsigned char r1;
    unsigned char Retry = 0;

    SPI_ReadWriteByte(0xff);
    //Select SD card
    SD_CS_ENABLE();

    //send
    SPI_ReadWriteByte(cmd | 0x40);  //seperately write command
    SPI_ReadWriteByte(arg >> 24);
    SPI_ReadWriteByte(arg >> 16);
    SPI_ReadWriteByte(arg >> 8);
    SPI_ReadWriteByte(arg);
    SPI_ReadWriteByte(crc);
    
    //wait respons
    while((r1 = SPI_ReadWriteByte(0xFF))==0xFF)
    {
        Retry++;
        if(Retry > 200)
        {
            break;
        }
    }
    //not select sd card
    SD_CS_DISABLE();
    //extra 8 clocks to the bus to let SD Card do the rest of the job
//    SPI_ReadWriteByte(0xFF);

    //return status value
    return r1;
}

uint8_t SD_SendCommand_NoDeassert(uint8_t cmd, uint32_t arg, uint8_t crc)
{
    unsigned char r1;
    unsigned char Retry = 0;

    SPI_ReadWriteByte(0xff);
    SD_CS_ENABLE();

    SPI_ReadWriteByte(cmd | 0x40);
    SPI_ReadWriteByte(arg >> 24);
    SPI_ReadWriteByte(arg >> 16);
    SPI_ReadWriteByte(arg >> 8);
    SPI_ReadWriteByte(arg);
    SPI_ReadWriteByte(crc);

    while((r1 = SPI_ReadWriteByte(0xFF))==0xFF)
    {
        Retry++;
        if(Retry > 200)
        {
            break;
        }
    }
    return r1;
}

uint8_t SD_Init(void)
{
    uint16_t 			i;      
    uint8_t 			r1;      
    uint16_t 			retry;  
    uint8_t 			buff[6];

		SD_CS_ENABLE();
		SPI_SetSpeed(SPI_SPEED_LOW);
		for(i=0;i<10;i++)
    {
				SPI_ReadWriteByte(0xFF);//dummy bytes
    }
		

    //-----------------SD set idle mode-----------------
    //continiously send CMD0, wait SD to return 0x01,enter IDLE status
    retry = 0;
    do
    {
        r1 = SD_SendCommand(CMD0, 0, 0x95);
        retry++;
    }while((r1 != 0x01) && (retry<200));
    if(retry==200)
    {
        return 1;   
    }
    //-----------------SD idle Status fertig-----------------
    //get the info of SD Card
    r1 = SD_SendCommand_NoDeassert(8, 0x1aa, 0x87);

    //version:v1.0 r1=0x05
    if(r1 == 0x05)
    {
				GUI_DispStringHCenterAt("SD_TYPE_V1" , 100, 30);
        SD_Type = SD_TYPE_V1;
        SD_CS_DISABLE();
        SPI_ReadWriteByte(0xFF);

        //-----------------SD Card MMC Card init-----------------

        //sd card init CMD55+ACMD41
        // when reply, dann SD
        // when not, MMC and do extra Work
        retry = 0;
        do
        {
            //send CMD55 and should return 0x01
            r1 = SD_SendCommand(CMD55, 0, 0);
            if(r1 != 0x01)
            {
								//FST_ILI9341_PrintText(4,1,"SD Card 0x01 Error!", &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
                return r1;  
            }
            //when correct, ACMD41,should return0x00
            r1 = SD_SendCommand(ACMD41, 0, 0);
						//FST_ILI9341_PrintText(4,1,"SD Card Found!", &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
            retry++;
        }while((r1!=0x00) && (retry<400));
        // MMC Extra Init
        // when reply, dann SD
        // when not, MMC 
        
        //----------MMC Extra Init------------
        if(retry==400)
        {
            retry = 0;
            do
            {
                r1 = SD_SendCommand(1, 0, 0);
                retry++;
            }while((r1!=0x00)&& (retry<400));
            if(retry==400)
            {
                return 1;
            }
						//FST_ILI9341_PrintText(4,1,"MMC Card Found!", &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
            SD_Type = SD_TYPE_MMC;
        }
        //----------MMC Extra Init fertig------------
        
				SPI_SetSpeed(1);
				SPI_ReadWriteByte(0xFF);
        
        //disable CRC
        /*
				r1 = SD_SendCommand(CMD59, 0, 0x01);
        if(r1 != 0x00)
        {
            return r1;  //????,??r1
        }
        */    
        //Set Sector Size
        r1 = SD_SendCommand(CMD16, 512, 0xff);
        if(r1 != 0x00)
        {
            return r1;  //wrong command and return r1
        }
        //-----------------SD Card MMC Card init fertig-----------------

    }
    //V2.0
    //decide SD2.0 or SD2.0HC
    else if(r1 == 0x01)
    {
				buff[0] = SPI_ReadWriteByte(0xFF);  //should be 0x00
			  if(buff[0]!=0x00)
				{					
				} 
				buff[1] = SPI_ReadWriteByte(0xFF);  //should be 0x00
				if(buff[1]!=0x00)
				{					
				} 
        buff[2] = SPI_ReadWriteByte(0xFF);  //should be 0x01
				if(buff[2]!=0x01)
				{					
				} 
        buff[3] = SPI_ReadWriteByte(0xFF);  //should be 0xAA
				if(buff[3]!=0xAA)
				{					
				} 
     
        SD_CS_DISABLE();
        //the next 8 clocks
        SPI_ReadWriteByte(0xFF);
        
        //ob support 2.7V-3.6V
        if(buff[2]==0x01 && buff[3]==0xAA)
        {
            //support
            retry = 0;
            //CMD55+ACMD41
						do
						{
							r1 = SD_SendCommand(CMD55, 0, 0);
							if(r1!=0x01)
							{
								GUI_DispStringHCenterAt("false1!" , 300, 30);
								return r1;
							}
							r1 = SD_SendCommand(ACMD41, 0x40000000, 0);
							if(retry>200)   
							{
								GUI_DispStringHCenterAt("false2!" , 300, 30);
								return r1;  
							}
						}while(r1!=0);
          
            //get OCR Info

            r1 = SD_SendCommand_NoDeassert(CMD58, 0, 0);
            if(r1!=0x00)
            {
                return r1;
            }
            //4 bytes OCR Data
            buff[0] = SPI_ReadWriteByte(0xFF);
            buff[1] = SPI_ReadWriteByte(0xFF); 
            buff[2] = SPI_ReadWriteByte(0xFF);
            buff[3] = SPI_ReadWriteByte(0xFF);

            SD_CS_DISABLE();
            SPI_ReadWriteByte(0xFF);

            //check OCR bit30(CCS),decide SD2.0 or SDHC
            //CCS=1:SDHC CCS=0:SD2.0
            if(buff[0]&0x40) //CCS
            {
                SD_Type = SD_TYPE_V2HC;
//								GUI_DispStringHCenterAt("SDHC Card Found!" , 300, 30);
            }
            else
            {
                SD_Type = SD_TYPE_V2;
//								GUI_DispStringHCenterAt("SD2.0 Card Found!" , 300, 30);
            }
            SPI_SetSpeed(SPI_SPEED_HIGH);  
        }

    }
		SD_CS_DISABLE();
		//SD_CS_ENABLE();
    return r1;
}


uint8_t SD_ReceiveData(u8 *data, u16 len, u8 release)
{
    u16 retry;
    u8 r1;

    SD_CS_ENABLE();
    retry = 0;
    do
    {
        r1 = SPI_ReadWriteByte(0xFF);
        retry++;
        if(retry>200)
        {
            SD_CS_DISABLE();
            return 1;
        }
    }while(r1 != 0xFE);
     while(len--)
    {
        *data = SPI_ReadWriteByte(0xFF);
        data++;
    }
    //send 2 fake CRC(dummy CRC)
    SPI_ReadWriteByte(0xFF);
    SPI_ReadWriteByte(0xFF);
    if(release == RELEASE)
    {
        //transmission ends
        SD_CS_DISABLE();
        SPI_ReadWriteByte(0xFF);
    }

    return 0;
}

uint8_t SD_GetCID(uint8_t *cid_data)
{
    u8 r1;

    r1 = SD_SendCommand(CMD10, 0, 0xFF);
    if(r1 != 0x00)
    {
        return r1;  //???????,???,??
    }
		//read 16 bytes data
    SD_ReceiveData(cid_data, 16, RELEASE);
    return 0;
}

uint8_t SD_GetCSD(uint8_t *csd_data)
{
//		char text[30];
    uint8_t r1;
    r1 = SD_SendCommand(CMD9, 0, 0xFF);
    if(r1 != 0x00)
    {
        return r1;
    }
    //receive 16 bytes data
    SD_ReceiveData(csd_data, 16, RELEASE);
		//sprintf(text,"CSD:0x%x",*csd_data);
		////FST_ILI9341_PrintText(10,1,text, &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
		
    return 0;
}


uint32_t SD_GetCapacity(void)
{
    uint8_t csd[16];
    uint32_t Capacity;
		u16 csize; 
    uint8_t r1;
    uint16_t i;
		uint16_t temp;

//    if(SD_GetCSD(csd)!=0)
//    {
//        return 0;
//    }
		SD_GetCSD(csd);
       
    //if SDHC
    if((csd[0]&0xC0)==0x40)
    {
        csize = csd[9] + ((u16)csd[8] << 8) + 1;
				Capacity = (u32)csize << 10;
    }
    else
    {
        //from website
        ////////////formula of the capacity///////////////
        //
        //  memory capacity = BLOCKNR * BLOCK_LEN
        //	
        //	BLOCKNR = (C_SIZE + 1)* MULT
        //
        //           C_SIZE_MULT+2
        //	MULT = 2
        //
        //               READ_BL_LEN
        //	BLOCK_LEN = 2
        /**********************************************/
        //C_SIZE
    	i = csd[6]&0x03;
    	i<<=8;
    	i += csd[7];
    	i<<=2;
    	i += ((csd[8]&0xc0)>>6);
    
        //C_SIZE_MULT
    	r1 = csd[9]&0x03;
    	r1<<=1;
    	r1 += ((csd[10]&0x80)>>7);
    
        //BLOCKNR
    	r1+=2;
    	temp = 1;
    	while(r1)
    	{
    		temp*=2;
    		r1--;
    	}
    	Capacity = ((u32)(i+1))*((u32)temp);
    
        // READ_BL_LEN
    	i = csd[5]&0x0f;
        //BLOCK_LEN
    	temp = 1;
    	while(i)
    	{
    		temp*=2;
    		i--;
    	}
        //The final result
    	Capacity *= (u32)temp; 
    	//Capacity /= 512;
    }
    return (uint32_t)Capacity;
}


uint8_t SD_ReadSingleBlock(uint32_t sector, uint8_t *buffer)
{
		uint8_t r1;
    SPI_SetSpeed(SPI_SPEED_HIGH);
    
    //SDHC
		if(SD_Type != SD_TYPE_V2HC)
		{
			sector = sector<<9;
		}
		r1 = SD_SendCommand(CMD17, sector, 0);//read command
		if(r1 != 0x00)
    {
				//FST_ILI9341_PrintText(2,10,"Read Block Error1", &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);	
        return r1;
    }
    
    r1 = SD_ReceiveData(buffer, 512, RELEASE);
    if(r1 != 0)
    {
			//FST_ILI9341_PrintText(2,10,"Read Block Error2", &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
      return r1;   //read data error
    }
    else
    {
        return 0;
    }
}

uint8_t SD_WriteSingleBlock(uint32_t sector, const uint8_t *data)
{
    uint8_t r1;
    uint16_t i;
    uint16_t retry;

    SPI_SetSpeed(SPI_SPEED_HIGH);

    if(SD_Type != SD_TYPE_V2HC)
    {
    
       sector = sector<<9;
    }

    r1 = SD_SendCommand(CMD24, sector, 0x00);
    if(r1 != 0x00)
    {
				//FST_ILI9341_PrintText(2,10,"Write Block Error1", &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
        return r1;
    }
    
    //prepare data to transfer
    SD_CS_ENABLE();
    //3 void data to wait for sd card
    SPI_ReadWriteByte(0xff);
    SPI_ReadWriteByte(0xff);
    SPI_ReadWriteByte(0xff);
    //begin ring
    SPI_ReadWriteByte(0xFE);

    //put a sector of data
    for(i=0;i<512;i++)
    {
        SPI_ReadWriteByte(*data++);
    }
    //2 Byte dummy CRC
    SPI_ReadWriteByte(0xff);
    SPI_ReadWriteByte(0xff);
    
    //wait SD answer
    r1 = SPI_ReadWriteByte(0xff);
    if((r1&0x1F)!=0x05)
    {
				//FST_ILI9341_PrintText(2,10,"Write Block Error2", &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
        SD_CS_DISABLE();
        return r1;
    }
    
    //wait the operation to be finished
    retry = 0;
    while(!SPI_ReadWriteByte(0xff))
    {
        retry++;
        if(retry>0xfffe)        
        {
            SD_CS_DISABLE();
						//FST_ILI9341_PrintText(2,10,"Write Block Error3", &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
            return 1;          
        }
    }

    SD_CS_DISABLE();
    SPI_ReadWriteByte(0xff);

    return 0;
}

uint8_t SD_ReadMultiBlock(uint32_t sector, uint8_t *buffer, uint8_t count)
{
    uint8_t r1;

    SPI_SetSpeed(SPI_SPEED_HIGH);
    
    //SDHC,sector byte
		if(SD_Type != SD_TYPE_V2HC)
		{
			sector = sector<<9;
    }
		//SD_WaitReady();
		//multiple read command
		r1 = SD_SendCommand(CMD18, sector, 0);//read data
		if(r1 != 0x00)
    {
				//FST_ILI9341_PrintText(2,10,"Read Multi Error1", &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
        return r1;
    }
    do
    {
        if(SD_ReceiveData(buffer, 512, NO_RELEASE) != 0x00)
        {
						//FST_ILI9341_PrintText(2,10,"Read Multi Error2", &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
            break;
        }
        buffer += 512;
    } while(--count);

    //all data transfer fertig
    SD_SendCommand(CMD12, 0, 0);
    
		SD_CS_DISABLE();
    SPI_ReadWriteByte(0xFF);
    
    if(count != 0)
    {
				//FST_ILI9341_PrintText(2,10,"NotAll Error", &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
        return count;   //return the number of the rest
    }
    else
    {
        return 0;
    }
}


uint8_t SD_WriteMultiBlock(uint32_t sector, const uint8_t *data, uint8_t count)
{
    uint8_t r1;
    uint16_t i;

    SPI_SetSpeed(SPI_SPEED_HIGH);

    //if not SDHC,then set sector address, else change to byte address
    if(SD_Type != SD_TYPE_V2HC)
    {
        sector = sector<<9;
    }
    //if not MMC?,send ACMD23 to erase
    if(SD_Type != SD_TYPE_MMC)
    {
        r1 = SD_SendCommand(ACMD23, count, 0x00);
    }
    //multiple write 
    r1 = SD_SendCommand(CMD25, sector, 0x00);
    if(r1 != 0x00)
    {
        return r1;  //if answer nor correct,return
    }
    
    SD_CS_ENABLE();
    SPI_ReadWriteByte(0xff);
    SPI_ReadWriteByte(0xff);

    do
    {
        SPI_ReadWriteByte(0xFC);
    
        for(i=0;i<512;i++)
        {
            SPI_ReadWriteByte(*data++);
        }
        SPI_ReadWriteByte(0xff);
        SPI_ReadWriteByte(0xff);
        
        //wait sd card to respond
        r1 = SPI_ReadWriteByte(0xff);
        if((r1&0x1F)!=0x05)
        {
            SD_CS_DISABLE();    //error
						//FST_ILI9341_PrintText(2,10,"Write Multi Error", &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
            return r1;
        }

        //wait sd card write finish
        if(SD_WaitReady()==1)
        {
            SD_CS_DISABLE();
            return 1;
        }

        //sector data transmission fertig
    }while(--count);
    
    //0xFD
    r1 = SPI_ReadWriteByte(0xFD);
    if(r1==0x00)
    {
        count =  0xfe;
    }

    if(SD_WaitReady())
    {
//        while(1)
//        {
//        }
    }
    
    //write fertig, cs 1
    SD_CS_DISABLE();
    SPI_ReadWriteByte(0xff);

    return count;   //return count?, if fertig count=0, else count=1
}
void SD_GPIOInit(void)
{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	SPI_InitTypeDef  SPI_InitStructure;
//	/*RCC Configuration*/
//	 /* PCLK2 = HCLK/2 */
//  
//	RCC_PCLK2Config(RCC_HCLK_Div2);
//  /* Enable GPIO clock for SPI2 */
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB| RCC_APB2Periph_AFIO, ENABLE);
//  /* Enable SPIz Periph clock */
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
//	
//	/* Configure SPIz pins: SCK, MISO and MOSI ---------------------------------*/
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//CS Pin
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
//	/* Configure SCK and MOSI pins as Alternate Function Push Pull */
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);


//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
//	/* Configure MISO pin as Input Floating  */
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	
//	/*SPI Settings*/
//	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
//	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
//	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
//	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
//	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
//	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
//  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
//	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
//	SPI_InitStructure.SPI_CRCPolynomial = 7;
//	SPI_Init(SPI2, &SPI_InitStructure);
//	 /* Enable SPIz */
//  SPI_Cmd(SPI2, ENABLE);
//  return;
}
uint8_t SD_GetCIDRegister(SD_CID* SD_cid)           										//read SD CID
{
	uint8_t CID_Tab[16];
	SD_GetCID(CID_Tab);
	/*!< Byte 0 */
  SD_cid->ManufacturerID = CID_Tab[0];

  /*!< Byte 1 */
  SD_cid->OEM_AppliID = CID_Tab[1] << 8;

  /*!< Byte 2 */
  SD_cid->OEM_AppliID |= CID_Tab[2];

  /*!< Byte 3 */
  SD_cid->ProdName1 = CID_Tab[3] << 24;

  /*!< Byte 4 */
  SD_cid->ProdName1 |= CID_Tab[4] << 16;

  /*!< Byte 5 */
  SD_cid->ProdName1 |= CID_Tab[5] << 8;

  /*!< Byte 6 */
  SD_cid->ProdName1 |= CID_Tab[6];

  /*!< Byte 7 */
  SD_cid->ProdName2 = CID_Tab[7];

  /*!< Byte 8 */
  SD_cid->ProdRev = CID_Tab[8];

  /*!< Byte 9 */
  SD_cid->ProdSN = CID_Tab[9] << 24;

  /*!< Byte 10 */
  SD_cid->ProdSN |= CID_Tab[10] << 16;

  /*!< Byte 11 */
  SD_cid->ProdSN |= CID_Tab[11] << 8;

  /*!< Byte 12 */
  SD_cid->ProdSN |= CID_Tab[12];

  /*!< Byte 13 */
  SD_cid->Reserved1 |= (CID_Tab[13] & 0xF0) >> 4;
  SD_cid->ManufactDate = (CID_Tab[13] & 0x0F) << 8;

  /*!< Byte 14 */
  SD_cid->ManufactDate |= CID_Tab[14];

  /*!< Byte 15 */
  SD_cid->CID_CRC = (CID_Tab[15] & 0xFE) >> 1;
  SD_cid->Reserved2 = 1;

	return 1;
}
uint8_t SD_GetCSDRegister(SD_CSD* SD_csd)           										//read SD CSD
{
	uint8_t CSD_Tab[16];
	SD_GetCSD(CSD_Tab);
	 /*!< Byte 0 */
  SD_csd->CSDStruct = (CSD_Tab[0] & 0xC0) >> 6;
  SD_csd->SysSpecVersion = (CSD_Tab[0] & 0x3C) >> 2;
  SD_csd->Reserved1 = CSD_Tab[0] & 0x03;

  if(SD_csd->CSDStruct == 1)
  {
    //printf("\n\r SDHC CSD Version 2.0 Acess AS SD High Capacity.");
		//FST_ILI9341_PrintText(10,1,"SDHC 2.0", &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
    flag_SDHC = 1;
  }
	else
	{
    //printf("\n\r SDSC CSD Version 1.0 Acess AS SD Standard Capacity.");	
		//FST_ILI9341_PrintText(10,1,"SDSC 1.0", &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
    flag_SDHC = 0;
	}
  

  /*!< Byte 1 */
  SD_csd->TAAC = CSD_Tab[1];

  /*!< Byte 2 */
  SD_csd->NSAC = CSD_Tab[2];

  /*!< Byte 3 */
  SD_csd->MaxBusClkFrec = CSD_Tab[3];

  /*!< Byte 4 */
  SD_csd->CardComdClasses = CSD_Tab[4] << 4;

  /*!< Byte 5 */
  SD_csd->CardComdClasses |= (CSD_Tab[5] & 0xF0) >> 4;
  SD_csd->RdBlockLen = CSD_Tab[5] & 0x0F;

  /*!< Byte 6 */
  SD_csd->PartBlockRead = (CSD_Tab[6] & 0x80) >> 7;
  SD_csd->WrBlockMisalign = (CSD_Tab[6] & 0x40) >> 6;
  SD_csd->RdBlockMisalign = (CSD_Tab[6] & 0x20) >> 5;
  SD_csd->DSRImpl = (CSD_Tab[6] & 0x10) >> 4;
  SD_csd->Reserved2 = 0; /*!< Reserved */

  if(flag_SDHC == 0)
  {
    SD_csd->DeviceSize = (CSD_Tab[6] & 0x03) << 10;
  
    /*!< Byte 7 */
    SD_csd->DeviceSize |= (CSD_Tab[7]) << 2;
  
    /*!< Byte 8 */
    SD_csd->DeviceSize |= (CSD_Tab[8] & 0xC0) >> 6;
  }
  else
  {
    SD_csd->DeviceSize = (CSD_Tab[7] & 0x3F) << 16;
    /* Byte 7 */
    SD_csd->DeviceSize += (CSD_Tab[8]) << 8;
    /* Byte 8 */
    SD_csd->DeviceSize += CSD_Tab[9] ;
  }    

  SD_csd->MaxRdCurrentVDDMin = (CSD_Tab[8] & 0x38) >> 3;
  SD_csd->MaxRdCurrentVDDMax = (CSD_Tab[8] & 0x07);

  /*!< Byte 9 */
  SD_csd->MaxWrCurrentVDDMin = (CSD_Tab[9] & 0xE0) >> 5;
  SD_csd->MaxWrCurrentVDDMax = (CSD_Tab[9] & 0x1C) >> 2;
  SD_csd->DeviceSizeMul = (CSD_Tab[9] & 0x03) << 1;
  /*!< Byte 10 */
  SD_csd->DeviceSizeMul |= (CSD_Tab[10] & 0x80) >> 7;
    
  SD_csd->EraseGrSize = (CSD_Tab[10] & 0x40) >> 6;
  SD_csd->EraseGrMul = (CSD_Tab[10] & 0x3F) << 1;

  /*!< Byte 11 */
  SD_csd->EraseGrMul |= (CSD_Tab[11] & 0x80) >> 7;
  SD_csd->WrProtectGrSize = (CSD_Tab[11] & 0x7F);

  /*!< Byte 12 */
  SD_csd->WrProtectGrEnable = (CSD_Tab[12] & 0x80) >> 7;
  SD_csd->ManDeflECC = (CSD_Tab[12] & 0x60) >> 5;
  SD_csd->WrSpeedFact = (CSD_Tab[12] & 0x1C) >> 2;
  SD_csd->MaxWrBlockLen = (CSD_Tab[12] & 0x03) << 2;

  /*!< Byte 13 */
  SD_csd->MaxWrBlockLen |= (CSD_Tab[13] & 0xC0) >> 6;
  SD_csd->WriteBlockPaPartial = (CSD_Tab[13] & 0x20) >> 5;
  SD_csd->Reserved3 = 0;
  SD_csd->ContentProtectAppli = (CSD_Tab[13] & 0x01);

  /*!< Byte 14 */
  SD_csd->FileFormatGrouop = (CSD_Tab[14] & 0x80) >> 7;
  SD_csd->CopyFlag = (CSD_Tab[14] & 0x40) >> 6;
  SD_csd->PermWrProtect = (CSD_Tab[14] & 0x20) >> 5;
  SD_csd->TempWrProtect = (CSD_Tab[14] & 0x10) >> 4;
  SD_csd->FileFormat = (CSD_Tab[14] & 0x0C) >> 2;
  SD_csd->ECC = (CSD_Tab[14] & 0x03);

  /*!< Byte 15 */
  SD_csd->CSD_CRC = (CSD_Tab[15] & 0xFE) >> 1;
  SD_csd->Reserved4 = 1;
	
	return 1;
}
uint8_t SD_GetCardInfo(SD_CardInfo *cardinfo)
{
  uint8_t status = 0;
  uint32_t Capacity;
	uint8_t csd[16];
	u16 csize; 
       
//    //if SDHC
//    if((csd[0]&0xC0)==0x40)
//    {
//        csize = csd[9] + ((u16)csd[8] << 8) + 1;
//				Capacity = (u32)csize << 10;
//    }
	char Text[30];

	SD_GetCSDRegister(&cardinfo->SD_csd);//SD_GetCSDRegister(&(cardinfo->SD_csd));
	SD_GetCIDRegister(&cardinfo->SD_cid);//SD_GetCIDRegister(&(cardinfo->SD_cid));
	if(0==flag_SDHC)//SDSC
	{
		cardinfo->CardCapacity = (cardinfo->SD_csd.DeviceSize + 1) ;
		cardinfo->CardCapacity *= (1 << (cardinfo->SD_csd.DeviceSizeMul + 2));
		cardinfo->CardBlockSize = 1 << (cardinfo->SD_csd.RdBlockLen);
		cardinfo->CardCapacity *= cardinfo->CardBlockSize;
	}
	else
	{
			SD_GetCSD(csd);
			cardinfo->CardBlockSize = 1 << (cardinfo->SD_csd.RdBlockLen);
			csize = csd[9] + ((u16)csd[8] << 8) + 1;
			cardinfo->CardCapacity = ((u32)csize << 10)*512;
	}

//  printf("\n\r SD CardBlockSize %d CardCapacity %d KB", cardinfo->CardBlockSize, 
//            cardinfo->CardCapacity / 1024 / 1024);
	sprintf(Text,"SD CardBlockSize %d Bytes\nSD Capacity %d MB", cardinfo->CardBlockSize,cardinfo->CardCapacity/ 1024/1024);
	//FST_ILI9341_PrintText(6,1,Text, &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
	sprintf(Text,"SD Card Name:%c%c%c%c%c", (cardinfo->SD_cid.ProdName1 & 0xff000000)>>24,(cardinfo->SD_cid.ProdName1 & 0xff0000)>>16, (cardinfo->SD_cid.ProdName1 & 0xff00)>>8,(cardinfo->SD_cid.ProdName1 & 0xff), cardinfo->SD_cid.ProdName2);
	//FST_ILI9341_PrintText(8,1,Text, &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
	sprintf(Text,"SD Date: 20%02d.%d", (cardinfo->SD_cid.ManufactDate & 0x0ff0)>>4,(cardinfo->SD_cid.ManufactDate & 0x0f));
	//FST_ILI9341_PrintText(9,1,Text, &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
  
  /*!< Returns the reponse */
  return 1;
	
}
