//#include "FST_SDIO_Library.h"
#include "main.h"
extern char text2[SD_Text_Length];//temp char
/*file system*/
extern FATFS fs;
extern FRESULT result;
extern FIL file;
extern DIR dir;
extern FILINFO fileinfo;
uint16_t mybr, mybw;               // File R/W count

#define RGB24TORGB16(R,G,B) ((unsigned short int)((((R)>>3)<<11) | (((G)>>2)<<5)	| ((B)>>3)))
//BYTE pColorData[960];					/* 一行真彩色数据缓存 320 * 3 = 960 */

//FATFS myfs[2];                 // Work area (file system object) for logical drive
FIL myfsrc, myfdst;            // file objects

//void SERVER_UpdateDstip(char * source,wiz_NetInfo *gWIZNETINFO)
//{
//	int num[5];
//}
//void SERVER_GetNumberFromDot(char* source,uint8_t *number)
//{
//	char *index=source;
//	char text[5];
//	uint8_t *numindex=number;
//	uint8_t i=1,j=0;
//	memcpy(text,'\0',5);
//	while(1)
//	{
//		if(*index=='.'||*index=='\0')
//		{
//			*numindex=atoi(text);
////			sprintf(text,"%d:%3d",i,num[i]);
////			//FST_ILI9341_PrintText(5,1,text, &FST_Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_MAGENTA);
//			memcpy(text,'\0',5);
////			_delay_ms(1000);
//			numindex++;
//			i++;
//			j=0;
//		}
//		else
//		{
//			text[j]=*index;
////			//FST_ILI9341_PrintText(4,1,&text[j], &FST_Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_MAGENTA);
////			_delay_ms(1000);
//			j++;
//		}
//		index++;
//		if(i==5)
//		{
//			break;
//		}		
//	}
//}

//void SERVER_GetCommand(char *command, char * dest, uint8_t index)
//{
//	uint8_t time=0;
//	char *destIndex=dest;
//	char *commandIndex=command;
//	while(1)
//	{
//		if(*commandIndex=='<')
//		{
//			time++;
//		}
//		if(*commandIndex=='\0')
//		{
//			*destIndex='\0';
//			return;
//		}
//		if(time==index)
//		{
//			commandIndex++;
//			break;
//		}
//		commandIndex++;
//	}
//	while(1)
//	{
//		if(*commandIndex=='>')
//		{
//			*destIndex='\0';
//			return;
//		}
//		if(*commandIndex=='\0')
//		{
//			*destIndex='\0';
//			return;
//		}
//		*destIndex=*commandIndex;
//		commandIndex++;
//		destIndex++;
//	}
//}
//uint8_t SERVER_IstCommandNull(char * dest)//return 1:empty return 0:not empty
//{
//	if(*dest=='\0')
//	{
//		return 1;
//	}
//	else
//	{
//		return 0;
//	}
//}
//void SERVER_SendCommand(uint8_t sock_tcp, char *command)
//{
//	//server binaryreader needs a length prefix watch:7BitEncodedInt
//		uint16_t i=0,j=0,k=0;
//		char text[50];
//		char *commandIndex=command;
//		uint8_t commandtosend[Temp_Text_Length];
//		while(*commandIndex!='\0')
//		{
//			commandtosend[i]=*commandIndex;
//			commandIndex++;
//			i++;
//			if(k>Temp_Text_Length)
//			{
//					////FST_ILI9341_PrintText(10,1,"send buffer full", &FST_Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_MAGENTA);
//			}
//		}
//		commandtosend[i]='\0';
//		j=send(sock_tcp,commandtosend,i+1);
//		
////7BitEncoderInt
////		if(k>127)
////		{
////			commandtosend[0]=0x81;
////			commandtosend[1]=0x00;
////			//commandtosend[1]=k-2-128;
////			j=send(sock_tcp,commandtosend,k);
////		}
////		else
////		{
////			commandtosend[1]=k-1;		
////			j=send(sock_tcp,&commandtosend[1],k);
////		}
////		if((i+1)!=j)
////		{
////			sprintf(text,"err:%2d IR:%2x IMR%2x",j,getSn_IR(1),getSn_IMR(1));
////			//FST_ILI9341_PrintText(3,1,text, &FST_Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_MAGENTA);
////			_delay_ms(1000);
////			//FST_ILI9341_ClearText(3,1,29, &FST_Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_MAGENTA);
////		}
//	
//}
//uint16_t SERVER_GetTextLength(char * source)
//{
//	uint16_t text_length=0;
//	char *index=source;
//	while(*index!='\0')
//	{
//		text_length++;
//		index++;
//	}
//	return text_length;
//}

