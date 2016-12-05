#ifndef _SPI_SDIO_DRIVER_H_
#define _SPI_SDIO_DRIVER_H_

/*FATFS File String operation*/


/*ini file support*/
void SDIO_GetSectionValue(uint16_t Index,char *source,char *destination,uint16_t *Position);
void SDIO_GetParameterValue(char *source, char *destination, char *SectionValue, uint16_t ParameterIndex, uint16_t *ParameterPosition);
uint8_t SDIO_SETParameterValue(char *source, char *ToSetValue, char *SectionValue, uint16_t ParameterIndex);

/*server commands support*/
//void SERVER_GetCommand(char *command,char * dest,uint8_t index);
//uint8_t SERVER_IstCommandNull(char * dest);//return 1:empty return 0:not empty
//uint16_t SERVER_GetTextLength(char * source);
//void SERVER_SendCommand(uint8_t sock_tcp,char *command);
//void SERVER_SendLabelList(uint8_t sock_tcp);
//void SERVER_UpdateDstip(char * source,wiz_NetInfo *gWIZNETINFO);
//void SERVER_GetNumberFromDot(char* source,uint8_t *number);//save 4 numbers from xx.xx.xx.xx
/*LBL Layout file support*/
void LBL_GetCode39Text(char *source,char *destination);
uint16_t LBL_GetCode39Pos(char *source);
void LBL_SetCode39Text(uint16_t InsertNum,char *Text,char *source,char *destination);

/*chinese font support*/
#define CH_HEIGHT 16
#define CH_WIDTH 	16
#define COLUMN		800
#define PAGE			480
/*******************************************************************************
* Function Name  : GetGBKCode_from_sd
* Description    : 从SD卡字库中读取自摸数据到指定的缓冲区 
* Input          : pBuffer---数据保存地址  
*				   					c--汉字字符低字节码 
* Output         : None
* Return         : 0(success)  -1(fail)
* Attention		 	 : None
*******************************************************************************/ 
int GetGBKCode_from_sd(unsigned char* pBuffer,const unsigned char * c);

/* 显示字符串 */
void LCD_DispCH(uint16_t x, uint16_t y, const uint8_t *pstr, uint32_t color,uint32_t bkcolor);
/* 显示一个汉字 
 * 大小为 16（宽度）* 16（高度），共32个字节
 * 取模顺序为：高位在前，低位在后
 */
/*
 * 显示一串汉字
 */
void LCD_DispStrCH(uint16_t x, uint16_t y, const uint8_t *pstr, uint32_t color, uint32_t bkcolor);

/*chinese font support*/


/*bmp file support*/
// 四个字节对齐进1制处理
#define WIDTHBYTES(bits) (((bits)+31)/32*4)		//对于24位真彩色 每一行的像素宽度必须是4的倍数  否则补0补齐
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;

//位图文件头信息结构定义
//其中不包含文件类型信息（由于结构体的内存结构决定，要是加了的话将不能正确读取文件信息）
typedef struct tagBITMAPFILEHEADER 
{   
    DWORD bfSize; 															//文件大小																				4
    WORD bfReserved1; 													//保留字，不考虑		 															2
    WORD bfReserved2; 													//保留字，同上			 															2
    DWORD bfOffBits; 														//实际位图数据的偏移字节数，即前三个部分长度之和	4
} BITMAPFILEHEADER,tagBITMAPFILEHEADER;
typedef struct tagBITMAPINFOHEADER
{
    DWORD biSize; 														//指定此结构体的长度，为40		 												4
    LONG biWidth; 														//位图宽											 												4
    LONG biHeight; 														//位图高											 												4
    WORD biPlanes; 														//平面数，为1								 													2
    WORD biBitCount; 													//采用颜色位数，可以是1，2，4，8，16，24新的可以是32	2
    DWORD biCompression; 											//压缩方式，可以是0，1，2，其中0表示不压缩						4
    DWORD biSizeImage; 												//实际位图数据占用的字节数														4
    LONG biXPelsPerMeter; 										//X方向分辨率																					4
    LONG biYPelsPerMeter; 										//Y方向分辨率																					4
    DWORD biClrUsed; 													//使用的颜色数，如果为0，则表示默认值(2^颜色位数)			4
    DWORD biClrImportant; 										//重要颜色数，如果为0，则表示所有颜色都是重要的				4
} BITMAPINFOHEADER,tagBITMAPINFOHEADER;
typedef struct tagRGBQUAD 
{
    BYTE rgbBlue; 													//该颜色的蓝色分量
    BYTE rgbGreen; 													//该颜色的绿色分量
    BYTE rgbRed; 														//该颜色的红色分量
    BYTE rgbReserved;											 	//保留值
} RGBQUAD,tagRGBQUAD;
typedef struct RGB_PIXEL
{      //像素的数据类型
    unsigned char   rgbBlue;
    unsigned char   rgbGreen;
    unsigned char   rgbRed;
}RGB_PIXEL;
#define GETR_FROM_RGB16(RGB565)  ((unsigned char)(( ((unsigned short int )RGB565) >>11)<<3))		  			//返回8位 R
#define GETG_FROM_RGB16(RGB565)  ((unsigned char)(( ((unsigned short int )(RGB565 & 0x7ff)) >>5)<<2)) 	//返回8位 G
#define GETB_FROM_RGB16(RGB565)  ((unsigned char)(( ((unsigned short int )(RGB565 & 0x1f))<<3)))       	//返回8位 B
#pragma diag_suppress 870 	//使编译器支持多字节字符,否则会有invalid multibyte character sequence警告 

void BMP_Load(unsigned short int x, unsigned short int y,char *pic_name);
#endif

