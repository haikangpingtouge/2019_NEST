/**
	|----------------------------- Copyright ------------------------------------|
	|                                                                            |
	|                        (C) Copyright 2018,����ƽͷ��,                       |
	|        1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China         |
	|                         All Rights Reserved                                |
	|                                                                            |
	|         By(GCU The wold of team | ��������ѧ����ѧԺ������Ұ�Ƕ�)           |
	|                  https://github.com/GCUWildwolfteam                        |
	|----------------------------------------------------------------------------|
	|--FileName    : baseclass.c                                                
	|--Version     : v1.0                                                            
	|--Author      : ����ƽͷ��                                                       
	|--Date        : 2018-12-30               
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
#include "baseclass.h " 
/* ----------------- ��־λ�������� -------------------- */
	unsigned int e_periphera_interface_flag = 0; 	//����ӿ�״̬��������
	unsigned int e_periphera_error_flag = 0;		//����ӿڴ���״̬��������
	unsigned int e_objects_flag = 0;     //����ʹ�ñ�־
	/**
	* @Data    2018-12-30 17:23
	* @brief   ��ȡ��������ָ��
	* @param   void* 
	* @retval  void*
	*/
	void* Get_Peripheral_type(void* Peripheral)
	{
		return Peripheral;
	}
	/**
	* @Data    2018-12-30 21:14
	* @brief   ʶ�𴮿�����,������Ӧ���ڱ�־״̬λ
	* @param   ��������ָ��huartx(x=1��2��3��4��5��6)
	* @retval  USART_TypeDef 
	*/
	UART_HandleTypeDef* RecognizeUSARTType(UART_HandleTypeDef* huartx)
	{
		if (huartx->Instance == USART1) 
		{
			SET_BIT(e_periphera_interface_flag,USART1_BY_USED);
			return huartx;
		}
		else if (huartx->Instance == USART2) 
		{
			SET_BIT(e_periphera_interface_flag,USART2_BY_USED);
			return huartx;
		}
		else if (huartx->Instance == USART3) 
		{
			SET_BIT(e_periphera_interface_flag,USART3_BY_USED);
			return huartx;
		}
		else if(huartx->Instance == UART4)
		{
			SET_BIT(e_periphera_interface_flag,UART4_BY_USED);
			return huartx;
		}
		else if (huartx->Instance == UART5)
		{
			SET_BIT(e_periphera_interface_flag,UART5_BY_USED);
			return huartx;
		}
		else return NULL;

	}

	/**
	* @Data    2018-12-30 21:14
	* @brief   ʶ��can����,������Ӧcan��־״̬λ
	* @param   ��������ָ��hcanx(x=1��2)
	* @retval  CAN_HandleTypeDef 
	*/
CAN_HandleTypeDef* RecognizeCanType(CAN_HandleTypeDef* hcanx)
{
	if (hcanx->Instance == CAN1) 
	{
		SET_BIT(e_periphera_interface_flag,CAN1_BY_USED);
		return hcanx;
	} 
#if HAL_F4 
	else if (hcanx->Instance == CAN2) 
	{
		SET_BIT(e_periphera_interface_flag,CAN2_BY_USED);
		return hcanx;
	} 
#endif 
	else return NULL;
}
	/**
	* @Data    2018-12-30 22:16
	* @brief   ��֡��־λ����
	* @param   int16_t flag
	* @retval  void
	*/
	void SetFrameDropflag(int16_t flag)
	{
		flag = 0;
		flag--;
		if(flag < -10)
		{
			SET_BIT(e_periphera_error_flag,RX_ERROR);
			flag = 0;
		}
	}
	/**
	* @Data    2018-12-30 22:16
	* @brief   ������־λ����
	* @param   int16_t flag
	* @retval  void
	*/
	void SetNormalflag(int16_t flag)
	{
		if(flag > 0)
		flag =1;
	}
