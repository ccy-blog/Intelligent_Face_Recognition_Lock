#ifndef BSP_H
#define BSP_H

#include "stdint.h"
#include "gpio.h"

#define  FALSE  0
#define    TRUE 1

#define BSPINIT TRUE            //�弶��ʼ��
#define DEBUG_PRINTF   TRUE         //ʹ�ܴ�ӡ��Ϣ

/************************��������״̬��ָʾ*************************************/
#define LEDSCOMn                 3   //�Ƶĸ���
//��������
#define     NetWorkState_LED2    0  
//��������
#define     NetWorkState_LED3    1
//��������
#define     NetWorkState_LED4    2

#define   FAN_ADC               2439
#define   LIGHT_ADC             15
#define   TEMPHUM_ADC           53             
#define   MPU9250_ADC           159

#define   GAS_ADC               200
#define   FLAME_ADC             251
#define   PHOTOACTOR_ADC        501
#define   LAMP_ADC              572

#define   RELAY_ADC             650
#define   BEEP_ADC              768
#define   POTENTIOMETER_ADC     845
#define   RAINFALL_ADC          1008

#define   INFRARED_ADC          1135
#define   ULTRASONIC_ADC        1290
#define   TOUCH_ADC             1444
#define   SOUND_ADC             1650
#define   FOG_ADC               1770   
#define   MPU6050_ADC           2079
#define   BMP180_ADC            2197
#define   HALL_ADC              2400  //��������

#define   ADC_ERRORS            25
#define   ADC_ERRORS_L          10

#define   FAN_ADDR                0x402c
#define   LIGHT_ADDR              0x002a
#define   TEMPHUM_ADDR            0x002b 
#define   MPU9250_ADDR            0x002c

#define   GAS_ADDR                0x002d
#define   FLAME_ADDR              0x002e
#define   PHOTOACTOR_ADDR         0x002f
#define   LAMP_ADDR               0x402d

#define   RELAY_ADDR              0x402e
#define   BEEP_ADDR               0x402f
#define   POTENTIOMETER_ADDR      0x0030
#define   RAINFALL_ADDR           0x0031

#define   INFRARED_ADDR           0x0032
#define   ULTRASONIC_ADDR         0x0033
#define   TOUCH_ADDR              0x0034
#define   SOUND_ADDR              0x0035
#define   FOG_ADDR                0x0036 
#define   MPU6050_ADDR                0x0038 
#define   BMP180_ADDR                0x0039 
#define   HALL_ADDR                0x0040 

#define HI_UINT16(a) (((a) >> 8) & 0xFF)
#define LO_UINT16(a) ((a) & 0xFF)


//extern uint8_t NetWorkState_LED2_Flag;
//extern uint8_t NetWorkState_LED3_Flag;
//extern uint8_t NetWorkState_LED4_Flag;

extern GPIO_PinState NetWorkState_LED2_Flag ;
extern GPIO_PinState NetWorkState_LED3_Flag ;
extern GPIO_PinState NetWorkState_LED4_Flag ;





typedef enum ADC_INx {
    ADC_IN0,                    //��ص���
    ADC_IN1,
//    ADC_IN2,
//    ADC_IN3,
    ADC_IN4,                   //������ADֵ
    ADC_IN5,                    //������������
    ADC_NUM = 3                 //ADCת���ĸ���
}ADC_INx;          

#define CORETYPE(x)      x      //���а������

#define ADCCOMP(c,x,y)   (((x<=c) && (c<=y)) ? 1:0)

extern uint8_t g_CoreType;   //���а������
extern uint16_t EndShortAddr ; 
extern uint32_t ADC_DMA_Value[ADC_NUM];

extern   uint8_t * P_H  ;
extern   uint8_t * P_L ;

//��õ���
uint8_t Get_volatile(void);
void Bsp_Init(void);
uint8_t Read_CoreType(void);
void Board_Init(uint8_t tpye);
void DisplayDeviceLogo(void);
void NetWorkLedstatusOnOff(uint8_t leds, GPIO_PinState PinState);
#endif


