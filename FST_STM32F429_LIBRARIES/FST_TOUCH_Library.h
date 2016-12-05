#ifndef __BSP_TOUCH_H
#define	__BSP_TOUCH_H

#include "main.h"

typedef	struct POINT 
{
   uint16_t x;		
   uint16_t y;
}Coordinate;

typedef struct Parameter 
{						
long double An,
            Bn,     
            Cn,   
            Dn,    
            En,    
            Fn,     
            Divider ;
}Parameter ;


#define	CHX 	0x90 	//Y+	
#define	CHY 	0xd0	//X+


//void FST_Touch_ConvertScreenPtToDisplayPt(Coordinate * displayPtr,Coordinate * screenPtr);
void FST_Touch_GetScreenPt(Coordinate * screenPtr);

/**
 * @brief ob point in the Rectangle is
 * @param  
 * @retval result: 1:PtInRectangle 0:PtNotInRectangle
 */
uint8_t FST_Touch_PtInRectangle(Coordinate * screenPtr, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void FST_Touch_Calibrate(void);

void Touch_Init(void);
void DrawCross(uint16_t x,uint16_t y);
int Touch_Calibrate(void);
uint16_t XPT2046_ReadAdc(uint8_t _ucCh);
unsigned short XPT2046_ReadCMD(void) ;
void XPT2046_WriteCMD(unsigned char cmd) ;
void DelayUS(vu32 cnt);
void Palette_Init(void);
FunctionalState Get_touch_point(Coordinate * displayPtr,
                                Coordinate * screenPtr,
                                Parameter * para );
Coordinate *Read_2046_2(void);
void Palette_draw_point(uint16_t x, uint16_t y);

void Delay_ms(uint16_t ms);

#endif /* __BSP_TOUCH_H */
