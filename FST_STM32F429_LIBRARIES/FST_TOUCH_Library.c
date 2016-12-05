/*
 * TP ?? IC XPT2046 ??BSP
 */
 
 
#include "FST_TOUCH_Library.h"
#include "FST_GPIO_SPI.h"




/* ??????? */
#if 1
long double aa1=-0.0904,bb1=0.0014,cc1=318.5011,aa2=0.0021,bb2=-0.0678,cc2=254.3644;
#elif 0
long double aa1=0.088370,\
            bb1=-0.000468,\
            cc1=-24.042172,\
            aa2=0.0001891,\
            bb2=0.062395,\
            cc2=-10.223455;
#endif

/* ???? */
#define THRESHOLD 2 

/*--------------------------------------------------------------------*/
// ??????????

/* ????AD?????? */
Coordinate ScreenSample[4];

/* LCD????,????????? */
Coordinate DisplaySample[4] =   
{
    { 35,  35 },
    { 35,  200},
    { 290, 200},
    { 290, 35}
};
//Coordinate DisplaySample[4] =   
//{
//    { 45,  35 },
//    { 10,  200},
//    { 290, 200},
//    { 200, 35}
//};
/* ???????? */
Parameter   touch_para ;

/* ??????,?????????? */
Coordinate  display ;
/*------------------------------------------------------------------*/

/* 
 * ????SPI IO ? ?? IO ???
 */
void Touch_Init(void)
{
  GPIO_SPI_Config();
}

/*
 * us ????,?????
 */
void DelayUS(vu32 cnt)
{
    uint16_t i;
    for(i = 0;i<cnt;i++)
    {
        uint8_t us = 12; /* ????12,???1?? */    
        while (us--)     /* ?1??	*/
        {
            ;   
        }
    }
}

/*
*********************************************************************************************************
*	? ? ?: XPT2046_WriteCMD
*	????: ???
*	?    ?:CHX 	0x90 	//??Y+?????? CHY 	0xd0	//??X+??????
*	? ? ?: ?
*********************************************************************************************************
*/
void XPT2046_WriteCMD(unsigned char cmd) 
{
    unsigned char buf;
    unsigned char i;
		TP_CS(0);
		TP_DIN(0);
    TP_DCLK(0);
		for(i=0;i<8;i++) 
    {
        buf=(cmd>>(7-i))&0x01;
        TP_DIN(buf);
				DelayUS(5);
				//Delay_ms(5);
        TP_DCLK(1);
				DelayUS(5);
				//Delay_ms(5);
        TP_DCLK(0);
    }
}

/*
*********************************************************************************************************
*	? ? ?: XPT2046_ReadCMD
*	????: ????????,??ADC,???ADC????
*	?    ?:?
*	? ? ?: ?
*********************************************************************************************************
*/

unsigned short XPT2046_ReadCMD(void) 
{
    unsigned short buf=0,temp;
    unsigned char i;
		//char Text[40];
    TP_DIN(0);
    //TP_DCLK(1);
    for(i=0;i<16;i++) 
    {
        TP_DCLK(1);
				DelayUS(5);
			  //Delay_ms(5);
        temp= (TP_DOUT) ? 1:0;
        buf|=(temp<<(16-i));
        TP_DCLK(0);
				DelayUS(5);
    }
	
//  buf&=0x0fff;
		buf=buf>>4;
		TP_CS(1);


    return(buf);
}


/*
*********************************************************************************************************
*	? ? ?: TSC2046_ReadAdc
*	????: ????????,??ADC,???ADC????
*	?    ?:_ucCh = 0x90 ??Y??; 0xd0 ??X??
*	? ? ?: 12?ADC?
*********************************************************************************************************
*/
uint16_t XPT2046_ReadAdc(uint8_t _ucCh)
{
	  //uint16_t usAdc;

    XPT2046_WriteCMD(_ucCh);

	return 	XPT2046_ReadCMD();
}


/*
 * ?????????? 
 * x:0~300
 * y:0~230
 */
