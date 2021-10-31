#include "string.h"
#include "lcd.h"
#include "fs_protocol.h"
#include "bsp.h"
#include "usart.h"
#include "switch.h"

#include "adc.h"
#include "p9813.h"
#include "dht11.h"
#include "mpu9250.h"
#include "mpu6050.h"
#include "ultrasonic.h"
#include "bmp180.h"


uint8  SensorStatusflag = '0';  //�������͵ĵ�ǰ״̬��1/0��  
uint8  g_SendDataLength = 0;  //���淢�͵����ݳ���

//�������ݵĻ�����
uint8 theMessageData[OFFSET + SENDDATALENGTH +1] = {HEADER,0x00,OFFSET,DATATYPEDATA,NWKTYPE,0x00,0x00,0x00,0x00};


int8 HeartRateNumber = 0;

int8 find_sensor_id(uint16 sensor_id);

void num_to_str(unsigned int num, unsigned char *pstr, unsigned char num_len,uint8 type)
{
  int8 dispnum[10] = {0};      
  unsigned char  count = 0, i = 0;
  uint8 number = 0;
  
  //����0�������
  if(num == 0)
  {
    while (num_len)
    {
      if(num_len == 1)
        pstr[i++] = '0';  //��ʾ�ĸ��ֽ�
      else
        pstr[i++] = ' '; 
      num_len--;
    }
    pstr[i++] = '\0'; 
    return;
  }
  if(type == 10){
    //����0������£�ת����ʮ�����ַ�
    while(num != 0)
    {           
      dispnum[count] = num % 10 + '0';   
      num = num / 10;
      count++;
    }
  }else if(type == 16)
  {
    //����0������£�ת����16�����ַ�
    while(num != 0)
    {           
      dispnum[count] = num % 16;
      
      if( 0 <= dispnum[count] && dispnum[count] <= 9)
      {
        dispnum[count] += '0';
      }
      else if(10<= dispnum[count] && dispnum[count] <= 15)
      {
        dispnum[count] += 55;
      }
      num = num / 16;
      count++;
    }
  }
  
  //�������4��������ʹ�á� �����
  number = num_len - count;
  
  while(number--)
  {
    if(type == 10){  //10������䡮 ��
    pstr[i++] = ' ';
    }
    if(type == 16){  //16������䡮 ��
    pstr[i++] = '0';
    }
  }
  while (count--) //ת���������λ�ڸ��ֽ�
    pstr[i++] = dispnum[count];
  //���'\0'
  pstr[i++] = '\0';
}

