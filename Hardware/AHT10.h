#ifndef __AHT10_H
#define __AHT10_H

void AHT10_Init(void);
void AHT10_StartMeas(void);
uint8_t AHT10_ReadData(float *AHT10_Temp,float *AHT10_RH);




#endif