void DrawCross(uint16_t x,uint16_t y)
{
   //LCD_Clear(x, y, 20, 1, RED);
   //LCD_Clear(x+10, y-10, 1, 20, RED);
//	//FST_ILI9341_DrawLine(x,y,x+20,y,ILI9341_COLOR_RED );
//	//FST_ILI9341_DrawLine(x+10,y-10,x+10,y+10,ILI9341_COLOR_RED );
}

/*
 * ??TP x y ?AD?(12bit,???4096)
 */
void Touch_GetAdXY(int *x,int *y)  
{ 
    int adx,ady; 
    adx = XPT2046_ReadAdc(CHX);
  
    DelayUS(1); 
    ady = XPT2046_ReadAdc(CHY);  
    *x=adx; 
    *y=ady; 
}

Coordinate *Read_2046(void)
{
    static Coordinate  screen;
    int m0,m1,m2,TP_X[1],TP_Y[1],temp[3];
    uint8_t count=0;
		
		/*Sample x and y 9 times*/
    int buffer[2][9]={{0},{0}};  	
    do
    {		   
        Touch_GetAdXY(TP_X,TP_Y);  
        buffer[0][count]=TP_X[0];  
        buffer[1][count]=TP_Y[0];
        count++; 
				 
    }
    while(!INT_IN_2046&& count<9);  
    
		
		if(INT_IN_2046 ==1)
    {
			
    }

		/*sample 9 times correctly*/ 
    if(count==9)   								
    {  
        temp[0]=(buffer[0][0]+buffer[0][1]+buffer[0][2])/3;
        temp[1]=(buffer[0][3]+buffer[0][4]+buffer[0][5])/3;
        temp[2]=(buffer[0][6]+buffer[0][7]+buffer[0][8])/3;
        
        m0=temp[0]-temp[1];
        m1=temp[1]-temp[2];
        m2=temp[2]-temp[0];
        
        m0=m0>0?m0:(-m0);
        m1=m1>0?m1:(-m1);
        m2=m2>0?m2:(-m2);
        
        if( m0>THRESHOLD  &&  m1>THRESHOLD  &&  m2>THRESHOLD ) 
            return 0;
        
        if(m0<m1)
        {
            if(m2<m0) 
                screen.x=(temp[0]+temp[2])/2;
            else 
                screen.x=(temp[0]+temp[1])/2;	
        }
        else if(m2<m1) 
            screen.x=(temp[0]+temp[2])/2;
        else 
            screen.x=(temp[1]+temp[2])/2;
        
        temp[0]=(buffer[1][0]+buffer[1][1]+buffer[1][2])/3;
        temp[1]=(buffer[1][3]+buffer[1][4]+buffer[1][5])/3;
        temp[2]=(buffer[1][6]+buffer[1][7]+buffer[1][8])/3;
        m0=temp[0]-temp[1];
        m1=temp[1]-temp[2];
        m2=temp[2]-temp[0];
        m0=m0>0?m0:(-m0);
        m1=m1>0?m1:(-m1);
        m2=m2>0?m2:(-m2);
        if(m0>THRESHOLD&&m1>THRESHOLD&&m2>THRESHOLD) 
            return 0;
        
        if(m0<m1)
        {
            if(m2<m0) 
                screen.y=(temp[0]+temp[2])/2;
            else 
                screen.y=(temp[0]+temp[1])/2;	
        }
        else if(m2<m1) 
            screen.y=(temp[0]+temp[2])/2;
        else
            screen.y=(temp[1]+temp[2])/2;
        
        return &screen;
    }
    
    else if(count>1)
    {
        screen.x=buffer[0][0];
        screen.y=buffer[1][0];
        return &screen;
    }  
    return 0; 
}

