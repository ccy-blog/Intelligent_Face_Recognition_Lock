#include "stdint.h"
#include "bsp.h"
#include "wificonfig.h"
#include "usart.h"
#include "string.h"
#include "stm32f0xx_hal.h"
//#include "fs_protocol.h"

#define TimeDelay  100

uint8_t NowWifiConfig = 2;  //�Ƿ��洦������ģʽ��Wifi���úͽ�������ģʽ

/*****************************ATָ��*******************************************/
static uint8_t *At_QuitCipsend = "+++";                //�˳�͸��
static uint8_t *At_CipClose = "AT+CIPCLOSE\r\n";       //�Ͽ����� 
static uint8_t *At_CwqAp = "AT+CWQAP\r\n";		//�Ͽ�AP����
static uint8_t *At_CwMode = "AT+CWMODE?\r\n";	        //��ѯ����ģʽ
static uint8_t *At_Restore= "AT+RESTORE\r\n";		//�ظ���������
static uint8_t *At_CwMode3 = "AT+CWMODE=3\r\n";        //����ΪAP��STATIONģʽ



static uint8_t *At_Cifsr = "AT+CIFSR\r\n";       //��ȡ����IP


uint8_t *SSID = "1002";     //����WiFI��SSID

//uint8_t At_CwjAp[50] = {0};

//uint8_t *At_CwjAp = "AT+CWJAP=\"11111111\",\"11111111\"\r\n"; //����AP
uint8_t *At_CwjAp = "AT+CWJAP=\"fssy7052\",\"fssy7052\"\r\n"; //����AP
static uint8_t *At_CipMux = "AT+CIPMUX=0\r\n";	        //���õ�����	
//uint8_t *At_CipStart = "AT+CIPSTART=\"UDP\",\"192.168.0.15\",8080,20000,0\r\n";//����UDP����  


//static uint8_t *At_CipStart = "AT+CIPSTART=\"UDP\",\"192.168.0.15\",8080,20000,0\r\n";//����UDP����  //old:
//static uint8_t *At_CipStart = "AT+CIPSTART=\"UDP\",\"192.168.137.126\",8080,20000,0\r\n";//����UDP����  //new:
static uint8_t *At_CipStart = "AT+CIPSTART=\"UDP\",\"192.168.31.19\",8080,20000,0\r\n";//����UDP����  //new:

static uint8_t *At_CipMode = "AT+CIPMODE=1\r\n";       //����͸��ģʽ
static uint8_t *At_CipSend = "AT+CIPSEND\r\n";	        //����͸��

//ATָ���Ӧ�Ļظ�
static uint8_t * At_CwMode_Ack = "AT+CWMODE?\r\r\n+CWMODE:2\r\n\r\nOK\r\n";//����Ϊ��APģʽ  
//uint8_t At_CwjAp_Ack_V1[50] = {0};
static uint8_t *At_CwjAp_Ack_V1 = "AT+CWJAP=\"fssy7052\",\"fssy7052\"\r\r\n";
//static uint8_t *At_CwjAp_Ack_V1 = "AT+CWJAP=\"11111111\",\"11111111\"\r\r\n";
static uint8_t *At_CwjAp_Ack_V2 = "WIFI CONNECTED\r\r\n";
static uint8_t *At_CwjAp_Ack_V3 = "WIFI GOT IP\r\r\n";
static uint8_t *At_CwjAp_Ack_V4 = "\r\nOK\r\n";
//static uint8_t *At_CwqAp_Ack = "AT+CWQAP\r\r\n\r\nOK\r\n";
static uint8_t *At_CipSend_Ack = "AT+CIPSEND\r\r\n\r\nOK\r\n\r\n>";	    //����͸��Ӧ��
static uint8_t *At_QuitCipsend_Ack = "+++";                //�˳�͸��

uint8_t StationIP[14] = {0};


