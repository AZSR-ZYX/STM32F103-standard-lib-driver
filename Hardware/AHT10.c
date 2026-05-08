#include "stm32f10x.h"
#include "SoftI2C.h"
#include "Delay.h"

void AHT10_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
    uint16_t Timeout;
    Timeout = 10000;
    while(I2C_CheckEvent(I2Cx,I2C_EVENT) != SUCCESS)
    {
        Timeout--;
        if(Timeout == 0)
        {
            
            break;
        }
    }
}

void AHT10_Init(void)
{
    SoftI2C_Init();
    SoftI2C_Start();
    SoftI2C_SendByte(0x70 | 0x01);
    SoftI2C_ReceiveACK();
    if(SoftI2C_ReceiveByte() & 0x08)
    {
        SoftI2C_SendACK(1);
    }
    else
    {
        SoftI2C_SendACK(1);
        SoftI2C_Stop();
        SoftI2C_Start();
        SoftI2C_SendByte(0x70 | 0x00);
        SoftI2C_ReceiveACK();
        SoftI2C_SendByte(0xE1);
        SoftI2C_ReceiveACK();
    }
    SoftI2C_Stop();
    Delay_ms(100);
    
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
//    Delay_ms(80);
}

uint8_t AHT10_ReadData(float *AHT10_Temp,float *AHT10_RH)
{
    uint8_t Data[6],i;
    SoftI2C_Start();
    SoftI2C_SendByte(0x70 | 0x01);
    SoftI2C_ReceiveACK();
    for(i = 0;i < 6;i++)
    {

        Data[i] = SoftI2C_ReceiveByte();
        if(i == 5)
        {
            SoftI2C_SendACK(1);
        }
        else
        {
            SoftI2C_SendACK(0);
        }
    }
    SoftI2C_Stop();
    I2C_AcknowledgeConfig(I2C1,ENABLE);
    *AHT10_RH = ((uint32_t)Data[1] << 12 )| ((uint32_t)Data[2] << 8) |
                        ((uint32_t)Data[3] >> 4);
    *AHT10_RH = *AHT10_RH / 1048576.0 * 100.0;
    *AHT10_Temp = ((uint32_t)(Data[3] & 0x0F) << 16 )| ((uint32_t)Data[4] << 8) |
                    (uint32_t)Data[5];
    *AHT10_Temp = *AHT10_Temp * 200.0 / 1048576.0 - 50.0;  

    return !!(Data[0] & 0x80);

}