///*<list_start><><><><list_end>*/
//void SERVER_SendLabelList(uint8_t sock_tcp)
//{
//	uint16_t i=1;
//	uint16_t length=0;
////	char *text2index=&text2[1];
//	char *text2index=text2;
//	char *LayoutFileNameindex;
//	memcpy(text2index,"<list_start>",12);
//	length=12;
//	text2index=text2index+12;
//	for(i=1;i<=Layout_Count;i++)
//	{
//		*text2index='<';
//		text2index++;
//		length++;
//		LayoutFileNameindex=&Layout_FileNames[i][0];
//		while(*LayoutFileNameindex!='\0')
//		{
//			*text2index=*LayoutFileNameindex;
//			text2index++;
//			length++;
//			LayoutFileNameindex++;		
//		}
//		*text2index='>';
//		text2index++;
//		length++;
//	}
//	memcpy(text2index,"<list_end>",10);
//	text2index=text2index+10;	
//	text2index='\0';
//	length=length+11;
////	text2[0]=length-1;
//	send(sock_tcp,text2,length);
//}

uint8_t SDIO_SETParameterValue(char *source, char *ToSetValue, char *SectionValue, uint16_t ParameterIndex)
{
	
	uint16_t ParaPosition,TextLength=0,TextLength_SetValue=0,i=0;
	char dest[30],*temp;
	char *text1;
	char Text[20];
	memcpy(text2,'\0',Temp_Text_Length);
	SDIO_GetParameterValue(source,dest,SectionValue,ParameterIndex,&ParaPosition);
	if(*dest=='\0') 
	{
		////FST_ILI9341_PrintText(13,1,"Parameter Error!", &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
		return 0;
	}
	temp=dest;
	while('\0'!=*temp)
	{
		temp++;
		TextLength++;
	}
	/*copy text2*/
	temp=source+ParaPosition+TextLength;
	while('\0'!=*temp)
	{
		text2[i]=*temp;
		i++;
		temp++;
		//TextLength_SetValue++;
	}
//	sprintf(Text,"lange:%d",TextLength_SetValue);
//	//FST_ILI9341_PrintText(1,1,Text, &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
//	while(1);
	
//	while('\0'!=*text2)
//	{
//		text2++;
//		TextLength++;
//	}
//	//FST_ILI9341_PrintText(2,1,"end", &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
//	while(1);
	
	/*change the para value*/
	text1=source+ParaPosition;
	while('\0'!=*ToSetValue)
	{
		*text1=*ToSetValue;
		ToSetValue++;
		text1++;
		TextLength_SetValue++;
	}
	//sprintf(Text,"lange:%d",TextLength_SetValue);
	////FST_ILI9341_PrintText(1,1,Text, &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
//	//FST_ILI9341_PrintText(2,1,source, &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
//	while(1);
	/*add the second part*/
	i=0;
	while('\0'!=text2[i])
	{
		*text1=text2[i];
		text1++;
		i++;
		//text2++;
	}
	text1++;
	*text1='\0';
	////FST_ILI9341_PrintText(1,1,"end", &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
	////FST_ILI9341_PrintText(2,1,source, &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
	
	
	
	return 1;//success	
}
void SDIO_GetParameterValue(char *source, char *destination, char *SectionValue, uint16_t ParameterIndex,uint16_t *ParameterPosition)
{
	uint16_t Position=0,Index=1,ParaIndex=0;
	uint16_t StringIndex=0,SectionNum=0;
	char dest[20];
	char Test[10];
	char PrintText[40];
	/*find the section*/
	do
	{
		SDIO_GetSectionValue(Index,source,dest,&Position);
		if(0==strcmp(dest,SectionValue))//section found
		{
			source=source+Position;
			StringIndex=Position;
			////FST_ILI9341_PrintText(12,1,source, &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
			/*begin to find the parameter*/
			while('\0'!=*source&&'['!=*source)
			{
				if('='==*source)
				{
					ParaIndex++;
					StringIndex++;
					if(ParaIndex==ParameterIndex)
					{
						////FST_ILI9341_PrintText(12,1,source, &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
						//while(1);
						////FST_ILI9341_PrintText(13,1,"Parameter Found", &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
						/*read value*/
						source++;
						*ParameterPosition=StringIndex-ParaIndex+1;//you du posion
						while('\0'!=*source&&'['!=*source&&'\r'!=*source&&'\n'!=*source&&';'!=*source)// end bedingungen
						{
							*destination=*source;
							////FST_ILI9341_PrintText(13,1,destination,&FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
							source++;
							destination++;
						}
						*destination='\0';//end flag
						////FST_ILI9341_PrintText(13,1,destination,&FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
						return;
					}						
				}
				source++;
				StringIndex++;
			}
			sprintf(PrintText,"Parameter Not Found\nSection:%s Index:%d",SectionValue,ParameterIndex);
			////FST_ILI9341_PrintText(12,1,PrintText, &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
			*destination='\0';
			return;
		}
		if('\0'==*dest)//Section Not Found
		{
			*destination='\0';
			////FST_ILI9341_PrintText(13,1,"Section Not Found", &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
			return;
		}
		Index++;//necessary,do not delete
//		sprintf(Test,"Index: %3d",Index);
//		//FST_ILI9341_PrintText(13,1,Test, &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
	}while(1);
	//strcmp()
	
	
}
void SDIO_GetSectionValue(uint16_t Index,char *source,char *destination,uint16_t *Position)
{
	uint16_t StringIndex=0,SectionNum=0;
	while('\0'!=*source)
	{
		if('['==*source)//1 section found
		{
			SectionNum++;
		}
		if(Index==SectionNum)//the required Section found
		{
			source++;
			StringIndex++;			
			while(']'!=*source||'\0'==*source)//read section value or read to the end(no ']' sign)
			{
				*destination=*source;
				source++;
				destination++;
				StringIndex++;
			}
			StringIndex++;
			*Position=StringIndex;
			*destination='\0';//end flag
			return;
		}
		StringIndex++;
		source++;
	}
	*Position=0;
	*destination='\0';
	return;
}