Coordinate *Read_2046_2(void)
{
    static Coordinate  screen2;
    int TP_X[1],TP_Y[1];
    uint8_t count=0;
    int buffer[2][10]={{0},{0}};  /*??X?Y??????*/
    int min_x,max_x;
    int min_y,max_y;
    int	i=0;
    
    do					       				
    {
        Touch_GetAdXY(TP_X,TP_Y);  
        buffer[0][count]=TP_X[0];  
        buffer[1][count]=TP_Y[0];
        count++;  
    }	
    while(!INT_IN_2046&& count<10);
    
    if(INT_IN_2046)						
    {
		 
    }
		
    if(count ==10)		 					
    {
        max_x=min_x=buffer[0][0];
        max_y=min_y=buffer[1][0];       
        for(i=1; i<10; i++)
        {
            if(buffer[0][i]<min_x)
            {
                min_x=buffer[0][i];
            }
            else
            if(buffer[0][i]>max_x)
            {
                max_x = buffer[0][i];
            }
        }
        
        for(i=1; i<10; i++)
        {
            if(buffer[1][i]<min_y)
            {
                min_y=buffer[1][i];
            }
            else
            if(buffer[1][i]>max_y)
            {
                max_y = buffer[1][i];
            }
        }
        screen2.x=(buffer[0][0]+buffer[0][1]+buffer[0][2]+buffer[0][3]+buffer[0][4]+buffer[0][5]+buffer[0][6]+buffer[0][7]+buffer[0][8]+buffer[0][9]-min_x-max_x)>>3;
        screen2.y=(buffer[1][0]+buffer[1][1]+buffer[1][2]+buffer[1][3]+buffer[1][4]+buffer[1][5]+buffer[1][6]+buffer[1][7]+buffer[1][8]+buffer[1][9]-min_y-max_y)>>3; 
        
        return &screen2;
    }    
    return 0;    
}

/******************************************************
* ???:Cal_touch_para
* ??  :???????????????????? K A B C D E F??
* ??  : ?
* ??  :??1???? 0??
* ??  :?
* ??  :???LCD??????????????,????????
*********************************************************/    
FunctionalState Cal_touch_para( Coordinate * displayPtr,
                                Coordinate * screenPtr,
                                Parameter * para)
{
    
    FunctionalState retTHRESHOLD = ENABLE ;

    /* K=(X0-X2) (Y1-Y2)-(X1-X2) (Y0-Y2) */
    para->Divider = ((screenPtr[0].x - screenPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) - 
                    ((screenPtr[1].x - screenPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;
    
		if( para->Divider == 0 )
    {
        retTHRESHOLD = DISABLE;
    }
    else
    {
        /* A=((XD0-XD2) (Y1-Y2)-(XD1-XD2) (Y0-Y2))/K	*/
        para->An = ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) - 
                   ((displayPtr[1].x - displayPtr[2].x) * (screenPtr[0].y - screenPtr[2].y));
        
        /* B=((X0-X2) (XD1-XD2)-(XD0-XD2) (X1-X2))/K	*/
        para->Bn = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].x - displayPtr[2].x)) - 
                   ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].x - screenPtr[2].x));
        
        /* C=(Y0(X2XD1-X1XD2)+Y1(X0XD2-X2XD0)+Y2(X1XD0-X0XD1))/K */
        para->Cn = (screenPtr[2].x * displayPtr[1].x - screenPtr[1].x * displayPtr[2].x) * screenPtr[0].y +
                   (screenPtr[0].x * displayPtr[2].x - screenPtr[2].x * displayPtr[0].x) * screenPtr[1].y +
                   (screenPtr[1].x * displayPtr[0].x - screenPtr[0].x * displayPtr[1].x) * screenPtr[2].y ;
        
        /* D=((YD0-YD2) (Y1-Y2)-(YD1-YD2) (Y0-Y2))/K	*/
        para->Dn = ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].y - screenPtr[2].y)) - 
                   ((displayPtr[1].y - displayPtr[2].y) * (screenPtr[0].y - screenPtr[2].y)) ;
        
        /* E=((X0-X2) (YD1-YD2)-(YD0-YD2) (X1-X2))/K	*/
        para->En = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].y - displayPtr[2].y)) - 
                   ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].x - screenPtr[2].x)) ;
        
        
        /* F=(Y0(X2YD1-X1YD2)+Y1(X0YD2-X2YD0)+Y2(X1YD0-X0YD1))/K */
        para->Fn = (screenPtr[2].x * displayPtr[1].y - screenPtr[1].x * displayPtr[2].y) * screenPtr[0].y +
                   (screenPtr[0].x * displayPtr[2].y - screenPtr[2].x * displayPtr[0].y) * screenPtr[1].y +
                   (screenPtr[1].x * displayPtr[0].y - screenPtr[0].x * displayPtr[1].y) * screenPtr[2].y;
        
    }
    return( retTHRESHOLD ) ;
}

