/**
	|------------------------------ Copyright -----------------------------------|
	|                                                                            |
	|                       (C) Copyright 2019,����ƽͷ��,                        |
	|          1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China       |
	|                            All Rights Reserved                             |
	|                                         -                                  |
	|          By(GCU The wold of team | ��������ѧ����ѧԺ������Ұ�Ƕ�)          |
	|                    https://github.com/GCUWildwolfteam                      |
	|----------------------------------------------------------------------------|
	|--FileName    : bsp_usart.c                                                
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
#include "bsp_usart.h"
/* ----------------- �ṹ���ַ�б� -------------------- */
usartDataStrcut *pusart1_t = NULL;
usartDataStrcut *pusart2_t = NULL;
usartDataStrcut *pusart3_t = NULL;
usartDataStrcut *pusart6_t = NULL;
usartDataStrcut *pusart7_t = NULL;
usartDataStrcut *pusart8_t = NULL;
// xQueueHandle usart1_queue = NULL;
// xQueueHandle usart2_queue = NULL;
// xQueueHandle usart3_queue = NULL;
// xQueueHandle usart6_queue = NULL;
/**
	* @Data    2019-01-15 15:31
	* @brief   ���ڳ�ʼ��
	* @param	 usartx(x=1~7)
	* @param   uint8_t frame_size һ֡�ĳ���
	* @param   on_off   DISABLE = 0U, ENABLE = 1U
	* @retval  HAL Status
	*/
HAL_StatusTypeDef UsartAndDMAInit(UART_HandleTypeDef *huartx,uint8_t frame_size\
																															,uint8_t on_off)
{
	usartDataStrcut *addr;
	/* -------- ����ӿ����ݻ�ȡ --------- */
	if(AllocateUsartxSpace(huartx) != HAL_OK)//�û����ڽṹ��ռ����
	{
		free(addr);
		return HAL_ERROR;
	}
	addr = GetUsartAddr(huartx); //��ȡ��Ӧ�û����ڽṹ���ַ
	addr->usart_queue = NULL; 
	addr->rx_buff_size = frame_size; //�õ�һ֡�ĳ���
	addr->rx_on_off = on_off;//���տ���
	/* ------ ������ջ����ַ�ռ� ------- */
	addr->rx_buff_data = (uint8_t *)malloc(addr->rx_buff_size * sizeof(uint8_t));
	if(UsartQueueCreate(addr,5,addr->rx_buff_size) != HAL_OK) //��������
	{
		//��Ӵ������
		free(addr->rx_buff_data);
		free(addr);
		return HAL_ERROR;
	}
  	/* -------- ʹ�� --------- */
 HAL_UART_Receive_DMA(huartx,addr->rx_buff_data,addr->rx_buff_size);
	__HAL_UART_ENABLE_IT(huartx, UART_IT_IDLE);					 //ʹ�ܴ����ж�
	return HAL_OK;
}
/**
	* @Data    2019-01-15 18:58
	* @brief   �����û��жϻص�����
	* @param   huartx(x=1~6) �û����ڽṹ���ַ
	* @retval  void
	*/
	void UserUsartCallback(UART_HandleTypeDef *huartx)
	{
		NoLenRXforUsart(huartx);
	}
/**
	* @Data    2019-01-15 19:07
	* @brief   �����ֽڽ���
	* @param   usartx(x=1~7) �û����ڽṹ���ַ
	* @retval  HAL Status
	*/
	HAL_StatusTypeDef NoLenRXforUsart(UART_HandleTypeDef *huartx)
	{
//--------------------ԭ��
//		usartDataStrcut *addr = NULL;
//		addr = GetUsartAddr(huartx); //��ȡ��Ӧ�û����ڽṹ���ַ
//		if(addr == NULL)
//			return HAL_ERROR;
//		if(addr->rx_on_off != ENABLE)
//			return HAL_ERROR;
//		__HAL_UART_CLEAR_IDLEFLAG(huartx);
////       HAL_UART_DMAStop(huartx);
//		/* -------- ����rx_buff_len���ֽ� --------- */
//		if(huartx->hdmarx->Instance->NDTR== A_FRAME_CHECK_LEN)
//		{
////			/* -------- ����У��ֵ --------- */
////			if(RCREncryption(addr->rx_buff_data, addr->rx_buff_size) != HAL_OK)
////				return HAL_ERROR;
//			xQueueSendToBackFromISR(addr->usart_queue, addr->rx_buff_data,0);
//		}
//    /* ------ ���ý��յ�ַ�����ݳ��� ------- */
//    HAL_UART_Receive_DMA(huartx,addr->rx_buff_data,addr->rx_buff_size);
//-------------------ԭ��
//------------------ceshi
      uint32_t temp; 
		usartDataStrcut *addr = NULL;
		addr = GetUsartAddr(huartx); //��ȡ��Ӧ�û����ڽṹ���ַ
    floatToUnion p;
		if(addr->rx_on_off != ENABLE)
    {
    }
     if((__HAL_UART_GET_FLAG(huartx,UART_FLAG_IDLE) != RESET))  
     {
	    	__HAL_UART_CLEAR_IDLEFLAG(huartx);
        HAL_UART_DMAStop(huartx);  
     temp = huartx->hdmarx->Instance->NDTR; 
      addr->datalen = addr->rx_buff_size-temp;
      p.u_16[0] = addr->datalen;
      addr->rx_buff_data[DATA_LEN_BYTE_HIGH_8] = p.u_8[DATA_LEN_BYTE_HIGH_8];
      addr->rx_buff_data[DATA_LEN_BYTE_LOW_8]  = p.u_8[DATA_LEN_BYTE_LOW_8];
       xQueueSendToBackFromISR(addr->usart_queue, addr->rx_buff_data,0);
       memset(addr->rx_buff_data,0,addr->rx_buff_size);
      HAL_UART_Receive_DMA(huartx,(addr->rx_buff_data + RX_HEAD_ADDR),addr->rx_buff_size);
     }
//ceshi-------------------
    return HAL_OK;
   }