/******************************************************************************
* Name:    CRC-8               x8+x2+x+1
* Poly:    0x07
* Init:    0x00
* Refin:   False
* Refout:  False
* Xorout:  0x00
* Note:
*****************************************************************************/
uint8 crc8(uint8 *data, uint8 length)
{
  uint8 i;
  uint8 crc = 0;        // Initial value
  while(length--)
  {
    crc ^= *data++;        // crc ^= *data; data++;
    for ( i = 0; i < 8; i++ )
    {
      if ( crc & 0x80 )
        crc = (crc << 1) ^ 0x07;
      else
        crc <<= 1;
    }
  }
  return crc;
}
/***********************************************************************************************************
* ��������:  EndGettAnalyzeProtocol(SendData_t *PacktData,uint16 Light)
* �������: 
*               
* ���ز���: 
*
* ��    ��: ����Э�������͵�����
*
* ��    ��: 2017/2/10, by SongLei
* ��    ��: 1170152143@qq.com
************************************************************************************************************/
void  EndGettAnalyzeProtocol(SendData_t *PacktData,uint16 Light)
{
  uint8 Crc8Data;
  uint8 status = 0,i=0;
  uint16_t RxDeviceAddr = 0x0000;
  
  //��֤�����Ƿ���ȷ 
  Crc8Data = crc8((uint8 *)PacktData, Light-1);

  if(Crc8Data == PacktData->Data[PacktData->DataLength])
  {     
    //HalUARTWrite(0,"CRC8 Success!",13);
    printf("CRC8 Success!\n");
  }
  else
  {
    //HalUARTWrite(0,"CRC8 Failed!",12);
  //  printf("CRC8 Failed!\n");
    return ;
  }

#ifdef DEBUG_PRINTF  
 for(i=0;i<Light;i++)
  {
    printf("%x ",((uint8*)PacktData)[i]);
  }
  printf("\n");
#endif
  
  //��������
  while(Light--)
  {
    switch(status)
    {
    case 0: //data header
      if(PacktData->Header == HEADER)
      {
        status++;  //��һ��״̬
      }
      else{
      }
      break;
    case 1://data light 
      if(PacktData->DataLength <= Light)
      { 
        status++;
      }
      else{ 
      }
      break;
    case 2://data ƫ��
      if(PacktData->Offset == OFFSET)
      {
        status++;
      }
      else{ 
      }
      break;
    case 3://��������
      if(PacktData->DataType == DATATYPECMD)
      {
        //            HalLcdWriteStringValue( "status:", status, 10, HAL_LCD_LINE_2 );
        status++;
      }
      else{ 
        return;
      }
      break;
    case 4://��������
      if(PacktData->NwkType == NWKTYPE)
      {
        //           HalLcdWriteStringValue( "status:", status, 10, HAL_LCD_LINE_2 );
        status++;
      }
      else{ 
        return;
      }
      break;
    case 5://�豸��ַ
      //          if((HI_UINT16(PacktData->ShortAddr) == LO_UINT16(EndShortAddr)) && 
      //             (LO_UINT16(PacktData->ShortAddr) == HI_UINT16(EndShortAddr)) )
      RxDeviceAddr = PacktData->ShortAddr[0];
      RxDeviceAddr |= PacktData->ShortAddr[1] << 8;
      if(RxDeviceAddr == EndShortAddr)
      {
        //            HalLcdWriteStringValue( "status:", status, 10, HAL_LCD_LINE_2 );
        status++;
      }
      else{
        return;
      }
      break;
    case 6://����������
      if(PacktData->DeviceType == g_CoreType)
      {
        //          HalLcdWriteStringValue( "status:", status, 10, HAL_LCD_LINE_2 );
        status++;
      }
      else{ 
      }
      break;
    case 7://����
       if(PacktData->DeviceType == LAMP)
            { 
              rdat = PacktData->Data[0];
              gdat = PacktData->Data[1];
              bdat = PacktData->Data[2];
              P9813_OnOff(rdat,gdat,bdat);
            }
       else if(PacktData->DeviceType == RELAY)
            {           
             SwitchOnOff(PacktData->Data[0]);       
            }
       else if(PacktData->DeviceType == FAN)
            {           
             Switch_FAN(PacktData->Data[0]);       
            } 
       else if(PacktData->DeviceType == BEEP)
            {   
             Switch_BEEP(PacktData->Data[0]); 
            }
            break;
      
    default:
      break;
    }
  }
}      
/***********************************************************************************************************
* ��������: Add_DataMsgBuff(void)
* �������: 
*               
* ���ز���: 
*
* ��    ��: --
*
* ��    ��: 2017/2/13, by SongLei
* ��    ��: 1170152143@qq.com
************************************************************************************************************/
void Add_DataMsgBuff(void)
{
  
  switch(g_CoreType)
  {
  case FAN :                    
     theMessageData[OFFSET]   =  SensorStatusflag;
    break;  
  case  LIGHT :
     theMessageData[OFFSET]   =   AdcData_H;
     theMessageData[OFFSET+1] =   AdcData_L ;
    break; 
  case  TEMPHUM :
     theMessageData[OFFSET]   =   ucharT_data_H ;
     theMessageData[OFFSET+1] =   ucharRH_data_H;
    break; 
  case  MPU9250:
     theMessageData[OFFSET ]    = ACC_X >> 8;        //���ٶ�
     theMessageData[OFFSET + 1] = ACC_X;
     theMessageData[OFFSET + 2] = ACC_Y >> 8;
     theMessageData[OFFSET + 3] = ACC_Y;
     theMessageData[OFFSET + 4] = ACC_Z >> 8;
     theMessageData[OFFSET + 5] = ACC_Z;
     
     theMessageData[OFFSET + 6] = GYRO_X >> 8;  //������
     theMessageData[OFFSET + 7] = GYRO_X;
     theMessageData[OFFSET + 8] = GYRO_Y >> 8;
     theMessageData[OFFSET + 9] = GYRO_Y;
     theMessageData[OFFSET + 10] = GYRO_Z >> 8;
     theMessageData[OFFSET + 11] = GYRO_Z;
                                                  //����
     theMessageData[OFFSET + 12] = MAG_X >> 8;
     theMessageData[OFFSET + 13] = MAG_X;
     theMessageData[OFFSET + 14] = MAG_Y >> 8;
     theMessageData[OFFSET + 15] = MAG_Y;
     theMessageData[OFFSET + 16] = MAG_Z >> 8;
     theMessageData[OFFSET + 17] = MAG_Z;
    break;
    
    case MPU6050 :  
     theMessageData[OFFSET ] =    mpu6050_ACC_X >> 8;        //���ٶ�
     theMessageData[OFFSET + 1] = mpu6050_ACC_X;
     theMessageData[OFFSET + 2] = mpu6050_ACC_Y >> 8;
     theMessageData[OFFSET + 3] = mpu6050_ACC_Y;
     theMessageData[OFFSET + 4] = mpu6050_ACC_Z >> 8;
     theMessageData[OFFSET + 5] = mpu6050_ACC_Z;
    break;
    case BMP180 :
     theMessageData[OFFSET ] = Bmp180Temp;        //�¶�
     theMessageData[OFFSET + 1] = Bmp180AirPressure >> 8;//��ѹ
     theMessageData[OFFSET + 2] = Bmp180AirPressure;
     theMessageData[OFFSET + 3] = Bmp180Altitude >> 8;//�߶�
     theMessageData[OFFSET + 4] = Bmp180Altitude;
      break;
    
  case GAS :
     theMessageData[OFFSET]   =   AdcData_H;
     theMessageData[OFFSET+1] =   AdcData_L ;    
    break; 
  case FLAME :
     theMessageData[OFFSET]   =   AdcData_H;
     theMessageData[OFFSET+1] =   AdcData_L ;
    break; 
  case PHOTOACTOR :
     theMessageData[OFFSET]   =  SensorStatusflag;  
    break; 
  case LAMP :
    theMessageData[OFFSET]   = rdat;
    theMessageData[OFFSET+1] = gdat;
    theMessageData[OFFSET+2] = bdat;  
    break; 
    
  case  RELAY:
     theMessageData[OFFSET]   =  SensorStatusflag;     
    break; 
  case  BEEP:
     theMessageData[OFFSET]   =  SensorStatusflag;
    break; 
  case  POTENTIOMETER:
     theMessageData[OFFSET]   =   AdcData_H;
     theMessageData[OFFSET+1] =   AdcData_L;
    break; 
  case  RAINFALL:
     theMessageData[OFFSET]   =    AdcData_H;
     theMessageData[OFFSET+1] =    AdcData_L ;    
    break; 
    
  case  INFRARED:
     theMessageData[OFFSET]   =  SensorStatusflag;     
    break; 
  case  ULTRASONIC:
     theMessageData[OFFSET]   =    (ultrasonicValue>>8);
     theMessageData[OFFSET+1] =    ultrasonicValue ;       
    break; 
  case  TOUCH:
     theMessageData[OFFSET]   =  SensorStatusflag;   
    break; 
  case  SOUND :         
     theMessageData[OFFSET]   =  SensorStatusflag;
    break; 
  case  FOG:
     theMessageData[OFFSET]   =   AdcData_H;
     theMessageData[OFFSET+1] =   AdcData_L;
    break; 
      
  default:
    break;
  }
}