/******************************************************
* ???:Touchl_Calibrate
* ??  :???????
* ??  : ?
* ??  :0	---	????
					1	---	????
* ??  :?
* ??  :?
*********************************************************/ 
int Touch_Calibrate(void)
{
//    #if 1
//    uint8_t i;
//    u16 test_x=0, test_y=0;
//    u16 gap_x=0, gap_y=0;
//    Coordinate * Ptr;   
//		char Text[40];
//    
//    for(i=0; i<4; i++)
//    {        
//        //LCD_Clear(0, 0, 320, 240, BACKGROUND);  
//				//FST_ILI9341_Fill(ILI9341_COLOR_BLUE);      
//        //LCD_DispStr(110, 110, (uint8_t *)"Touch Calibrate......", RED);	 
//				sprintf(Text,"Touch Calibrate......%2d/4",i+1);
//				//FST_ILI9341_Puts(10, 110, Text, &FST_Font_11x18, ILI9341_COLOR_YELLOW, ILI9341_COLOR_MAGENTA);
//        //LCD_DisNum(160, 90, i+1, RED);
//      
//        Delay_ms(500);     
//        DrawCross(DisplaySample[i].x,DisplaySample[i].y);  //??????“?”?
//        do
//        {
//            Ptr=Read_2046_2();        //??XPT2046?????ptr            
//        }
//        while( Ptr == (void*)0 );     //?ptr????????????
//        ScreenSample[i].x= Ptr->x; 	  //???????????????ScreenSample???
//        ScreenSample[i].y= Ptr->y;
//				sprintf(Text,"Position X:%d\nPosition Y:%d",ScreenSample[i].x,ScreenSample[i].y);
//				//FST_ILI9341_PrintText(1, 6, Text, &FST_Font_11x18, ILI9341_COLOR_YELLOW, ILI9341_COLOR_MAGENTA);
//				Delay_ms(500); 
//				

//    }
//		/* ???????? ????????????? */
//    Cal_touch_para( &DisplaySample[0],&ScreenSample[0],&touch_para ) ;  	   
//    
//		/*??????X?*/
//    test_x = ( (touch_para.An * ScreenSample[3].x) + 
//               (touch_para.Bn * ScreenSample[3].y) + 
//                touch_para.Cn 
//             ) / touch_para.Divider ;			 
//    
//		/*??????Y?*/
//    test_y = ( (touch_para.Dn * ScreenSample[3].x) + 
//               (touch_para.En * ScreenSample[3].y) + 
//               touch_para.Fn 
//             ) / touch_para.Divider ;
//    
//    /* ??????????? */
//    gap_x = (test_x > DisplaySample[3].x)?(test_x - DisplaySample[3].x):(DisplaySample[3].x - test_x);
//    gap_y = (test_y > DisplaySample[3].y)?(test_y - DisplaySample[3].y):(DisplaySample[3].y - test_y);
//		sprintf(Text,"gap_x:%d\ngap_y:%d",gap_x,gap_y);
//		//FST_ILI9341_PrintText(1, 8, Text, &FST_Font_11x18, ILI9341_COLOR_YELLOW, ILI9341_COLOR_MAGENTA);
//		Delay_ms(100); 
//    

//    //LCD_Rectangle(0,0,320,240,CAL_BACKGROUND_COLOR);
//    //LCD_Clear(0, 0, 320, 240, BACKGROUND);
//		//FST_ILI9341_Fill(ILI9341_COLOR_BLUE);
//    
//    /* ?????????????????? */
//    if((gap_x>30)||(gap_y>30))
//    {
//      //LCD_DispStr(100, 100, (uint8_t *)"Calibrate fail", RED);
//			//FST_ILI9341_Puts(100, 100, "Calibrate fail", &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
//      //LCD_DispStr(100, 120, (uint8_t *)"try again", RED);
//		  //FST_ILI9341_Puts(100, 120, "try again", &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
//      Delay_ms(2000);
//      return 1;
//    }    
//    
//    aa1 = (touch_para.An*1.0)/touch_para.Divider;
//    bb1 = (touch_para.Bn*1.0)/touch_para.Divider;
//    cc1 = (touch_para.Cn*1.0)/touch_para.Divider;
//    
//    aa2 = (touch_para.Dn*1.0)/touch_para.Divider;
//    bb2 = (touch_para.En*1.0)/touch_para.Divider;
//    cc2 = (touch_para.Fn*1.0)/touch_para.Divider;
//		
//		sprintf(Text,"aa1=%f\nbb1=%f\ncc1=%f\naa2=%f\nbb2=%f\ncc2=%f\n",aa1,bb1,cc1,aa2,bb2,cc2);
//		//FST_ILI9341_PrintText(1, 8, Text, &FST_Font_11x18, ILI9341_COLOR_YELLOW, ILI9341_COLOR_MAGENTA);
//		 
//    while(1);
//    #elif 0
//    aa1=0.088370;
//    bb1=-0.000468;
//    cc1=-24.042172;
//    aa2=0.0001891;
//    bb2=0.062395;
//    cc2=-10.223455;
//    
//    #endif
//    
//    //LCD_DispStr(100, 100, (uint8_t *)"Calibrate Succed", RED);  
//		//FST_ILI9341_Puts(100, 100, "Calibrate Succed", &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
//    Delay_ms(1000);
    
    
    return 0;    
}

