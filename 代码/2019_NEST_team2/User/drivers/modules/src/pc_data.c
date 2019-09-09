/**
  |-------------------------------- Copyright -----------------------------------|
  |                                                                              |
  |                        (C) Copyright 2019,����ƽͷ��,                         |
  |            1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China       |
  |                            All Rights Reserved                               |
  |                                                                              |
  |            By(GCU The wold of team | ��������ѧ����ѧԺ������Ұ�Ƕ�)          |
  |                     https://github.com/GCUWildwolfteam                       |
  |------------------------------------------------------------------------------|
  |--FileName    : pc_data.c                                                
  |--Version     : v1.0                                                            
  |--Author      : ����ƽͷ��                                                       
  |--Date        : 2019-03-21               
  |--Libsupports : STM32CubeF1 Firmware Package V1.6.0(�ñ�Ŀ�����������)
  |--Description :                                                       
  |--FunctionList                                                       
  |-------1. ....                                                       
  |          <version>:                                                       
  |     <modify staff>:                                                       
  |             <data>:                                                       
  |      <description>:                                                        
  |-------2. ...                                                       
  |---------------------------------declaration of end----------------------------|
 **/
#include "pc_data.h" 
#define JUN_ZHUN_VERSION 1
#define ZI_KAI_VERSION 0
#define PC_CHECK_BYTE (0x55)//У��λ
#if JUN_ZHUN_VERSION
#define PC_DATA_LEN (11 + HEAD_FRAME_LEN)//�������ݳ���
#elif ZI_KAI_VERSION
#define PC_DATA_LEN (20 + HEAD_FRAME_LEN)//�������ݳ���
#endif 
#define  PC_DATA_LEN_BSP (PC_DATA_LEN)
uint8_t pc_databuff[PC_DATA_LEN_BSP];
fps_t tem_fps,tem_fps1;
    /**
    * @Data    2019-03-21 00:22
    * @brief   С�������ݽ��ճ�ʼ��
    * @param   void
    * @retval  void
    */
    HAL_StatusTypeDef PcDataRxInit(pcDataStruct* pc)
    {
      pc->status = MOD_READ;
      if(pc == NULL)
      {
        return HAL_ERROR;
      }
      pc->pitch_target_angle = 0;
      pc->yaw_target_angle = 0;
      pc->commot = 0;
      pc->status = 0;
      if(UsartAndDMAInit(PC_DATA_UASRT,PC_DATA_LEN_BSP,ENABLE) != HAL_OK)
      {
        //�������
        return HAL_ERROR;
      }
      SetFpsAddress(pc->fps);
      SetFpsAddress(tem_fps);
      SetFpsAddress(tem_fps1);
      SET_BIT(pc->status,INIT_OK);
      return HAL_OK;
    }
/**
* @Data    2019-03-21 00:46
* @brief   ���ݽ���
* @param   void
* @retval  void
*/
void Pc_ParseData(pcDataStruct* pc)
{
  if(UserUsartQueueRX(PC_DATA_UASRT,pc_databuff) == HAL_OK)
  {
    if(pc_databuff[0+DATA_LEN_BYTE_LEN] =='$')
     {
       pc->shoot_commot = 1;
//       pc->yaw_target_angle = (pc_databuff[1+DATA_LEN_BYTE_LEN]&0x0f) |
//                              (pc_databuff[2+DATA_LEN_BYTE_LEN]&0x0f);
//       pc->yaw_target_angle =  pc->yaw_target_angle - OPENMV_YAW;
//       pc->pitch_target_angle=(pc_databuff[3+DATA_LEN_BYTE_LEN]&0x0f) |
//                              (pc_databuff[4+DATA_LEN_BYTE_LEN]&0x0f);                              
//       pc->distance = (pc_databuff[5+DATA_LEN_BYTE_LEN]&0x0f)|
//                      (pc_databuff[6+DATA_LEN_BYTE_LEN]&0x0f);                     
       pc->yaw_target_angle = ((pc_databuff[1+DATA_LEN_BYTE_LEN]&0x0f) *100 + \
                              (pc_databuff[2+DATA_LEN_BYTE_LEN]&0x0f)*10 + \
                              (pc_databuff[3+DATA_LEN_BYTE_LEN]&0x0f)) - OPENMV_YAW;      
       pc->pitch_target_angle=(pc_databuff[4+DATA_LEN_BYTE_LEN]&0x0f) *100 + \
                              (pc_databuff[5+DATA_LEN_BYTE_LEN]&0x0f)*10 + \
                              (pc_databuff[6+DATA_LEN_BYTE_LEN]&0x0f); 
       pc->distance = (pc_databuff[7+DATA_LEN_BYTE_LEN]&0x0f) *1000 +\
                      (pc_databuff[8+DATA_LEN_BYTE_LEN]&0x0f)*100 +\
                      (pc_databuff[9+DATA_LEN_BYTE_LEN]&0x0f)*10 + \
                      (pc_databuff[10+DATA_LEN_BYTE_LEN]&0x0f);       
     }
     else if(pc_databuff[0+DATA_LEN_BYTE_LEN] =='#')
     {
       pc->shoot_commot = 0;
     }
     SET_BIT(pc->status,RX_OK);    
     Fps(pc->fps);   
  }
  else if(GetFps(pc->fps) ==0)//û���ݻ����ص�֡
  {
    pc->pitch_target_angle = 0;
    pc->yaw_target_angle = 0;
    pc->status = 0;
    pc->distance = 0;
    pc->shoot_commot = 0;
    CLEAR_BIT(pc->status,RX_OK);
  }  
}


/**
* @Data    2019-03-21 00:46
* @brief   С���������˳�
* @param   void
* @retval  void
*/
uint8_t esc;
//int16_t pc_red =0;
void EscPc(int16_t key,int16_t ch1,int16_t ch2,int16_t ch3,int16_t ch4,int16_t thumbwheel,int16_t key1)
{
  if(key ==2 && ch1 < -600 && ch2 <-600 && ch3 >600 && ch4 <-600)
  {
//    if( (ch1 < -600) && \
//        (ch2 <-600) && \
//        (ch3 >600) && \
//        (ch4 <-600) )//�ڰ����ñ��������
    esc= 'q';
  }
   // printf("q");
  else if(key1 ==2&&thumbwheel >600)
  {
        esc= 'r';
  }
  else if(key1 ==2&& thumbwheel <-600)
  {
    esc= 'b';
  }
  else 
  {
    if(esc == 'q')
    esc = 'r';
  }
  if(HAL_UART_Transmit(PC_DATA_UASRT,&esc,1,1) ==HAL_OK)
      Fps(tem_fps1);
   // HAL_UART_Transmit(PC_DATA_UASRT,'r',1,0);
  //printf("r");

}
/*------------------------------------file of end-------------------------------*/