uint16_t LBL_GetCode39Pos(char *source)
{
	uint16_t CodeIndex=0;
	char *FindPt=source;
	/*Filter to find CODE39*/
	while(1)
	{
		if(*FindPt=='C'&&*(FindPt+1)=='O'&&*(FindPt+2)=='D'&&*(FindPt+3)=='E'&&*(FindPt+4)=='3'&&*(FindPt+5)=='9') break;
		FindPt++;
		CodeIndex++;		
	}
	/*Find ;*/
	while(1)
	{
		if(*FindPt==';') break;
		FindPt++;
		CodeIndex++;
	}
	CodeIndex++;
	return CodeIndex;
}

void LBL_GetCode39Text(char *source, char *destination)
{
//	uint16_t i=0;
//	char *FindPt=source;
//	uint16_t index=LBL_GetCode39Pos(source);
//	FindPt=FindPt+index;
//	while(1)
//	{
//		if('A'==*FindPt&&'1'==*(FindPt+2)) break;
//		destination[i]=*FindPt;
//		i++;
//		FindPt++;
//	}
//	destination[i]=0;
}
void LBL_SetCode39Text(uint16_t InsertNum,char *Text,char *source,char *destination)
{
//	char *FindPt=source;
//	char LayoutText_Part2[LayoutText_Part2_Length];
//	uint16_t index=LBL_GetCode39Pos(source);
//	uint16_t InsertIndex=0;
//	FindPt=FindPt+index+InsertNum;//FindPt points to the Code Text
//	
//	/*Save Code Text Part2*/
//	while(1)
//	{
//		LayoutText_Part2[InsertIndex]=*FindPt;
//		InsertIndex++;
//		FindPt++;
//		if(*FindPt==0)
//		{
//			LayoutText_Part2[InsertIndex]=0;
//			break;
//		}
//		
//	}
////	//FST_ILI9341_PrintText(7,1,LayoutText_Part2, &FST_Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_MAGENTA);
//	
//	InsertIndex=index+InsertNum;
//	index=0;
//	while(1)
//	{

//		destination[index]=source[index];
//		index++;
//		if(index==InsertIndex)
//		{
////			destination[index]=0;
////			//FST_ILI9341_PrintText(7,1,destination, &FST_Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_MAGENTA);
//			break;
//		}
//	}
//	/*adding Text*/
//	while(1)
//	{
//		destination[index]=*Text;
//		index++;
//		Text++;
//		if(*Text==0)
//		{
////			destination[index]=0;
////			//FST_ILI9341_PrintText(7,1,destination, &FST_Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_MAGENTA);
////			while(1);
//			break;
//		}
//	}
//	
//	InsertIndex=0;
//	/*rebuild the whole Layout*/
//	while(1)
//	{
//		destination[index]=LayoutText_Part2[InsertIndex];
//		InsertIndex++;
//		index++;
//		if(LayoutText_Part2[InsertIndex]==0)
//		{
//			destination[index]=0;
////			//FST_ILI9341_PrintText(7,1,destination, &FST_Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_MAGENTA);
////			while(1);
//			break;			
//		}
//	}
	
//	//FST_ILI9341_PrintText(7,1,LayoutText_Part2, &FST_Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_MAGENTA);
}

