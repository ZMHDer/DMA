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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3); //����ϵͳ�ж����ȼ�����2
	delay_init(168);								//��ʼ����ʱ����
	uart_init(115200);								//��ʼ�����ڲ�����Ϊ115200

		  //DMA2,STEAM5,CH4,����Ϊ����1,�洢��ΪSendBuff,����Ϊ:SEND_BUF_SIZE.

	//DMA�������ݽ���
	MYDMA_RXConfigPtoM(DMA2_Stream5, DMA_Channel_4, (u32)&USART1->DR, (u32)USART_RX_BUF, sizeof(USART_RX_BUF)); //DMA2,STEAM5,CH4,����Ϊ����1,�洢��ΪUSART_RX_BUF,����Ϊ:USART_REC_LEN.
	while (1)
	{
		if (USART_RX_STA)
		{
			if (DMA_GetFlagStatus(DMA2_Stream5, DMA_FLAG_TCIF5) != RESET) //�ȴ�DMA2_Steam5�������
			{
				USART_RX_STA = 0;			//����ձ�־λ
				printf("%s", USART_RX_BUF); //�������ݴ�ӡ

				//DMA�������ݷ��� DMA2,STEAM7,CH4,����Ϊ����1,�洢��ΪUSART_RX_BUF,����Ϊ:USART_RX_BUF.
				MYDMA_TXConfig(DMA2_Stream7, DMA_Channel_4, (u32)&USART1->DR, (u32)USART_RX_BUF, sizeof(USART_RX_BUF));
				while (DMA_GetFlagStatus(DMA2_Stream7, DMA_FLAG_TCIF7) == RESET)
				{
				} //�ȴ�DMA2_Steam7���ݴ������

				memset(USART_RX_BUF, 0, sizeof(USART_RX_BUF)); //��ս�����USART_RX_BUF,sizeof(USART_RX_BUF)
				DMA_ClearFlag(DMA2_Stream5, DMA_FLAG_TCIF5);   //���DMA2_Steam5������ɱ�־
			}
		}
	}
}
