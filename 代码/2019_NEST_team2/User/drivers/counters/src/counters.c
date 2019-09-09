/**
	|----------------------------- Copyright ------------------------------------|
	|                                                                            |
	|                     (C) Copyright 2018,����ƽͷ��,                          |
	|         1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China        |
	|                  -       All Rights Reserved                               |
	|                                                                            |
	|          By(GCU The wold of team | ��������ѧ����ѧԺ������Ұ�Ƕ�)          |
	|                  https://github.com/GCUWildwolfteam                        |
	|----------------------------------------------------------------------------|
	|--FileName    : counters.c                                                
	|--Version     : v1.0                                                            
	|--Author      : ����ƽͷ��                                                       
	|--Date        : 2018-11-27               
	|--Description :                                                       
	|--FunctionList                                                       
	|-------1. ....                                                       
	|          <version>:                                                       
	|     <modify staff>:                                                       
	|             <data>:                                                       
	|      <description>:                                                        
	|-------2. ...                                                       
	|-----------------------------declaration of end-----------------------------|
 **/
#include "counters.h" 
#include <Math.h>
/* ===================== Bezier(����������) of begin ===================== */
//  /**
// 	* @Data    2019-02-23 11:45
// 	* @brief   ���������߽ṹ���ʼ��
// 	* @param   void
// 	* @retval  void
// 	*/
// 	void BezierStructInit(bezierStruct* bs)
// 	{
// 		bs->coordinate_x = NULL;
// 		bs->coordinate_y = NULL;
// 		bs->n = 4;
// 		bs->precision = 1000;
// 	}
	/**
		* @Data    2019-02-21 16:38
		* @brief   ��������㴢��ռ�
		* @param   void
		* @retval  void
		*/
		HAL_StatusTypeDef CreateDataSpace(bezierStruct* bs)
		{
			uint32_t i = 0;
			float t=0;
			bs->coordinate_x	= (uint32_t*)malloc(sizeof(uint32_t)*(bs->precision+3));
			bs->coordinate_y	= (uint32_t*)malloc(sizeof(uint32_t)*(bs->precision+3));
			if(bs->coordinate_x == NULL || bs->coordinate_x == NULL)
			{
				free(bs->coordinate_x);
				free(bs->coordinate_y);
	      return HAL_ERROR;
			}
			
			for(i = 0;i<(bs->precision+1);i++)
			{
				t = i/bs->precision;
				*(bs->coordinate_x+i) = QuadTo(bs->n,bs->point_x,t);
			  *(bs->coordinate_y+i) = QuadTo(bs->n,bs->point_y,t);
			}
			return HAL_OK;
		}
	/**
	* @Data    2019-02-21 14:08
	* @brief   n�ױ����������������� 
							Bn(t) =(1?t)nP0+��i=1n?1nti(1?t)n?iPi+tnPn(n��2,n��Z)
	* @param   n ����������
	* @param   *point  ���Ƶ��ַ
	* @param  t ����ϵ��
	* @retval uint32_t ����ó�����x��y��ֵ
	*/
	uint32_t QuadTo(const uint8_t n,const uint32_t *point,float t)
	{
		if(n < 2 && n < 8)
		{
			return 0;
		}
		uint8_t i = 1;
		float sum_Pi = 0;
		for(i = 1;i < n;i++)
		{
			sum_Pi += n *pow(t,i) * pow((1-t),(n-i)) * (*(point+i));
		}
		return (uint32_t) \
						((pow((1-t),n) * (*(point))) + sum_Pi +( pow(t,n) * (*(point+n))));
	}
//	/**
//		* @Data    2019-02-19 11:40
//		* @brief   ƽ������䳤�ȼ���
//		* @param   ��Q1(x,y) = (Q1[0],Q1[1])
//		* @param	 ��Q2(x,y) = (Q2[0],Q2[1])
//		* @retval  int16_t ���� ��λmm
//		*/
//		int16_t GetLengthForTwopoint(const int16_t *Q1,const int16_t *Q2)
//		{
//			return (int16_t)(sqrt(pow((Q1[X] + Q2[X]),2) + pow((Q1[Y] + Q2[Y]),2)));
//		}
/* ====================== Bezier(����������) of end ====================== */
/* =========================== PID������ of begin =========================== */
/**
	* @Data    2019-01-26 16:55
	* @brief   λ��pid������
	* @param  postionPidStruct λ��pid�ṹ��ָ��
	* @param  ��ǰ���
	* @retval  int16_t
	*/
	int16_t PostionPid(postionPidStruct *pps,int16_t error)
	{
	  pps->error	= error;
    //pps->kp = KpSeparationCallback(pps);
		pps->pout = pps->kp * pps->error;
    pps->pout = MAX(pps->pout,pps->motor_lim);
    pps->pout = MIN(pps->pout,(-pps->motor_lim));
		pps->iout =  IntegralSeparationCallback(pps);
		pps->dout = pps->kd * (pps->error - pps->last_error);
		pps->last_error = pps->error;
    pps->pid_out = MAX(((int32_t)(pps->pout + pps->iout + pps->dout)),pps->motor_lim);
    pps->pid_out = MIN(((int32_t)(pps->pout + pps->iout + pps->dout)),(-pps->motor_lim));
		return pps->pid_out;
	}
