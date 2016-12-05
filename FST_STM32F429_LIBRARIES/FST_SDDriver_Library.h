#ifndef _SPI_SD_DRIVER_H_
#define _SPI_SD_DRIVER_H_

#include "main.h"

/* Private define ------------------------------------------------------------*/
/* SD card type definition*/
#define SD_TYPE_MMC     0
#define SD_TYPE_V1      1
#define SD_TYPE_V2      2
#define SD_TYPE_V2HC    4

/* SPI bus speed setting*/
#define SPI_SPEED_LOW   0
#define SPI_SPEED_HIGH  1

/* SD Card after Data Transmission release Bus*/
#define NO_RELEASE      0
#define RELEASE         1
/*SD Card init*/
#define STA_NO_ERR 					0
#define STA_TIME_OUT 				1
#define STA_NO_CARD					99
/* SD Card command */
#define CMD0    0       //card reset
#define CMD1		1				//command1, card initialization
#define CMD9    9       //command9 ,read CSD data
#define CMD10   10      //command10,read CID data
#define CMD12   12      //command12,stop data transmission
#define CMD16   16      //command16,set SectorSize and should return 0x00
#define CMD17   17      //command17,read sector
#define CMD18   18      //command18,read Multi sector
#define ACMD23  23      //command23,before read sector erase N block
#define CMD24   24      //command24,write sector
#define CMD25   25      //command25,write Multi sector
#define ACMD41  41      //command41,return 0x00
#define CMD55   55      //command55,return 0x01
#define CMD58   58      //command58,read OCR data
#define CMD59   59      //command59,enable/disable CRC, should return0x00

/** 
  * @brief  Card Specific Data: CSD Register   
  */ 
typedef struct
{
  __IO uint8_t  CSDStruct;            /*!< CSD structure */
  __IO uint8_t  SysSpecVersion;       /*!< System specification version */
  __IO uint8_t  Reserved1;            /*!< Reserved */
  __IO uint8_t  TAAC;                 /*!< Data read access-time 1 */
  __IO uint8_t  NSAC;                 /*!< Data read access-time 2 in CLK cycles */
  __IO uint8_t  MaxBusClkFrec;        /*!< Max. bus clock frequency */
  __IO uint16_t CardComdClasses;      /*!< Card command classes */
  __IO uint8_t  RdBlockLen;           /*!< Max. read data block length */
  __IO uint8_t  PartBlockRead;        /*!< Partial blocks for read allowed */
  __IO uint8_t  WrBlockMisalign;      /*!< Write block misalignment */
  __IO uint8_t  RdBlockMisalign;      /*!< Read block misalignment */
  __IO uint8_t  DSRImpl;              /*!< DSR implemented */
  __IO uint8_t  Reserved2;            /*!< Reserved */
  __IO uint32_t DeviceSize;           /*!< Device Size */
  __IO uint8_t  MaxRdCurrentVDDMin;   /*!< Max. read current @ VDD min */
  __IO uint8_t  MaxRdCurrentVDDMax;   /*!< Max. read current @ VDD max */
  __IO uint8_t  MaxWrCurrentVDDMin;   /*!< Max. write current @ VDD min */
  __IO uint8_t  MaxWrCurrentVDDMax;   /*!< Max. write current @ VDD max */
  __IO uint8_t  DeviceSizeMul;        /*!< Device size multiplier */
  __IO uint8_t  EraseGrSize;          /*!< Erase group size */
  __IO uint8_t  EraseGrMul;           /*!< Erase group size multiplier */
  __IO uint8_t  WrProtectGrSize;      /*!< Write protect group size */
  __IO uint8_t  WrProtectGrEnable;    /*!< Write protect group enable */
  __IO uint8_t  ManDeflECC;           /*!< Manufacturer default ECC */
  __IO uint8_t  WrSpeedFact;          /*!< Write speed factor */
  __IO uint8_t  MaxWrBlockLen;        /*!< Max. write data block length */
  __IO uint8_t  WriteBlockPaPartial;  /*!< Partial blocks for write allowed */
  __IO uint8_t  Reserved3;            /*!< Reserded */
  __IO uint8_t  ContentProtectAppli;  /*!< Content protection application */
  __IO uint8_t  FileFormatGrouop;     /*!< File format group */
  __IO uint8_t  CopyFlag;             /*!< Copy flag (OTP) */
  __IO uint8_t  PermWrProtect;        /*!< Permanent write protection */
  __IO uint8_t  TempWrProtect;        /*!< Temporary write protection */
  __IO uint8_t  FileFormat;           /*!< File Format */
  __IO uint8_t  ECC;                  /*!< ECC code */
  __IO uint8_t  CSD_CRC;              /*!< CSD CRC */
  __IO uint8_t  Reserved4;            /*!< always 1*/
} SD_CSD;