/***********************************************************************************************************
* ��������: void Reset_RxbuffClear(void)
* �������: 
*               
* ���ز���: 
*
* ��    ��: --
*
* ��    ��: 2017/2/22, by SongLei
* ��    ��: 1170152143@qq.com
************************************************************************************************************/
void Reset_RxbuffClear(void)
{
  UsartType2.receive_flag = 0;//������
  
#if (DEBUG_PRINTF == TRUE) 
  printf("WiFi:%s\n",UsartType2.usartDMA_rxBuf);
#endif
  
  memset(UsartType2.usartDMA_rxBuf,0,strlen(UsartType2.usartDMA_rxBuf));
  
  NetWorkLedstatusOnOff(NetWorkState_LED4, NetWorkState_LED4_Flag);
  NetWorkState_LED4_Flag = (~NetWorkState_LED4_Flag);
}
/***********************************************************************************************************
* ��������: void Wifi_Config(void)
* �������: 
*               
* ���ز���: 
*
* ��    ��: ʹ�ô��ڷ�ʽ����wifi��ʵ��AT͸��ģʽ��UDP��
*
* ��    ��: 2017/2/23, by SongLei
* ��    ��: 1170152143@qq.com
************************************************************************************************************/
void Wifi_Config(void)
{
  uint8_t i=0;
  //��Ϊ����USART DMAʱ���״ε���һ��HAL_UART_RxCpltCallback();������0
  
  UsartType2.receive_flag = 0;
  
  //͸��ģʽ��û�п�ʼ��������������WiFi����Ĭ�Ͽ������л��Թ���
  NowWifiConfig = 0;
    /****************** �ָ��������� *************************/  
  if(Wifi_RESTORE() ==1 )
  {
    memset(UsartType2.usartDMA_rxBuf,0,1024);
    printf("�֏ͳ��S�O�� OK\n");
  }else printf("�֏ͳ��S�O�� Faile\n");
  
REST:
  HAL_Delay(100);  //�ȴ�WiFi��λ��ɣ�wifi��λʱ���ӡһ��������Ϣ��
  Reset_RxbuffClear();
  /******************start �˳�͸��*************************/
  HAL_UART_Transmit(&huart2, At_QuitCipsend, strlen((char*)At_QuitCipsend), 0xffff);
  while(UsartType2.receive_flag != 1);
  if (0 != strncmp((char*)UsartType2.usartDMA_rxBuf, (char*)At_QuitCipsend_Ack, strlen((char*)At_QuitCipsend_Ack))) 
  {
    Reset_RxbuffClear(); 
    goto REST;
  }
  else
  {
    Reset_RxbuffClear(); 
  }
  
  HAL_UART_Transmit(&huart2, At_QuitCipsend, strlen((char*)At_QuitCipsend), 0xffff);
  while(UsartType2.receive_flag != 1);
  Reset_RxbuffClear();
  HAL_UART_Transmit(&huart2, At_QuitCipsend, strlen((char*)At_QuitCipsend), 0xffff);
  while(UsartType2.receive_flag != 1);
  Reset_RxbuffClear();
  /****************** �Ͽ��ϴ����� *************************/
  HAL_UART_Transmit(&huart2, At_CipClose, strlen((char*)At_CipClose), 0xffff);
  while(UsartType2.receive_flag != 1);
  Reset_RxbuffClear();
  HAL_UART_Transmit(&huart2, At_CipClose, strlen((char*)At_CipClose), 0xffff);
  while(UsartType2.receive_flag != 1);
  Reset_RxbuffClear();
  HAL_UART_Transmit(&huart2, At_CipClose, strlen((char*)At_CipClose), 0xffff);
  while(UsartType2.receive_flag != 1);
  Reset_RxbuffClear();
  
  /****************** �Ͽ���AP���� *************************/
  HAL_UART_Transmit(&huart2, At_CwqAp, strlen((char*)At_CwqAp), 0xffff);
  HAL_Delay(TimeDelay);
  while(UsartType2.receive_flag != 1);
  Reset_RxbuffClear();
  HAL_UART_Transmit(&huart2, At_CwqAp, strlen((char*)At_CwqAp), 0xffff);
  HAL_Delay(TimeDelay);
  while(UsartType2.receive_flag != 1);
  Reset_RxbuffClear();
  
err:
  
  /****************** ����AP+ATATIONģʽ *******************/
  HAL_UART_Transmit(&huart2, At_CwMode3, strlen((char*)At_CwMode3), 0xffff);
  while(UsartType2.receive_flag != 1);
  HAL_Delay(TimeDelay);
  Reset_RxbuffClear();
  
  /****************** ��ѯ����ģʽ *************************/
  HAL_UART_Transmit(&huart2, At_CwMode, strlen((char*)At_CwMode), 0xffff);
  while(UsartType2.receive_flag != 1);
  //  UsartType2.receive_flag = 0;
  //  HAL_Delay(TimeDelay);
  if (0 == strncmp((char*)UsartType2.usartDMA_rxBuf, (char*)At_CwMode_Ack, strlen((char*)At_CwMode_Ack))) 
  {//����ΪAPģʽ
    HAL_UART_Transmit(&huart2, At_Restore, strlen((char*)At_Restore), 0xffff);
    __HAL_UART_DISABLE_IT(&huart2,UART_FLAG_RXNE);
    HAL_Delay(4000);
    MX_USART1_UART_Init();
    printf("Restore to factory settings\n");
    __HAL_UART_ENABLE_IT(&huart2,UART_FLAG_RXNE);
    Reset_RxbuffClear(); 
    goto REST;
  }
  else
  {
    Reset_RxbuffClear(); 
  }
  Reset_RxbuffClear(); 
  
  //snprintf((char*)At_CwjAp ,50,"AT+CWJAP=\"farsight_%s\",\"1234567890123\"\r\n",SSID);//old:
  //snprintf((char*)At_CwjAp ,50,"AT+CWJAP=\"fssy7051\",\"fssy7051\"\r\n",SSID);//new
  /****************** ���� AP ******************************/
  HAL_UART_Transmit(&huart2, At_CwjAp, strlen((char*)At_CwjAp), 0xffff);
  
  // snprintf((char*)At_CwjAp_Ack_V1 ,50,"AT+CWJAP=\"farsight_%s\",\"1234567890123\"\r\r\n",SSID);//old:
  //snprintf((char*)At_CwjAp_Ack_V1 ,50,"AT+CWJAP=\"fssy7051\",\"fssy7051\"\r\r\n");//new
  
    while(UsartType2.receive_flag != 1);
    while(!strstr(UsartType2.usartDMA_rxBuf,At_CwjAp_Ack_V1));
   {

//     HAL_Delay(50);
   }
  i=7;
  while(i--)  //ѭ��7��
  {
    if (0 == strncmp((char*)UsartType2.usartDMA_rxBuf, (char*)At_CwjAp_Ack_V1, strlen((char*)At_CwjAp_Ack_V1))) 
    {
      //printf("connection AP Success\n");
      Reset_RxbuffClear();
      while(UsartType2.receive_flag != 1);
      if (0 == strncmp((char*)UsartType2.usartDMA_rxBuf, (char*)At_CwjAp_Ack_V2, strlen((char*)At_CwjAp_Ack_V2))) 
      {
        //printf("connection AP Success\n");
        Reset_RxbuffClear();
      }
      Reset_RxbuffClear();
      while(UsartType2.receive_flag != 1);
      if (0 == strncmp((char*)UsartType2.usartDMA_rxBuf, (char*)At_CwjAp_Ack_V3, strlen((char*)At_CwjAp_Ack_V3))) 
      {
        //printf("connection AP Success\n");
        Reset_RxbuffClear();
      }
      Reset_RxbuffClear();
      while(UsartType2.receive_flag != 1);
      if (0 == strncmp((char*)UsartType2.usartDMA_rxBuf, (char*)At_CwjAp_Ack_V4, strlen((char*)At_CwjAp_Ack_V4))) 
      {
        //printf("connection AP Success\n");
        Reset_RxbuffClear();
      }
      break;
    }
    else
    {
      Reset_RxbuffClear();
      printf("connection AP Fail %d\n",i);
      HAL_Delay(1000);
      if(i == 1)
        goto err;
    }
  }
  /****************** ���õ�����ģʽ *******************/
  HAL_UART_Transmit(&huart2, At_CipMux, strlen((char*)At_CipMux), 0xffff); 
  HAL_Delay(TimeDelay);
  Reset_RxbuffClear();
  //    /****************** �Ͽ� UDP ���� *******************/
  HAL_UART_Transmit(&huart2, At_CipClose, strlen((char*)At_CipClose), 0xffff); 
  HAL_Delay(TimeDelay);
  Reset_RxbuffClear();
  /****************** ���� UDP ���� *******************/
  HAL_UART_Transmit(&huart2, At_CipStart, strlen((char*)At_CipStart), 0xffff);
  HAL_Delay(1000);
  Reset_RxbuffClear(); 
  /****************** ��ѯIP *******************/
  Wifi_Read_IP(); 
  HAL_Delay(TimeDelay);
  Reset_RxbuffClear();
  /****************** ����͸��ģʽ *******************/
  HAL_UART_Transmit(&huart2, At_CipMode, strlen((char*)At_CipMode), 0xffff);
  HAL_Delay(TimeDelay);
  Reset_RxbuffClear();

  /****************** ����͸��ģʽ *******************/
  HAL_UART_Transmit(&huart2, At_CipSend, strlen((char*)At_CipSend), 0xffff);
  HAL_Delay(TimeDelay);
  if (0 == strncmp((char*)UsartType2.usartDMA_rxBuf, (char*)At_CipSend_Ack, strlen((char*)At_CipSend_Ack))) 
  {
#if (DEBUG_PRINTF == TRUE) 
    printf("Start Transparent Success\n");
#endif
    Reset_RxbuffClear();
  }
  else
  {
#if (DEBUG_PRINTF == TRUE) 
    printf("Start Transparent Fail\n");
#endif
    goto REST;
  }
  
  NetWorkState_LED2_Flag = (~NetWorkState_LED2_Flag);
  NetWorkState_LED3_Flag = (~NetWorkState_LED3_Flag);
  //�ر��շ�LED�ĵ�
  NetWorkLedstatusOnOff(NetWorkState_LED2, NetWorkState_LED2_Flag);
  NetWorkLedstatusOnOff(NetWorkState_LED3, NetWorkState_LED3_Flag);
  
  //����͸��ģʽ
  NowWifiConfig = 1; 
  
  UsartType2.receive_flag = 0;
}
void Wifi_Read_IP(void)
{
  uint8_t i=0;
  HAL_UART_Transmit(&huart2, At_Cifsr, strlen((char*)At_Cifsr), 0xffff);
  HAL_Delay(TimeDelay);
  // if (0 == strncmp((char*)UsartType2.usartDMA_rxBuf, (char*)At_CipSend_Ack, strlen((char*)At_CipSend_Ack))) 
  //  strstr(usartDMA_rxBuf,STAIP)
  char delims[] = "\"";
  char *result = NULL;
  result = strtok( (char*)UsartType2.usartDMA_rxBuf, delims );
  //result = "192.168.1.1";
  while( result != NULL ) {
//    printf("result is \"%s\"\n", result );
    if(i++ == 5 )
    {  
//        StationIP = result;
      
     //rcm add 
      //memcpy(StationIP,result,strlen(result));//old
     memcpy(StationIP,result,strlen(result));//new
//    printf("StationIP:%s\n", StationIP );
    }   
    result=strtok(NULL,delims);
  }
}
uint8_t Wifi_RESTORE(void)//wifi�ָ���������
{
REST: 
  /******************start �˳�͸��*************************/
  HAL_UART_Transmit(&huart2, At_QuitCipsend, strlen((char*)At_QuitCipsend), 0xffff);
  while(UsartType2.receive_flag != 1);
  if (0 != strncmp((char*)UsartType2.usartDMA_rxBuf, (char*)At_QuitCipsend_Ack, strlen((char*)At_QuitCipsend_Ack))) 
  {
    Reset_RxbuffClear(); 
    goto REST;
  }
  else
  {
    Reset_RxbuffClear(); 
  }
  
  HAL_UART_Transmit(&huart2, At_Restore, strlen((char*)At_Restore), 0xffff);
  
//   while(UsartType2.receive_flag != 1);
//   while(!strstr((char *)UsartType2.usartDMA_rxBuf,"@ 1000"))
//   {
//     Reset_RxbuffClear();
//     while(UsartType2.receive_flag != 1);
//   }

   __HAL_UART_DISABLE(&huart2);
   __HAL_UART_DISABLE_IT(&huart2,UART_IT_IDLE); 
   
   HAL_Delay(1000);
   Reset_RxbuffClear();
   __HAL_UART_ENABLE(&huart2);
   __HAL_UART_ENABLE_IT(&huart2,UART_IT_IDLE);
   Reset_RxbuffClear();   
//    while(UsartType2.receive_flag != 1);
//    while(!strstr((char *)UsartType2.usartDMA_rxBuf,"ready"))
//   {
//     Reset_RxbuffClear();
//     HAL_Delay(100);
//     while(UsartType2.receive_flag != 1);
//   }
//   Reset_RxbuffClear();
   return 1; 


}

