/**
  ******************************************************************************
  * File Name          : ultrasonic.c
  * Description        : ���ڳ��������ĳ�ʼ���;���Ĳ�����
  ******************************************************************************
  *

  *
  ******************************************************************************
  */
#include "stdint.h"
#include "tim.h"
#include "gpio.h"
#include "ultrasonic.h"

#include "fs_protocol.h"
    
uint32_t ultrasonicValue=0;
//int overflow = 0;
/* USER CODE BEGIN 0 */
    
void ultrasonicUsing_Init(void)   
{
    MX_TIM2_Init();
    D1_IN_GPIO_Init(); 
    D2_OUT_GPIO_Init();     
}    

void ultrasonicUsing(void)
{
      uint32_t TIM=0;
      
      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8, GPIO_PIN_SET);       
      HAL_Delay_10us(2);   
      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8, GPIO_PIN_RESET);                   
      HAL_Delay_10us(1);   
      
      __HAL_TIM_SET_COUNTER(&htim2,0);

      while(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_7)==0);        //�ȴ�Echo�ز����ű�ߵ�ƽ
      __HAL_TIM_ENABLE(&htim2);  //������ʱ��
      while(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_7)==1);
      __HAL_TIM_DISABLE(&htim2);

        TIM = __HAL_TIM_GET_COUNTER(&htim2);         

        ultrasonicValue = ((TIM * 340)/2) /1000 + 3;   //����1000����λmm��3mm�����
        printf("����:%d\n",ultrasonicValue);        
      return ;
}

/* USER CODE END 0 */





  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