/***********************************************************************************************************
* ��������: EndSendAnalyzeProtocol(uint8 *packet)
* �������: 
*               
* ���ز���: 
*
* ��    ��: ���Ͳɼ������ݺ���/Э�����������ݵ��ڵ�
*
* ��    ��: 2017/2/10, by SongLei
* ��    ��: 1170152143@qq.com
************************************************************************************************************/
void  EndSendAnalyzeProtocol(void)
{  

//#if  (NWKTYPE == 'Z')  
////  �ڵ������ַ
//  osal_memcpy(&theMessageData[OFFSET -4],(uint8*)&EndShortAddr,2);
//#else
//  memcpy(&theMessageData[OFFSET -4],(uint8*)&EndShortAddr,2);
//#endif
  memcpy(&theMessageData[OFFSET -4],(uint8*)&EndShortAddr,2);
//�豸����
  theMessageData[OFFSET - 2] = g_CoreType;
//������������򲿷�  
//���ADC������ 
   Add_DataMsgBuff();
//�������
  theMessageData[OFFSET -1] = Get_volatile();
  //У��CRC8
  theMessageData[OFFSET + g_SendDataLength] = crc8(theMessageData,OFFSET + g_SendDataLength);
 
  //rcm add
  //printf("send_data:%d\n",theMessageData[OFFSET]<<8 |theMessageData[OFFSET+1] );//old
  printf("st%dh%d\n",theMessageData[OFFSET],theMessageData[OFFSET+1] );//old
   if ( HAL_UART_Transmit(&huart2, theMessageData, OFFSET + g_SendDataLength + 1,0xFFFF) == HAL_OK )
  {
      // Successfully requested to be sent.
      NetWorkLedstatusOnOff(NetWorkState_LED3, NetWorkState_LED3_Flag);//LED����˸��������(����)
      NetWorkState_LED3_Flag = (~NetWorkState_LED3_Flag);
  }
  else
  {
    // Error occurred in request to send.
  }
}

