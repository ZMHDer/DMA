/*
 * @Descripttion: 
 * @version: 
 * @Author: ZMHDer
 * @Date: 2019-08-24 11:40:20
 * @LastEditors: ZMHDer
 * @LastEditTime: 2021-08-11 09:25:32
 */
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "key.h"
#include "dma.h"
#include <string.h>

//u8 packebuff[900] = {0};
int DATA_LEN = 0;

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3); //设置系统中断优先级分组2
	delay_init(168);								//初始化延时函数
	uart_init(115200);								//初始化串口波特率为115200

		  //DMA2,STEAM5,CH4,外设为串口1,存储器为SendBuff,长度为:SEND_BUF_SIZE.

	//DMA串口数据接收
	MYDMA_RXConfigPtoM(DMA2_Stream5, DMA_Channel_4, (u32)&USART1->DR, (u32)USART_RX_BUF, sizeof(USART_RX_BUF)); //DMA2,STEAM5,CH4,外设为串口1,存储器为USART_RX_BUF,长度为:USART_REC_LEN.
	while (1)
	{
		if (USART_RX_STA)
		{
			if (DMA_GetFlagStatus(DMA2_Stream5, DMA_FLAG_TCIF5) != RESET) //等待DMA2_Steam5传输完成
			{
				USART_RX_STA = 0;			//清接收标志位
				printf("%s", USART_RX_BUF); //串口数据打印

				//DMA串口数据发送 DMA2,STEAM7,CH4,外设为串口1,存储器为USART_RX_BUF,长度为:USART_RX_BUF.
				MYDMA_TXConfig(DMA2_Stream7, DMA_Channel_4, (u32)&USART1->DR, (u32)USART_RX_BUF, sizeof(USART_RX_BUF));
				while (DMA_GetFlagStatus(DMA2_Stream7, DMA_FLAG_TCIF7) == RESET)
				{
				} //等待DMA2_Steam7数据传输完成

				memset(USART_RX_BUF, 0, sizeof(USART_RX_BUF)); //清空接受区USART_RX_BUF,sizeof(USART_RX_BUF)
				DMA_ClearFlag(DMA2_Stream5, DMA_FLAG_TCIF5);   //清除DMA2_Steam5传输完成标志
			}
		}
	}
}
