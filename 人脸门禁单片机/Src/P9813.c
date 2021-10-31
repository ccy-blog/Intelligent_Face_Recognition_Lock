#include "stdint.h"
#include "p9813.h"
#include "gpio.h"



uint8_t jdat;
uint8_t rdat;
uint8_t gdat;
uint8_t bdat;


/***********************************************************************************************************
* ��������: void P9813_Init()
* �������: 
*               
* ���ز���: 
*
* ��    ��:
*
* ��    ��: 2017/2/13, by 
* ��    ��: 
************************************************************************************************************/

void P9813_Init()
  { 
     int i;
     for(i=32;i>0;i--)
	{
          HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);
          HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET);
          HAL_Delay_us(1); 
          HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET);
          HAL_Delay_us(1);
	}
  }
/************************************************************************************
* P9813���ͱ�־λ����
*	��־λ����2����1��
*************************************************************************************/
void P9813_flg()
  {
        int i;
	for(i=2;i>0;i--)
	{
          HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);
          HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET);
          HAL_Delay_us(1); 
          HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET);
          HAL_Delay_us(1);
	}
}
/***************************************************************************************
*  P9813���ͼ���λ����
*	 6������λ
*  ��ڲ�����jdat
****************************************************************************************/
void P9813send_jdat(uint8_t dat)
{
  int i;
	for(i=6;i>0;i--)
	{
          HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET);
		if(dat&0x80)
          HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);
		else
          HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);
		dat=dat<<1;
          HAL_Delay(1);
          HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET);
          HAL_Delay(1);
	}
}
/***************************************************************************************
*  P9813������ɫɫ���ݺ���
*	 8������λ
*  ��ڲ�����colourdat
****************************************************************************************/
void P9813colour_dat(uint8_t dat)
{
  int i;
	for(i=8;i>0;i--)
	{
          HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET);
          HAL_Delay_us(1); 
          if(dat&0x80)
          HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);
          else
          HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);
		dat=dat<<1;
          HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET);
          HAL_Delay_us(1); 
	}		
}
/****************************************************************************************
*  P9813��ȡУ��λ����
*	 
*  
****************************************************************************************/
uint8_t P9813_get_jdat(uint8_t Red,uint8_t Green,uint8_t Blue)
{
   
	jdat=~Green&0xC0|(~Blue>>2)&0x30|(~Red>>4)&0x0C;
        return jdat;
}


void P9813_OnOff(uint8_t Red,uint8_t Green,uint8_t Blue)
{
      P9813_Init();
      P9813_flg() ;
      P9813send_jdat(P9813_get_jdat(Red,Green,Blue));           
      P9813colour_dat(Green);      
      P9813colour_dat(Blue);       
      P9813colour_dat(Red);
}

