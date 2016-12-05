#ifndef __BSP_CAN_H
#define	__BSP_CAN_H
#include "main.h"

/*definition for CAN*/
#define CANx 											CAN1
#define CAN_CLK										RCC_APB1Periph_CAN1
/*interrupt number*/
#define CAN_RX_IRQ								CAN1_RX0_IRQn
/*isr*/
#define CAN_RX_IRQHandler					CAN1_RX0_IRQHandler 
/*pin definition*/
#define CAN_RX_PIN 								GPIO_Pin_8
#define CAN_TX_PIN								GPIO_Pin_9
#define CAN_TX_GPIO_PORT 					GPIOB
#define CAN_RX_GPIO_PORT					GPIOB 
#define CAN_TX_GPIO_CLK						RCC_AHB1Periph_GPIOB 					
#define CAN_RX_GPIO_CLK						RCC_AHB1Periph_GPIOB
#define CAN_AF_PORT								GPIO_AF_CAN1
#define CAN_RX_SOURCE							GPIO_PinSource8
#define CAN_TX_SOURCE							GPIO_PinSource9

/*IDE Sign*/
#define CAN_ID_STD 								((uint32_t)0x00000000) /*standard ID */
#define CAN_ID_EXT 								((uint32_t)0x00000004) /*extended ID */

/*RTR Sign*/
#define CAN_RTR_Data 							((uint32_t)0x00000000) /*data frame*/
#define CAN_RTR_Remote 						((uint32_t)0x00000002) /*remote frame*/

void CAN_SetMsg(CanTxMsg *TxMessage);

void CAN_Config(void);

void CAN_GPIO_Config(void);

void CAN_Mode_Config(void);

void CAN_Filter_Config(void);


#endif