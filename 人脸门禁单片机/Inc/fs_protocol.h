#ifndef FS_PROTOCOL_H
#define FS_PROTOCOL_H

#include "stdint.h"

typedef __INT8_T_TYPE__   int8;
typedef __UINT8_T_TYPE__ uint8;

typedef __INT16_T_TYPE__   int16;
typedef __UINT16_T_TYPE__ uint16;

typedef __INT32_T_TYPE__   int32;
typedef __UINT32_T_TYPE__ uint32;

/******************************************************************************
FSЭ�����,һ�㲻��Ҫ�޸�
*******************************************************************************/
/***************************************************************************************
Э���ʽ��
|  ��ʶ   | ���ݳ��� |  ����ƫ�� | �������� | �������� |  �豸��ַ |   �豸����  |  ����   |������   |   У���   |
|  0x21   |   1byte |   1byte   |  1byte   |  1byte  |   2byte   |    1byte   |  1byte  |256byte  |   1byte    |
******************************************************************************************/

//���巢�͵�����
#define HEADER                 0x21

//���巢���������������󳤶�                  
#define SENDDATALENGTH         18     //�����򳤶�


#define OFFSET                 9     //����ƫ�Ƴ���
#define DATATYPEDATA           0x00  //��������   0x00������ 0x01������ 
#define DATATYPECMD            0x01  //��������   0x00������ 0x01������
#define NWKTYPE                'W'   //��������

#define   FAN               'f'   //�豸���� ���ȿ�
#define   LIGHT             'L'   //�豸���� ��ǿ
#define   TEMPHUM           'T'   //�豸���� ��ʪ��
#define   MPU9250           'M'   //�豸���� ����

#define   GAS               'G'   //�豸���� ��ȼ����
#define   FLAME             0x26  //�豸���� ����  "F" - 0x20
#define   PHOTOACTOR        'P'   //�豸���� ���
#define   LAMP              'l'   //�豸���� �ƿ�

#define   RELAY             'r'   //�豸���� �̵�����
#define   BEEP              'b'   //�豸���� ��������
#define   POTENTIOMETER     0x30  //�豸���� ��λ��  'P'- 0x20
#define   RAINFALL          'R' //�豸���� ��� 'R' 

#define   INFRARED          'I'   //�豸���� ������� 
#define   ULTRASONIC        'U'   //�豸���� ������
#define   TOUCH             0x34 //�豸���� ���� 'T' - 0x20
#define   SOUND             'S'  //�豸���� ���ؿ���
#define   FOG               'F'  //�豸���� ����

#define   MPU6050           0x2D  //�豸���� ���� 'M'- 0x20 
#define   BMP180            'B'   //�豸���� ��ѹ
#define   HALL              'H'           //�豸���� ����  ��H��

#define   Err               'E'  //δʶ�𴫸���

#define    CMDON        '1'
#define    CMDOFF       '0'

  
//#define RXLENGHT   512   //USART2 ���ڻ�����
  
extern uint8_t g_SendDataLength; 
extern uint8_t theMessageData[OFFSET + SENDDATALENGTH +1];

extern uint8_t SensorStatusflag;  

//����ֵ
//extern uint8_t AdcData_H ;
//extern uint8_t AdcData_L ;

//�������ݽṹ��
typedef struct
{
  uint8_t   Header;               //��ͷ
  uint8_t   DataLength;           //Number of bytes in TransData
  uint8_t   Offset;               //ƫ����
  uint8_t   DataType;             //��������
  uint8_t   NwkType;              //��������
  uint8_t   ShortAddr[2];            //�����ַ
  uint8_t   DeviceType;           //�豸����
  uint8_t   Bat;                  //����
  uint8_t   Data[0];              //����
  //  uint8_t   CRC8;                 //У��
} SendData_t;



#define ZIGBEE_SENSOR_MAXIMAL_NUM 30   //���Ա���30���ڵ��ַ

typedef struct {
  uint16_t zigbee_sensor_id;              //��fsЭ����أ�ռ�������ֽ�.
  uint16_t zigbee_sensor_shortaddr;       //��zigbeeЭ����أ���ָzigbee_device��PAN�е�short_addr.
} zigbee_sensor_addr_t;

typedef struct {
  zigbee_sensor_addr_t zigbee_sensor_addr_table[ZIGBEE_SENSOR_MAXIMAL_NUM];     //�洢�������ĵ�ַ��Ϣ��sensor_id + endpoint_shortaddr��
  uint8_t zigbee_sensor_nums;                             //�Ѿ����ӵĴ������ĸ���
} zigbee_sensors_desc_t;

//extern byte GenericApp_TransID;  // This is the unique message ID (counter)
//extern afAddrType_t GenericApp_DstAddr;
//extern endPointDesc_t GenericApp_epDesc;  
////extern devStates_t GenericApp_NwkState;

extern void delay_ms(uint16_t ms);
extern uint8_t crc8(uint8_t *data, uint8_t length);
extern void EndGettAnalyzeProtocol(SendData_t *PacktData,uint16_t Light);
extern void EndSendAnalyzeProtocol(void);
extern void num_to_str(unsigned int num, unsigned char *pstr, unsigned char num_len,uint8 type);
extern void Add_DataMsgBuff(void);

#endif