/**
* @Data    2019-01-16 16:01
* @brief   RCRУ�����
* @param   uint8_t* pdata �����׵�ַ  ����һλ�ǵͰ�λ �����ڶ�λ�Ǹ߰�λ
* @param   uint8_t size ���ݳ���
* @retval  HAL Status
*/
HAL_StatusTypeDef RCREncryption(uint8_t* pdata,uint8_t size)
{
	if(size<4)
		return HAL_ERROR;
	uint8_t i;
	uint16_t sum;
	unsigned char *p;
	pdata[size-3] = 0x55;
	for (i = 0; i < (size-2); i++)
	{
		sum += pdata[i];
	}
	p=(unsigned char *)&sum;
	pdata[size-1] = (unsigned char)*p;//�Ͱ�λ
	pdata[size-2] = (unsigned char)*(p+1);//�߰�λ
	return HAL_OK;
}
	/**
	* @Data    2019-01-16 16:38
	* @brief   RCRУ�����
	* @param   uint8_t* pdata �����׵�ַ
	* @param   uint8_t size ���ݳ���
	* @retval  HAL Status
	*/
	HAL_StatusTypeDef RCRDecryption(uint8_t* pdata,uint8_t size)
	{
		uint8_t i;
		uint16_t sumen;//����ǰ��ֵ
		uint16_t sumde;//���ܺ��ֵ
		for (i = 0; i < (size-2); i++)
		{
			sumde += pdata[i];
		}
		sumen = (uint16_t)(pdata[size - 2] << 8 | pdata[size - 1]);
		if (pdata[size - 3] == 0x55 && sumen == sumde)
			return HAL_OK;
		return HAL_ERROR;
	}
	/**
		* @Data    2019-02-14 15:09
		* @brief   ���ֽ�����ת���ֽ�����,��λ��ǰ
		* @param   uint32_t �ĸ��ֽ�
		* @param	 uint8_t *���ֽ�ָ�� 
		* @retval  void
		*/
		void MultibyteToByle(uint32_t data,uint8_t *pdata)
		{
			unsigned char *p;
			p = (unsigned char *)&data;
			*pdata = (unsigned char)*(p + 3);//�߰�λ
			*(pdata+1)= (unsigned char)*(p + 2);
			*(pdata + 2) = (unsigned char)*(p + 1);
			*(pdata + 3) = (unsigned char)*p;//�Ͱ�λ
		}
		/**
		* @Data    2019-02-14 15:09
		* @brief   �����ֽ�����ת���ֽ�����,��λ��ǰ
		* @param   uint32_t �ĸ��ֽ�
		* @param	 uint8_t *���ֽ�ָ�� 
		* @retval  void
		*/
		void TwobyteToByle(int16_t data,uint8_t *pdata)
		{
			unsigned char *p;
			p = (unsigned char *)&data;
			*pdata = (unsigned char)*(p + 1);//�߰�λ
			*(pdata+1)= (unsigned char)*p;//�Ͱ�λ
		}
	/**
		* @Data    2019-02-14 15:45
		* @brief   ���ֽ�����ת�ĸ��ֽڣ���λ��ǰ
		* @param	 uint8_t *���ֽ�ָ�� 
		* @param   uint32_t *�ĸ��ֽ�ָ��
		* @retval  void
		*/
		void ByleToMultibyte(uint8_t *pdata,uint32_t *data)
		{
			*data = (uint32_t)((*pdata << 24) | (*(pdata + 1) << 16) | \
							(*(pdata + 2) << 8) |(*(pdata + 3)));
		}
	/**
		* @Data    2019-02-16 13:42
		* @brief   ˳���������
		* @param   uint8_t size ����ĳ���
		* @retval  void
		*/
		void DataFilling(uint8_t *pdata,uint8_t s,uint8_t size)
		{
			uint8_t i;
			for(i = 0; i < size; i++)
			{
				*(pdata+i) = s;
			}
		}
/*--------------------------------file of end-------------------------------*/