/**
	* @Data    2019-01-16 10:24
	* @brief   ���д���
	* @param   void
	* @retval  HAL Status
	*/
	HAL_StatusTypeDef UsartQueueCreate(usartDataStrcut *usartx,uint8_t len,\
																														uint8_t deep)
	{
		usartx->usart_queue = xQueueCreate(len,deep);//�������len����21����
		if(usartx->usart_queue == NULL)
			return HAL_ERROR;
		return HAL_OK;
	}
/**
	* @Data    2019-01-16 10:54
	* @brief   ������Ӧ�����������ݵĿռ�
	* @param   huartx ��x=1,2,3,6,7��
	* @retval  HAL Status
	*/
HAL_StatusTypeDef AllocateUsartxSpace(UART_HandleTypeDef *huartx)
{
	if (huartx->Instance == USART1) 
	{
		pusart1_t	= (struct usartDataStrcut*)malloc(sizeof(struct usartDataStrcut));
		if(pusart1_t == NULL)
		return HAL_ERROR;
		return HAL_OK;
	} 
	else if (huartx->Instance == USART2) 
	{
		pusart2_t	= (struct usartDataStrcut*)malloc(sizeof(struct usartDataStrcut));
		if(pusart2_t== NULL)
		return HAL_ERROR;
		return HAL_OK;
	} 
	else if (huartx->Instance == USART3) 
	{
		pusart3_t	= (struct usartDataStrcut*)malloc(sizeof(struct usartDataStrcut));
		if(pusart3_t == NULL)
		return HAL_ERROR;
		return HAL_OK;
	} 
	else if (huartx->Instance == USART6) 
	{
		pusart6_t	= (struct usartDataStrcut*)malloc(sizeof(struct usartDataStrcut));
		if(pusart6_t == NULL)
		return HAL_ERROR;
		return HAL_OK;
	}
  else if(huartx->Instance == UART7)
  {
    pusart7_t	= (struct usartDataStrcut*)malloc(sizeof(struct usartDataStrcut));
		if(pusart7_t == NULL)
		return HAL_ERROR;
		return HAL_OK;
  }
   else if(huartx->Instance == UART8)
  {
    pusart8_t	= (struct usartDataStrcut*)malloc(sizeof(struct usartDataStrcut));
		if(pusart8_t == NULL)
		return HAL_ERROR;
		return HAL_OK;
  }
	else	return HAL_ERROR;
}
/**
	* @Data    2019-01-16 11:08
	* @brief   �Զ��б𴮿����ͻ�ȡ��Ӧ�û����ڽṹ���ַ
	* @param   huartx ��1,2,3,6,7��
	* @retval  usartDataStrcut* �û����ڽṹ��ָ��
	*/
	usartDataStrcut* GetUsartAddr(UART_HandleTypeDef *huartx)
	{
		if (huartx->Instance == USART1) 
		{
			return pusart1_t;
		} 
		else if (huartx->Instance == USART2) 
		{
			return pusart2_t;
		} 
		else if (huartx->Instance == USART3) 
		{
			return pusart3_t;
		} 
		else if (huartx->Instance == USART6) 
		{
			return pusart6_t;
		}
    else if(huartx->Instance == UART7) 
    {
			return pusart7_t;
		}
    else if(huartx->Instance == UART8) 
    {
			return pusart8_t;
		}
		else	return NULL;
	}

/**
	* @Data    2019-01-16 15:22
	* @brief   ���н���
	* @param   huartx��1,2,3,6,7��
	* @param 	 pvBuffer �������ݵ�ַ
	* @retval  HAL Status
	*/
	HAL_StatusTypeDef UserUsartQueueRX(UART_HandleTypeDef *huartx,\
																		 void* const pvBuffer)
	{
				portBASE_TYPE xStatus;
		usartDataStrcut *addr;
		addr = GetUsartAddr(huartx);
xStatus = xQueueReceive(addr->usart_queue, pvBuffer, 0);
				if(pdFAIL != xStatus)
				{
						return HAL_OK;
				}
        	return HAL_ERROR;

	}
/*-----------------------------------file of end------------------------------*/
