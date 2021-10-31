#ifndef __WIFICONFIG_H
#define __WIFICONFIG_H

#include "stdint.h"


extern uint8_t NowWifiConfig;  //�Ƿ��洦������ģʽ��Wifi���úͽ�������ģʽ

extern uint8_t *SSID;

extern uint8_t StationIP[14] ;

void Wifi_Config(void);
void Reset_RxbuffClear(void);
void Wifi_Read_IP(void);
uint8_t Wifi_RESTORE(void);



#endif
