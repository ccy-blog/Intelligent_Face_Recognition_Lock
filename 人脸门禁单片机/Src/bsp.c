#include "bsp.h"
#include "adc.h"
#include "i2c.h"
#include "tim.h"

#include "fs_protocol.h"
#include "switch.h"
#include "ultrasonic.h"
#include "mpu9250.h"
#include "mpu6050.h"
#include "bmp180.h"

GPIO_PinState NetWorkState_LED2_Flag = GPIO_PIN_RESET;
GPIO_PinState NetWorkState_LED3_Flag = GPIO_PIN_RESET;
GPIO_PinState NetWorkState_LED4_Flag = GPIO_PIN_RESET;

uint8_t  g_CoreType = 0;   //���а������
uint16_t DEVICE_ADDR = 0x00; //��������ַ
uint16_t EndShortAddr ; 
uint32_t ADC_DMA_Value[ADC_NUM] = {0};    //AD�ɼ���

GPIO_TypeDef* LEDS_GPIOx_PORT[LEDSCOMn]={GPIOB,GPIOB,GPIOB}; 
const uint32_t LEDS_GPIOx_Pin[LEDSCOMn]={GPIO_PIN_2,GPIO_PIN_1,GPIO_PIN_0};


/***********************************************************************************************************
* ��������: void Bsp_Init(void)
* �������: 
*               
* ���ز���: 
*
* ��    ��: �弶��ʼ����AD��GPIO
*
* ��    ��: 2017/2/9, by SongLei
* ��    ��: 1170152143@qq.com
************************************************************************************************************/
void Bsp_Init(void)
{
  //�����İ������
  g_CoreType= Read_CoreType();
  //��ʼ����Ӧ�Ĵ�����
    EndShortAddr = LO_UINT16(DEVICE_ADDR)<<8 | HI_UINT16(DEVICE_ADDR);
  Board_Init(g_CoreType);
}