/*
 * ?????,?????
 */
void Palette_Init(void)
{
  /* ?????? */
  //LCD_Clear(0, 0, 320, 240, WHITE);
//	//FST_ILI9341_Fill(ILI9341_COLOR_WHITE);
  
  /* ????? */
//  LCD_Clear(39, 0, 1, 30, BLACK);
//  LCD_Clear(0, 29, 40, 1, BLACK);
//  LCD_DispStr(7, 10, (uint8_t *)"CLR", RED);
//  
//  LCD_Clear(0, 30, 40, 30, GREEN);
//  LCD_Clear(0, 60, 40, 30, BLUE);
//  LCD_Clear(0, 90, 40, 30, BRED);
//  LCD_Clear(0, 120, 40, 30, GRED);
//  LCD_Clear(0, 150, 40, 30, GBLUE);
//  LCD_Clear(0, 180, 40, 30, BLACK);
//  LCD_Clear(0, 210, 40, 30, RED);  
  
  Delay_ms(500);
}

/******************************************************
* ???:Get_touch_point
* ??  :?? K A B C D E F ???X Y??????????
* ??  : ?
* ??  :??1???? 0??
* ??  :?
* ??  :???????????,???DISABLE
*********************************************************/    
//long double linear=0 ;
//long double aa1=0,bb1=0,cc1=0,aa2=0,bb2=0,cc2=0;
FunctionalState Get_touch_point(Coordinate * displayPtr,
                                Coordinate * screenPtr,
                                Parameter * para )
{
  FunctionalState retTHRESHOLD =ENABLE ;

  if(screenPtr==0)
  {
    /*???????????,???DISABLE*/
    retTHRESHOLD = DISABLE;			
  }
  else
  {    
    if( para->Divider != 0 )
    {        
      displayPtr->x = ( (aa1 * screenPtr->x) + (bb1 * screenPtr->y) + cc1);        
      displayPtr->y = ((aa2 * screenPtr->x) + (bb2 * screenPtr->y) + cc2 );
    }
    else
    {
      retTHRESHOLD = DISABLE;
    }
  }
  return(retTHRESHOLD);
} 