__weak int16_t IntegralSeparationCallback(postionPidStruct *pps)
{
  if(pps->integral_threshold ==0)
    return 0;
    pps->integral_er += pps->error;
    pps->integral_er = MAX(pps->integral_er,pps->integral_limint);
    pps->integral_er = MIN(pps->integral_er,(-pps->integral_limint));
  if(ABS(pps->error) > pps->integral_threshold)
  {
    pps->integral_er = 0;
  }
  return  pps->ki * pps->integral_er;
}
__weak int16_t KpSeparationCallback(postionPidStruct *pps)
{
  if(pps->kp_separatecmd ==1)
  {
    if(ABS(pps->error) >43)
    {
      pps->kp = 220;
    }
    else pps->kp = (int16_t)(-0.015 *(float)(pps->error*pps->error) + 245);
  }
  return pps->kp;
}
/**
	* @Data    2019-01-26 16:55
	* @brief   �ٶ�pid������
	* @param  speedPidStruct �ٶ�pid�ṹ��ָ��
	* @param  ��ǰ���
	* @retval  int16_t
	*/
	int16_t SpeedPid(speedPidStruct *sps,int16_t error)
	{
		if(sps==NULL)
		 return 0;
		sps->error = error;
//		if((ABS(sps->error)) < (ABS(sps->last_error)))
//			sps->last_error = sps->error;
//		if((ABS(sps->last_error)) < (ABS(sps->before_last_error)))
//			sps->before_last_error = sps->last_error;
		sps->pout = (int32_t)(sps->kp * (sps->error - sps->last_error));
    sps->pout = MAX(sps->pout,sps->motor_lim);
    sps->pout = MIN(sps->pout,-sps->motor_lim);
    
		sps->iout = (int32_t)(sps->ki * sps->error);
    sps->iout = MAX(sps->iout,sps->motor_lim);
    sps->iout = MIN(sps->iout,-sps->motor_lim);
    
		sps->dout = (int32_t)(sps->kd * (sps->error - 2*sps->last_error + \
													 sps->before_last_error));
    sps->dout = MAX(sps->dout,sps->motor_lim);
    sps->dout = MIN(sps->dout,-sps->motor_lim);
    
		sps->pid_out += (int32_t)(sps->pout + sps->iout + sps->dout);
    sps->pid_out = MAX(sps->pid_out,sps->motor_lim);
    sps->pid_out = MIN(sps->pid_out,(-sps->motor_lim));
    
    sps->pid_out = MAX(sps->pid_out,sps->limiting);
    sps->pid_out = MIN(sps->pid_out,(-sps->limiting));
    
    sps->before_last_error = sps->last_error;
		sps->last_error = sps->error;
		return (int16_t)sps->pid_out;
	}
/* ============================ PID������ of end ============================ */

/* =========================== �Ӽ��� of begin =========================== */
/**
	* @Data    2019-03-30 16:48
	* @brief   ֱ�߼���
	* @param   int16_t
	* @retval  void
	*/
	int16_t LineAcc(int16_t r,int16_t t,int16_t p)
	{
		if(r < t)
		{
			r -= p;
			return r;
		}
		else return r;
	}
  
/* =========================== �Ӽ��� of end =========================== */

void Insert(int16_t fx[], int16_t x[])
{
	int i, j, a, b = 2;
	for (i = 0; i<2; i++)
	{
		a = b;
		for (j = 2; j>i; j--)
		{
			fx[j] = (fx[j] - fx[j - 1]) / (x[j] - x[--a]);
		}
		b--;
	}
}
int16_t NewtonInterpolation(int16_t fx[], int16_t x[], int16_t a)
{
	int16_t i;
	int16_t sum;
	int16_t cc = 1;
	sum = fx[0];
	for (i = 1; i<3; i++)
	{
		cc = cc*(a - x[i - 1]);
		sum = sum + fx[i] * cc;
	}
	return sum;
}
/*--------------------------------file of end---------------------------------*/