/***********************************************************************************************************
* ��������: void Board_Init(tpye)
* �������: 
*               
* ���ز���: 
*
* ��    ��: --
*
* ��    ��: 2017/2/10, by SongLei
* ��    ��: 1170152143@qq.com
************************************************************************************************************/
void Board_Init(uint8 tpye)
{
  switch(tpye)
  {
      case FAN :                    
          D1_OUT_GPIO_Init(); 
          HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);
        break;  
      case  LIGHT :        
        break; 
      case  TEMPHUM :
        break; 
      case  MPU9250:
         MX_I2C1_Init();
          Init_MPU9250(); 
        break;
        
      case GAS :   
        break; 
      case FLAME :
          D2_IN_GPIO_Init(); 
        break; 
      case PHOTOACTOR :
          D2_IN_GPIO_Init(); 
        break; 
      case LAMP :
          D1_OUT_GPIO_Init(); 
          D2_OUT_GPIO_Init(); 
        break; 
        
      case  RELAY:
          D1_OUT_GPIO_Init(); 
        break; 
      case  BEEP:
          MX_TIM17_Init();
        break; 
      case  POTENTIOMETER:
        break; 
      case  RAINFALL:        
        break; 
        
      case  INFRARED:
          D2_IN_GPIO_Init(); 
        break; 
      case  ULTRASONIC:
      ultrasonicUsing_Init()  ; 
        break; 
      case  TOUCH:
          D1_IN_GPIO_Init(); 
        break; 
      case  SOUND :         
          D2_IN_GPIO_Init();  
        break; 
      case FOG :   
        break;   
     case  MPU6050 :   
       MX_I2C1_Init();
       InitMPU6050();	//��ʼ��MPU6050
    break; 
  case  BMP180 :  
    MX_I2C1_Init();
      Bmp180_Init();  
    break; 
  default:
    break;
  }
}
/***********************************************************************************************************
* ��������: void Read_CoreType(void)
* �������: 
*               
* ���ز���: 
*
* ��    ��: M0��ȡ���а�D7���ţ����İ�P0_1����ADֵ��ͨ��ADֵ�����жϺ��İ�����
*           WiFi   D7   PA5
* ��    ��: 2017/2/10, by SongLei
* ��    ��: 1170152143@qq.com
************************************************************************************************************/
uint8 Read_CoreType(void)
{
  uint32 adcCoreTypes = 0;
  uint8 DeviceType[2] = {0};
  //��ʼDMAת��ADC
  HAL_ADC_Start_DMA(&hadc,(uint32_t*)ADC_DMA_Value,ADC_NUM);
  
  HAL_Delay(500);
  
  adcCoreTypes = (uint16_t)ADC_DMA_Value[ADC_IN5];    //���ͨ��5��ADCֵ
  
  adcCoreTypes = (adcCoreTypes * 3300)/4096;         //����ADC��ֵ%d mV

#ifdef DEBUG_PRINTF   
  printf("CoreADC:%d mV\n",adcCoreTypes);
#endif
  
  if(ADCCOMP(adcCoreTypes,FAN_ADC-ADC_ERRORS,FAN_ADC+ADC_ERRORS))                //���� ����AD�ɼ�ֵ 16
  {
    DeviceType[0]  =  FAN;
    DEVICE_ADDR =  FAN_ADDR;
    theMessageData[1] = g_SendDataLength = 1;           //�����򳤶�
  }
  else if(ADCCOMP(adcCoreTypes,LIGHT_ADC-ADC_ERRORS_L,LIGHT_ADC+ADC_ERRORS_L))          //��ǿ ����AD�ɼ�ֵ 65
  {
    DeviceType[0] =  LIGHT;
    DEVICE_ADDR =  LIGHT_ADDR ;
    theMessageData[1] = g_SendDataLength = 2;           //�����򳤶�
  }  
  else if(ADCCOMP(adcCoreTypes,TEMPHUM_ADC-ADC_ERRORS,TEMPHUM_ADC+ADC_ERRORS))         //��ʪ�� ����AD�ɼ�ֵ105
  {
    DeviceType[0] =  TEMPHUM;
    DEVICE_ADDR = TEMPHUM_ADDR;
    theMessageData[1] = g_SendDataLength = 2;           //�����򳤶�
  }
  else if(ADCCOMP(adcCoreTypes,MPU9250_ADC-ADC_ERRORS,MPU9250_ADC+ADC_ERRORS))        //���� ����AD�ɼ�ֵ210
  {
    DeviceType[0] =  MPU9250 ;
    DEVICE_ADDR = MPU9250_ADDR ;
    theMessageData[1] = g_SendDataLength = 18;          //�����򳤶�
  }  
  else if(ADCCOMP(adcCoreTypes,GAS_ADC-ADC_ERRORS,GAS_ADC+ADC_ERRORS))        //��ȼ���� ����AD�ɼ�ֵ300
  {
    DeviceType[0] =  GAS;
    DEVICE_ADDR = GAS_ADDR ;
    theMessageData[1] = g_SendDataLength = 2;           //�����򳤶�
  }
  else if(ADCCOMP(adcCoreTypes,FLAME_ADC-ADC_ERRORS,FLAME_ADC+ADC_ERRORS))        //����  ����AD�ɼ�ֵ 430
  {
    DeviceType[0] =  FLAME;
    DEVICE_ADDR = FLAME_ADDR ;
    theMessageData[1] = g_SendDataLength = 2;           //�����򳤶�
  }
  else if(ADCCOMP(adcCoreTypes,PHOTOACTOR_ADC-ADC_ERRORS,PHOTOACTOR_ADC+ADC_ERRORS))       //��翪�� ����AD�ɼ�ֵ 550
  {
    DeviceType[0] =  PHOTOACTOR;
    DEVICE_ADDR = PHOTOACTOR_ADDR;
    theMessageData[1] = g_SendDataLength = 1;           //�����򳤶�
  }
  else if(ADCCOMP(adcCoreTypes,LAMP_ADC-ADC_ERRORS,LAMP_ADC+ADC_ERRORS))        //RGB��ɫ�� ����AD�ɼ�ֵ627
  {
    DeviceType[0] = LAMP;
    DEVICE_ADDR = LAMP_ADDR;
    theMessageData[1] = g_SendDataLength = 3;           //�����򳤶�
  }
  else if(ADCCOMP(adcCoreTypes,RELAY_ADC-ADC_ERRORS,RELAY_ADC+ADC_ERRORS))       //�̵��� ����AD�ɼ�ֵ700
  {
    DeviceType[0] = RELAY ;
    DEVICE_ADDR = RELAY_ADDR;
    theMessageData[1] = g_SendDataLength = 1;           //�����򳤶�
  }
  else if(ADCCOMP(adcCoreTypes,BEEP_ADC-ADC_ERRORS,BEEP_ADC+ADC_ERRORS))      //������ ����AD�ɼ�ֵ818
  {
    DeviceType[0] =  BEEP;
    DEVICE_ADDR = BEEP_ADDR;
    theMessageData[1] = g_SendDataLength = 1;            //�����򳤶�
  }
  else if(ADCCOMP(adcCoreTypes,POTENTIOMETER_ADC-ADC_ERRORS,POTENTIOMETER_ADC+ADC_ERRORS))       //��λ�� ����AD�ɼ�ֵ908
  {
    DeviceType[0] = POTENTIOMETER ;
    DEVICE_ADDR = POTENTIOMETER_ADDR;
    theMessageData[1] = g_SendDataLength = 2;           //�����򳤶�
  }
  else if(ADCCOMP(adcCoreTypes,RAINFALL_ADC-ADC_ERRORS,RAINFALL_ADC+ADC_ERRORS))       //���� ����AD�ɼ�ֵ1055
  {
    DeviceType[0] =  RAINFALL ;
    DEVICE_ADDR = RAINFALL_ADDR ;
    theMessageData[1] = g_SendDataLength = 2;           //�����򳤶�
  }
  else if(ADCCOMP(adcCoreTypes,INFRARED_ADC-ADC_ERRORS,INFRARED_ADC+ADC_ERRORS))      //������� ����AD�ɼ�ֵ1185
  {
    DeviceType[0] =  INFRARED ;
    DEVICE_ADDR = INFRARED_ADDR ;
    theMessageData[1] = g_SendDataLength = 1;           //�����򳤶�
  }
  else if(ADCCOMP(adcCoreTypes,ULTRASONIC_ADC-ADC_ERRORS,ULTRASONIC_ADC+ADC_ERRORS))     //������  ����AD�ɼ�ֵ1336
  {
    DeviceType[0] = ULTRASONIC ;
    DEVICE_ADDR = ULTRASONIC_ADDR ;
    theMessageData[1] = g_SendDataLength = 2;           //�����򳤶�
  }
  else if(ADCCOMP(adcCoreTypes,TOUCH_ADC-ADC_ERRORS,TOUCH_ADC+ADC_ERRORS))        //�������� ����AD�ɼ�ֵ1487
  {
    DeviceType[0] = TOUCH;
    DEVICE_ADDR = TOUCH_ADDR;
    theMessageData[1] = g_SendDataLength = 1;  
  }
  else if(ADCCOMP(adcCoreTypes,SOUND_ADC-ADC_ERRORS,SOUND_ADC+ADC_ERRORS))         //���ؿ��� ����AD�ɼ�ֵ1650
  {
    DeviceType[0] = SOUND;
    DEVICE_ADDR = SOUND_ADDR ;
    theMessageData[1] = g_SendDataLength = 1;     
  }
  else if(ADCCOMP(adcCoreTypes,FOG_ADC-ADC_ERRORS,FOG_ADC+ADC_ERRORS))         //������ ����AD�ɼ�ֵ1650
  {
    DeviceType[0] = FOG ;
    DEVICE_ADDR = FOG_ADDR;
    theMessageData[1] = g_SendDataLength = 2;     
  }  
//  else if(ADCCOMP(adcCoreTypes,0,20))//���� ����AD�ɼ�ֵ1964
//  {
//    DeviceType[0] = HEARTRATE;
//    theMessageData[1] = g_SendDataLength = 1;     //���
//  }
  else if(ADCCOMP(adcCoreTypes,MPU6050_ADC-ADC_ERRORS,MPU6050_ADC+ADC_ERRORS))//���� ����AD�ɼ�ֵ
  {
    DeviceType[0] = MPU6050;
    DEVICE_ADDR = MPU6050_ADDR;
    theMessageData[1] = g_SendDataLength = 6;     //���
  }
  else if(ADCCOMP(adcCoreTypes,BMP180_ADC-ADC_ERRORS,BMP180_ADC+ADC_ERRORS))//��ѹ ����AD�ɼ�ֵ
  {
    DeviceType[0] = BMP180 ;
     DEVICE_ADDR = BMP180_ADDR;
    theMessageData[1] = g_SendDataLength = 5;     //���
  }
    else         //������ ����AD�ɼ�ֵ1650
  {
    DeviceType[0] = Err ;
//    DEVICE_ADDR = Err_ADDR;

  }  
#ifdef    DEBUG_PRINTF 
  printf("DeviceType:%d\n",DeviceType[0]);
#endif
//  HAL_ADC_Stop_DMA(&hadc);
  return DeviceType[0];
}

