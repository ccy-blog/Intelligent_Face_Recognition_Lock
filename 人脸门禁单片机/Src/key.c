#include "key.h"
#include "stm32f0xx_hal.h"
#include "adc.h"
#include "dma.h"
#include "bsp.h"
void KeyInit()
{
  MX_DMA_Init();
  MX_ADC_Init();

  HAL_ADC_Start_DMA(&hadc,(uint32_t*)ADC_DMA_Value,ADC_NUM);//¿ªÊ¼×ª»»ADC       
}
unsigned int GetKey()
{
    unsigned int  ret = AdSensor_ch1();
    if( ret > 200 && ret < 600 )
    {
      return KEY_DOWN;
    }
    else if(ret > 1064 && ret < 1264)
    {
      return KEY_LEFT;
    }
    else if(ret > 1512 && ret <1712)
    {
      return KEY_UP;
    }
    else if(ret > 1885 && ret < 2085)
    {
      return KEY_SEL;
    }
    else
    {
      return KEY_NONE;
    }
}