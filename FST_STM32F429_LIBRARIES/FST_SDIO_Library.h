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
* Description    : ��SD���ֿ��ж�ȡ�������ݵ�ָ���Ļ����� 
* Input          : pBuffer---���ݱ����ַ  
*				   					c--�����ַ����ֽ��� 
* Output         : None
* Return         : 0(success)  -1(fail)
* Attention		 	 : None
*******************************************************************************/ 
int GetGBKCode_from_sd(unsigned char* pBuffer,const unsigned char * c);

/* ��ʾ�ַ��� */
void LCD_DispCH(uint16_t x, uint16_t y, const uint8_t *pstr, uint32_t color,uint32_t bkcolor);
/* ��ʾһ������ 
 * ��СΪ 16����ȣ�* 16���߶ȣ�����32���ֽ�
 * ȡģ˳��Ϊ����λ��ǰ����λ�ں�
 */
/*
 * ��ʾһ������
 */
void LCD_DispStrCH(uint16_t x, uint16_t y, const uint8_t *pstr, uint32_t color, uint32_t bkcolor);

/*chinese font support*/


/*bmp file support*/
// �ĸ��ֽڶ����1�ƴ���
#define WIDTHBYTES(bits) (((bits)+31)/32*4)		//����24λ���ɫ ÿһ�е����ؿ�ȱ�����4�ı���  ����0����
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;

//λͼ�ļ�ͷ��Ϣ�ṹ����
//���в������ļ�������Ϣ�����ڽṹ����ڴ�ṹ������Ҫ�Ǽ��˵Ļ���������ȷ��ȡ�ļ���Ϣ��
typedef struct tagBITMAPFILEHEADER 
{   
    DWORD bfSize; 															//�ļ���С																				4
    WORD bfReserved1; 													//�����֣�������		 															2
    WORD bfReserved2; 													//�����֣�ͬ��			 															2
    DWORD bfOffBits; 														//ʵ��λͼ���ݵ�ƫ���ֽ�������ǰ�������ֳ���֮��	4
} BITMAPFILEHEADER,tagBITMAPFILEHEADER;
typedef struct tagBITMAPINFOHEADER
{
    DWORD biSize; 														//ָ���˽ṹ��ĳ��ȣ�Ϊ40		 												4
    LONG biWidth; 														//λͼ��											 												4
    LONG biHeight; 														//λͼ��											 												4
    WORD biPlanes; 														//ƽ������Ϊ1								 													2
    WORD biBitCount; 													//������ɫλ����������1��2��4��8��16��24�µĿ�����32	2
    DWORD biCompression; 											//ѹ����ʽ��������0��1��2������0��ʾ��ѹ��						4
    DWORD biSizeImage; 												//ʵ��λͼ����ռ�õ��ֽ���														4
    LONG biXPelsPerMeter; 										//X����ֱ���																					4
    LONG biYPelsPerMeter; 										//Y����ֱ���																					4
    DWORD biClrUsed; 													//ʹ�õ���ɫ�������Ϊ0�����ʾĬ��ֵ(2^��ɫλ��)			4
    DWORD biClrImportant; 										//��Ҫ��ɫ�������Ϊ0�����ʾ������ɫ������Ҫ��				4
} BITMAPINFOHEADER,tagBITMAPINFOHEADER;
typedef struct tagRGBQUAD 
{
    BYTE rgbBlue; 													//����ɫ����ɫ����
    BYTE rgbGreen; 													//����ɫ����ɫ����
    BYTE rgbRed; 														//����ɫ�ĺ�ɫ����
    BYTE rgbReserved;											 	//����ֵ
} RGBQUAD,tagRGBQUAD;
typedef struct RGB_PIXEL
{      //���ص���������
    unsigned char   rgbBlue;
    unsigned char   rgbGreen;
    unsigned char   rgbRed;
}RGB_PIXEL;
#define GETR_FROM_RGB16(RGB565)  ((unsigned char)(( ((unsigned short int )RGB565) >>11)<<3))		  			//����8λ R
#define GETG_FROM_RGB16(RGB565)  ((unsigned char)(( ((unsigned short int )(RGB565 & 0x7ff)) >>5)<<2)) 	//����8λ G
#define GETB_FROM_RGB16(RGB565)  ((unsigned char)(( ((unsigned short int )(RGB565 & 0x1f))<<3)))       	//����8λ B
#pragma diag_suppress 870 	//ʹ������֧�ֶ��ֽ��ַ�,�������invalid multibyte character sequence���� 

void BMP_Load(unsigned short int x, unsigned short int y,char *pic_name);
#endif