/***********************************************************************************************************
* ��������: uint8 Get_volatile(void)
* �������: 
*               
* ���ز���: 
*
* ��    ��: ZigBee ��زɼ� P0_0
*           WiFi   ��زɼ� PA0
* ��    ��: 2017/2/10, by SongLei
* ��    ��: 1170152143@qq.com
************************************************************************************************************/
uint8 Get_volatile(void)
{
  uint32 adcBt =0;
  uint8 percent = 0;
  
//  adcBt = HalAdcRead (HAL_ADC_CHANNEL_0, HAL_ADC_RESOLUTION_12);
//  HAL_ADC_Start_DMA(&hadc,(uint32_t*)ADC_DMA_Value,ADC_NUM);//��ʼת��ADC
  adcBt = (uint16_t)ADC_DMA_Value[ADC_IN0];
  
  adcBt = (adcBt * 3300)/4096;

#ifdef DEBUG_PRINTF  
  //rcm add 
  //printf("adcBt:%d\n",adcBt);//old
#endif
  
  /*���ǹ涨��ͷŵ��ѹΪ3.51v ,������ѹAD��⵽��Ӧ����2.34v*/
  /*���ǹ涨��ͷŵ��ѹΪ4.20v ,������ѹAD��⵽��Ӧ����2.8v*/
  //����ٷֱ�   �����ʱ����2.8v  ,�ó���ʱ����2.34v, 460������2800��û��2340�Ĳ�
  percent = ((adcBt - 2340)*100) / 460;
  
  return percent;
}

/***********************************************************************************************************
* ��������: NetWorkLedstatusOnOff(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
* �������: 
*               
* ���ز���: 
*
* ��    ��: ��ʾ����״̬�����ã����ͣ����գ�
*
* ��    ��: 2017/2/26, by SongLei
* ��    ��: 1170152143@qq.com
************************************************************************************************************/
void NetWorkLedstatusOnOff(uint8_t leds, GPIO_PinState PinState)
{
    HAL_GPIO_WritePin(LEDS_GPIOx_PORT[leds], LEDS_GPIOx_Pin[leds], PinState); 
}