/** 
  * @brief  Card Identification Data: CID Register   
  */
typedef struct
{
  __IO uint8_t  ManufacturerID;       /*!< ManufacturerID */
  __IO uint16_t OEM_AppliID;          /*!< OEM/Application ID */
  __IO uint32_t ProdName1;            /*!< Product Name part1 */
  __IO uint8_t  ProdName2;            /*!< Product Name part2*/
  __IO uint8_t  ProdRev;              /*!< Product Revision */
  __IO uint32_t ProdSN;               /*!< Product Serial Number */
  __IO uint8_t  Reserved1;            /*!< Reserved1 */
  __IO uint16_t ManufactDate;         /*!< Manufacturing Date */
  __IO uint8_t  CID_CRC;              /*!< CID CRC */
  __IO uint8_t  Reserved2;            /*!< always 1 */
} SD_CID;

/** 
  * @brief SD Card information 
  */
typedef struct
{
  SD_CSD SD_csd;
  SD_CID SD_cid;
  uint32_t CardCapacity;  /*!< Card Capacity */
  uint32_t CardBlockSize; /*!< Card Block Size */
} SD_CardInfo;

/* Private macro -------------------------------------------------------------*/
//SD card enable CS:
#define SD_CS_ENABLE()      GPIO_ResetBits(GPIOA,GPIO_Pin_4)   //select SD Card
#define SD_CS_DISABLE()     GPIO_SetBits(GPIOA,GPIO_Pin_4)     	//not select SD card
#define SD_PWR_ON()         //GPIO_ResetBits(GPIOD,GPIO_Pin_10) //SD Card power on
#define SD_PWR_OFF()        //GPIO_SetBits(GPIOD,GPIO_Pin_10)   //SD Card power off
#define SD_DET()            1 
//!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2) //test ob card exis 1-exist 0-not exist

/* Private function prototypes -----------------------------------------------*/
uint8_t SPI_ReadWriteByte(uint8_t TxData);
void SD_GPIOInit(void);
void SPI_SetSpeed(uint8_t SpeedSet);
uint8_t SD_WaitReady(void);                          										//wait SD Card Ready
uint8_t SD_SendCommand(uint8_t cmd, uint32_t arg, uint8_t crc);					//SD Card sends a command
uint8_t SD_SendCommand_NoDeassert(uint8_t cmd, uint32_t arg, uint8_t crc);
uint8_t SD_Init(void);                               										//SD Card init
uint8_t SD_ReceiveData(uint8_t *data, uint16_t len, uint8_t release);		//SD Card read data
uint8_t SD_GetCID(u8 *cid_data);                     										//read SD CID
uint8_t SD_GetCSD(u8 *csd_data);                     										//read SD CSD
uint8_t SD_GetCIDRegister(SD_CID* SD_cid);           										//read SD CID
uint8_t SD_GetCSDRegister(SD_CSD* SD_csd);           										//read SD CSD
uint32_t SD_GetCapacity(void);                       										//read SD Card storage
uint8_t SD_GetCardInfo(SD_CardInfo *cardinfo);													//read sd card info

uint8_t SD_ReadSingleBlock(uint32_t sector, uint8_t *buffer);  					//read a sector
uint8_t SD_WriteSingleBlock(uint32_t sector, const uint8_t *buffer); 		//write a sector
uint8_t SD_ReadMultiBlock(uint32_t sector, uint8_t *buffer, uint8_t count); //read multi sectors
u8 SD_WriteMultiBlock(u32 sector, const u8 *data, u8 count);  					//write multi sectors	

#endif
