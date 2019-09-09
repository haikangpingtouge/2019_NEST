/**
	|------------------------------ Copyright -----------------------------------|
	|                                                                            |
	|                        (C) Copyright 2019,����ƽͷ��,                       |
	|           1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China      |
	|                            All Rights Reserved                             |
	|                                                                            |
	|           By(GCU The wold of team | ��������ѧ����ѧԺ������Ұ�Ƕ�)         |
	|                    https://github.com/GCUWildwolfteam                      |
	|----------------------------------------------------------------------------|
	|--FileName    : bsp_usart.h                                                
	|--Version     : v1.0                                                            
	|--Author      : ����ƽͷ��                                                       
	|--Date        : 2019-01-15               
	|--Libsupports : 
	|--Description :                                                       
	|--FunctionList                                                       
	|-------1. ....                                                       
	|          <version>:                                                       
	|     <modify staff>:                                                       
	|             <data>:                                                       
	|      <description>:                                                        
	|-------2. ...                                                       
	|------------------------------declaration of end----------------------------|
 **/
#ifndef __BSP_USART_H 
#define __BSP_USART_H 
#include "baseclass.h "
#include "fps.h" 
typedef struct usartDataStrcut
{
	uint8_t a_frame_len;//һ֡�ĳ���
	uint8_t rx_buff_size;//����У�鳤��
	uint8_t *rx_buff_data; //���ջ���ռ��ַ
	uint8_t rx_on_off;   //���ݽ��տ���
  uint16_t datalen;//һ֡���ݳ���
	xQueueHandle usart_queue; //���о��
}usartDataStrcut;
HAL_StatusTypeDef UsartAndDMAInit(UART_HandleTypeDef *huartx,\
																	uint8_t frame_len,\
																	uint8_t on_off);
void UserUsartCallback(UART_HandleTypeDef *huartx);
HAL_StatusTypeDef NoLenRXforUsart(UART_HandleTypeDef *huartx);
HAL_StatusTypeDef UsartQueueCreate(usartDataStrcut *usartx, uint8_t len,
																	 uint8_t deep);
HAL_StatusTypeDef AllocateUsartxSpace(UART_HandleTypeDef *huartx);
usartDataStrcut *GetUsartAddr(UART_HandleTypeDef *huartx);
HAL_StatusTypeDef UserUsartQueueRX(UART_HandleTypeDef *huartx, \
																	 void *const pvBuffer);

void RxCcomuu(UART_HandleTypeDef *huartx);
#define A_FRAME_CHECK_LEN 3//һ֡���ջ��泤��
#define DATA_LEN_BYTE_LEN 2//���ݳ���
#define DATA_LEN_BYTE_HIGH_8 0//���ݳ��ȸ߰�λ
#define DATA_LEN_BYTE_LOW_8  1//���ݳ��ȵͰ�λ
#define HEAD_FRAME_LEN (A_FRAME_CHECK_LEN + DATA_LEN_BYTE_LEN)
#define RX_HEAD_ADDR   2//�������ݵ��׵�ַ

#endif // __BSP_USART_H

/*----------------------------------file of end-------------------------------*/