//void FST_Touch_ConvertScreenPtToDisplayPt(Coordinate * displayPtr,Coordinate * screenPtr)
//{
//	displayPtr->x = (aa1 * screenPtr->x) + (bb1 * screenPtr->y) + cc1;        
//  displayPtr->y = (aa2 * screenPtr->x) + (bb2 * screenPtr->y) + cc2;
//}
void FST_Touch_Calibrate(void)
{
	uint16_t i,j,temp,NumOfSamplePunkt=100;
	uint16_t displayPtr_x;
	uint16_t displayPtr_y;
	uint16_t sum_x=0,sum_y=0;
	uint16_t screenPtr_x[100];
	uint16_t screenPtr_y[100];
	char Text[40];
	for(i=0;i<NumOfSamplePunkt;i++)
		{
			//do
			//{
				screenPtr_x[i]= XPT2046_ReadAdc(CHX);
				DelayUS(10);
				screenPtr_y[i]= XPT2046_ReadAdc(CHY);
			//}while(screenPtr_x[i]>4000||screenPtr_x[i]>4000);
		}
			
		
		for(i=0;i<NumOfSamplePunkt;i++)
		{
			for(j=i+1;j<NumOfSamplePunkt;j++)
			{
				if(screenPtr_x[i]>screenPtr_x[j])
				{
					temp=screenPtr_x[i];
					screenPtr_x[i]=screenPtr_x[j];
					screenPtr_x[j]=temp;
				}
				if(screenPtr_y[i]>screenPtr_y[j])
				{
					temp=screenPtr_y[i];
					screenPtr_y[i]=screenPtr_y[j];
					screenPtr_y[j]=temp;
				}
			}
		}
//		//FST_ILI9341_Fill(ILI9341_COLOR_MAGENTA);
		for(i=45;i<55;i++)
		{
			sum_x+=screenPtr_x[i];
			sum_y+=screenPtr_y[i];
//			sprintf(Text,"Val_X:%d Val_Y:%d",screenPtr_x[i],screenPtr_y[i]);
//			//FST_ILI9341_PrintText(i-42,1,Text, &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
		}
		sum_x=sum_x/10;
		sum_y=sum_y/10;
		sum_x=aa1*sum_x+bb1*sum_y+cc1;
		sum_y=aa2*sum_x+bb2*sum_y+cc2;

		
		
		//screenPtr=Read_2046_2();
		//FST_Touch_GetScreenPunkt(displayPtr, screenPtr);
//		displayPtr_x = (aa1 * screenPtr_x) + (bb1 * screenPtr_y) + cc1;        
//		displayPtr_y = (aa2 * screenPtr_x) + (bb2 * screenPtr_y) + cc2;
//		sprintf(Text,"Real Val_X:%d\nReal Val_Y:%d",sum_x,sum_y);
//	
//		//FST_ILI9341_PrintText(1,1,Text, &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
		
		DrawCross(50,50);
		DrawCross(50,100);
		DrawCross(50,150);
		DrawCross(50,200);
		
		DrawCross(100,50);
		DrawCross(100,100);
		DrawCross(100,150);
		DrawCross(100,200);
		
		DrawCross(150,50);
		DrawCross(150,100);
		DrawCross(150,150);
		DrawCross(150,200);
		
		DrawCross(200,50);
		DrawCross(200,100);
		DrawCross(200,150);
		DrawCross(200,200);
		
		DrawCross(250,50);
		DrawCross(250,100);
		DrawCross(250,150);
		DrawCross(250,200);
}

