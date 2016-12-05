/**
	 IAP_Library
	*@MCU			STM32F429
	*@author	Frank Shen
	*@email		shenchijun@gmail.com
	*@version	V1.0
	*@history
						v1.0
 */
#ifndef	FST_IAP
#define	FST_IAp 1

#include "main.h"

/**
 * @brief  
 * @param  
 * @retval result: 1:success 0:failure
 */
 
typedef void (*iapfun)(void);				//定义一个函数类型的参数.

void iap_load_app(u32 appxaddr);			//执行flash里面的app程序
void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 applen);	//在指定地址开始,写入bin


#endif