/*BMP*/
/*
 * 显示bmp图片, 24位真彩色
 * 图片宽度和高度根据图片大小而定
 */
void BMP_Load(unsigned short int x, unsigned short int y,char *pic_name)
{
	int i, j, k;
	int width, height, l_width;

	BYTE red,green,blue;
	BITMAPFILEHEADER bitHead;
	BITMAPINFOHEADER bitInfoHead;
	WORD fileType;

	UINT read_num;
	char tmp_name[30];
	sprintf(tmp_name,"Pictures/%s",pic_name);
	
	if(FR_OK!=f_mount(0, &fs))
	{
		GUI_DispStringHCenterAt("not mount correctly" , 600, 10);
		while(1);		
	}
	result = f_open( &file ,tmp_name, FA_OPEN_EXISTING | FA_READ);	
/*-------------------------------------------------------------------------------------------------------*/
	if(result == FR_OK)
	{
		/* 读取文件头信息  两个字节*/         
		f_read(&file,&fileType,sizeof(WORD),&read_num);     

		/* 判断是不是bmp文件 "BM"*/
		if(fileType != 0x4d42)
		{
			GUI_DispStringHCenterAt("not bmp" , 600, 10);
			while(1);
		}
		else
		{
			GUI_DispStringHCenterAt("is bmp" , 600, 10);
		}        
		/* 读取BMP文件头信息*/
		f_read(&file,&bitHead,sizeof(tagBITMAPFILEHEADER),&read_num);        
		/* 读取位图信息头信息 */
		f_read(&file,&bitInfoHead,sizeof(BITMAPINFOHEADER),&read_num);        
	}    
	else
	{
		GUI_DispStringHCenterAt(tmp_name, 600, 10);
		GUI_DispStringHCenterAt("file not open correctly" , 600, 30);
		while(1);
	}    
/*-------------------------------------------------------------------------------------------------------*/
	width = bitInfoHead.biWidth;
	height = bitInfoHead.biHeight;
	

	/* 计算位图的实际宽度并确保它为32的倍数	*/
	l_width = WIDTHBYTES(width* bitInfoHead.biBitCount);	
	sprintf(tmp_name,"width:%d height:%d",width,height);
	GUI_DispStringHCenterAt(tmp_name , 500, 50);
	/* 判断是否是24bit真彩色图 */
	if(bitInfoHead.biBitCount >= 24)
	{
		FST_SSD1963_SetPos(x,x+width-1,y,y+height-1);
		for(i=0;i< height; i++)
		{	  
			result=f_read(&file,text2,l_width,&read_num);
			for(j=0; j<width; j++) 											   //一行有效信息
			{
				k = j*3;																	 //一行中第K个像素的起点
				red = text2[k+2];
				green = text2[k+1];
				blue = 	text2[k];
				sprintf(tmp_name,"r:%3d g:%3d b:%3d",red,green,blue);
				GUI_DispStringHCenterAt(tmp_name , 100, 50);
			}            
		} 
		GUI_DispStringHCenterAt("fertig" , 700, 300);		
	}    
	else 
	{        
		GUI_DispStringHCenterAt("not 24bit bmp" , 500, 50);
		while(1);
	}
	f_close(&file); 
}