void FST_Touch_GetScreenPt(Coordinate * screenPtr)
{
	uint8_t i=0,j;
	uint16_t NumOfSamplePunkt=20;
	uint16_t sum_x=0,sum_y=0,temp=0;
	uint16_t screenPtr_x[20];
	uint16_t screenPtr_y[20];
	char Text[40];
	while(0==INT_IN_2046)
	{
		screenPtr_x[i]= XPT2046_ReadAdc(CHX);
		DelayUS(1);
		screenPtr_y[i]= XPT2046_ReadAdc(CHY);
		DelayUS(1);
		i++;
		if(i>20) break;
	}
	while(!INT_IN_2046);
		
	/*sorting*/
	for(i=0;i<NumOfSamplePunkt;i++)
	{
		for(j=i+1;j<NumOfSamplePunkt;j++)
		{
			if(screenPtr_x[i]>screenPtr_x[j])
			{
				temp=screenPtr_x[i];
				screenPtr_x[i]=screenPtr_x[j];
				screenPtr_x[j]=temp;
			}
			if(screenPtr_y[i]>screenPtr_y[j])
			{
				temp=screenPtr_y[i];
				screenPtr_y[i]=screenPtr_y[j];
				screenPtr_y[j]=temp;
			}
		}
	}
	/*filtering*/
	for(i=5;i<15;i++)
	{
		sum_x+=screenPtr_x[i];
		sum_y+=screenPtr_y[i];
		//sprintf(Text,"Val_X:%d Val_Y:%d",screenPtr_x[i],screenPtr_y[i]);
		////FST_ILI9341_PrintText(i-2,1,Text, &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
	}
	sum_x=sum_x/10;
	sum_y=sum_y/10;
	//sprintf(Text,"Real Val_X:%d\nReal Val_Y:%d",sum_x,sum_y);
	////FST_ILI9341_PrintText(1,10,Text, &FST_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
	screenPtr->x=aa1*sum_x+bb1*sum_y+cc1;
	screenPtr->y=aa2*sum_x+bb2*sum_y+cc2;
}

uint8_t FST_Touch_PtInRectangle(Coordinate * screenPtr, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
	uint16_t PointX=0;
	uint16_t PointY=0;
	uint16_t i;
	//uint16_t x2,y2,x3,y3;//x2,y2 right top point x3,y3 left bottom point
	
	//17 points around the touch point
	uint8_t ChangeInX[9]={-5,0,5,-5,0,5,-5,0,5};//,-10,0,10,-10,10,-10,0,10};
	uint8_t ChangeInY[9]={-5,-5,-5,0,0,0,5,5,5};//,-10,-10,-10,0,0,10,10,10};
	
	for(i=0;i<9;i++)
	{
		PointX=screenPtr->x+ChangeInX[i];
		PointY=screenPtr->y+ChangeInY[i];
		if((x0<=PointX)&&(PointX<=x1)&&(y0<=PointY)&&(PointY<=y1))
		{
			return 1;//point in the rectangle
		}
		
	}
	return 0;//point not in the rectangle
}




/******************************************************
* ???:Palette_draw_point
* ??  :?LCD?????????(??????)
* ??  : Xpos		--X????
*         Ypos		--Y????
* ??  :?
* ??  :Palette_draw_point(100,100);
* ??  :???? "????????" ????
*********************************************************/    
void Palette_draw_point(uint16_t x, uint16_t y)
{
//	//FST_ILI9341_DrawPixel(x, y, ILI9341_COLOR_RED);
//  /* ??????? */
//  static u16 Pen_color=0; 
//  u16 y_pos = y;

//  /* ?????? */
//  if( x<40 )
//  {
//    if( y>30 )
//    Pen_color = (y_pos<60)?GREEN:\
//                (y_pos<90)?BLUE:\
//                (y_pos<120)?BRED:\
//                (y_pos<150)?GRED:\
//                (y_pos<180)?GBLUE:\
//                (y_pos<210)?BLACK:\
//                (y_pos<240)?RED:BLUE;
//    else
//    {/* ?? */      
//      #if 1
//      LCD_Clear(40, 0, 280, 240, BLACK);
//      #elif 0
//      LCD_Clear(40, 0, 280, 240, WHITE);
//      #endif
//      return;
//    }
//  }
//  else
//  {
//    #if 0
//    LCD_SetPoint(x , y , Pen_color);
//    LCD_SetPoint(x+1 , y , Pen_color);
//    LCD_SetPoint(x , y+1 , Pen_color);
//    LCD_SetPoint(x+1 , y+1 , Pen_color);
//    #elif 1
//    LCD_SetPoint(x , y , Pen_color);
//    LCD_SetPoint(x-1 , y , Pen_color);
//    LCD_SetPoint(x , y-1 , Pen_color);
//    LCD_SetPoint(x+1 , y , Pen_color);
//    LCD_SetPoint(x , y+1 , Pen_color);
//    #endif
//  }	
}
void Delay_ms(uint16_t ms)
{
	uint32_t l;
	for(l=0;l<ms;l++)
	{
		DelayUS(1000);
	}
}
