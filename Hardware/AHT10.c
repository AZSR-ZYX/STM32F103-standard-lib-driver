#include "stm32f10x.h"
#include "SoftI2C.h"
#include "Delay.h"

void AHT10_Init(void)
{
    SoftI2C_Init();
    
    SoftI2C_Start();
    SoftI2C_SendByte(0x70 | 0x00);
    SoftI2C_ReceiveACK();
    SoftI2C_SendByte(0xE0);
    SoftI2C_ReceiveACK();
    SoftI2C_Stop();
    Delay_ms(20);
}

void AHT10_StartMeas()
{
    SoftI2C_Start();
    SoftI2C_SendByte(0x70 | 0x00);
    SoftI2C_ReceiveACK();
    SoftI2C_SendByte(0xac);
    SoftI2C_ReceiveACK();
    SoftI2C_SendByte(0x33);
    SoftI2C_ReceiveACK();
    SoftI2C_SendByte(0x00);
    SoftI2C_ReceiveACK();
    SoftI2C_Stop();
    Delay_ms(80);
}

uint8_t AHT10_ReadData(float *AHT10_Temp,float *AHT10_RH)
{
    uint8_t Data[5],i;
    SoftI2C_Start();
    SoftI2C_SendByte(0x70 | 0x01);
    SoftI2C_ReceiveACK();
    if((SoftI2C_ReceiveByte() | 0x80) == 1)
    {
        return 1;
    }
    for(i = 0;i < 5;i++)
    {
    SoftI2C_SendACK(0);
    Data[i] = SoftI2C_ReceiveByte();
    }
    SoftI2C_SendACK(1);
    SoftI2C_Stop();
    *AHT10_RH = ((((uint32_t)Data[0] << 12 )| ((uint32_t)Data[1] << 8) |
                        ((uint32_t)Data[2] >> 8)) / 1048576.0) * 100.0;
    *AHT10_Temp = ((((uint32_t)(Data[2] & 0x0F) << 16 )| ((uint32_t)Data[3] << 8) |
                    (uint32_t)Data[4] )/ 1048576.0) * 200.0 - 50.0;
    return Data[4];
}