/*******************************************************************************
* Function Name  : GetGBKCode_from_sd
* Description    : 从SD卡字库中读取自摸数据到指定的缓冲区 
* Input          : pBuffer---数据保存地址  
*				   					c--汉字字符低字节码 
* Output         : None
* Return         : 0(success)  -1(fail)
* Attention		 	 : None
*******************************************************************************/ 
int GetGBKCode_from_sd(unsigned char* pBuffer,const unsigned char * c)
{ 
    unsigned char High8bit,Low8bit;
    unsigned int pos;
    High8bit=*c;     /* 取高8位数据 */
    Low8bit=*(c+1);  /* 取低8位数据 */
	
    //pos = ((High8bit-0xb0)*94+Low8bit-0xa0-1)*2*16;
    pos = ((High8bit-0xa0-16)*94+Low8bit-0xa0-1)*2*16;
	
//		if(FR_OK!=disk_initialize(0))//sd card not read
//		{
//			////FST_ILI9341_ClearText(10,1,30, &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
//			////FST_ILI9341_PrintText(10,1,"Error:\nSD Card cannot be read. \nPlease insert and restart!", &FST_Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_MAGENTA);
//			while(1);
//		}
//    f_mount(0, &bmpfs[0]);
    result = f_open(&myfsrc , "HZLIB.bin", FA_OPEN_EXISTING | FA_READ);
    
    if ( result == FR_OK ) 
    {
        f_lseek (&myfsrc, pos);		//指针偏移
        result = f_read( &myfsrc, pBuffer, 32, &mybr );//16*16大小的汉字 其字模 占用16*2个字节
        f_close(&myfsrc);
        return 0;  
    }    
    else
        return -1;    
}


void LCD_DispCH(uint16_t x, uint16_t y, const uint8_t *pstr, uint32_t color,uint32_t bkcolor)
{
	uint8_t page , column;
	uint8_t buffer[32];		
	uint16_t tmp_char=0;
	
	GetGBKCode_from_sd(buffer,pstr);	 /* 取字模数据 */
	
	for(page=0; page< CH_HEIGHT; page++)
	{
    /* 取出两个字节的数据，在lcd上即是一个汉字的一行 */
		tmp_char=buffer[page*2];
		tmp_char=(tmp_char<<8);
		tmp_char|=buffer[2*page+1];
		
		for (column=0; column< CH_WIDTH; column++)
		{			
			if ( tmp_char & (0x01<<15) )  /* 高位在前 */
			{				
				GUI_SetColor(GUI_RED);
				GUI_DrawPixel(x+column,y+page);
				
				
			}
			else
			{
				GUI_SetColor(GUI_WHITE);
				GUI_DrawPixel(x+column,y+page);	
			}
			tmp_char <<= 1;
		}
	}
}
void LCD_DispStrCH(uint16_t x, uint16_t y,  const uint8_t *pstr, uint32_t color, uint32_t bkcolor)
{
	while( *pstr != '\0' )
	{
		if( x > (COLUMN-CH_WIDTH) )
		{
			x = 0;
			y += CH_HEIGHT;
		}
		if( y > (PAGE-CH_HEIGHT) )
		{
			x = 0;
			y = 0;
		}		
		LCD_DispCH(x, y, pstr, color, bkcolor);
		pstr +=2;   /* 一个汉字两个字节 */  
		x += CH_WIDTH;		
	}	   
}


