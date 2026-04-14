#include "stm32f10x.h"
//#include "SoftI2C.h"
#include "HardwareI2CInit.h"
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
    uint8_t aht10_status;
//    SoftI2C_Init();
//    SoftI2C_Start();
//    SoftI2C_SendByte(0x70 | 0x01);
//    SoftI2C_ReceiveACK();
//    if(SoftI2C_ReceiveByte() & 0x08)
//    {
//        SoftI2C_SendACK(1);
//    }
//    else
//    {
//        SoftI2C_SendACK(1);
//        SoftI2C_Stop();
//        SoftI2C_Start();
//        SoftI2C_SendByte(0x70 | 0x00);
//        SoftI2C_ReceiveACK();
//        SoftI2C_SendByte(0xE0);
//        SoftI2C_ReceiveACK();
//    }
//    SoftI2C_Stop();
    I2C_GenerateSTART(I2C1,ENABLE);
    AHT10_WaitEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT);
    I2C_Send7bitAddress(I2C1,0x38,I2C_Direction_Receiver);
    AHT10_WaitEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);
    I2C_AcknowledgeConfig(I2C1,DISABLE);
    I2C_GenerateSTOP(I2C1,ENABLE);
    aht10_status = I2C_ReceiveData(I2C1);
    AHT10_WaitEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED);
    if((aht10_status & 0x08) == 0)
    {
        I2C_GenerateSTART(I2C1,ENABLE);
        AHT10_WaitEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT);
        I2C_Send7bitAddress(I2C1,0x38,I2C_Direction_Transmitter);
        AHT10_WaitEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
        I2C_SendData(I2C1,0xE0);
        AHT10_WaitEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED);
        I2C_GenerateSTOP(I2C1,ENABLE);
    }
    
    Delay_ms(100);
    
}

void AHT10_StartMeas()
{
//    SoftI2C_Start();
//    SoftI2C_SendByte(0x70 | 0x00);
//    SoftI2C_ReceiveACK();
//    SoftI2C_SendByte(0xac);
//    SoftI2C_ReceiveACK();
//    SoftI2C_SendByte(0x33);
//    SoftI2C_ReceiveACK();
//    SoftI2C_SendByte(0x00);
//    SoftI2C_ReceiveACK();
//    SoftI2C_Stop();
    I2C_GenerateSTART(I2C1,ENABLE);
    AHT10_WaitEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT);
    I2C_Send7bitAddress(I2C1,0x38,I2C_Direction_Transmitter);
    AHT10_WaitEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
    I2C_SendData(I2C1,0xac);
    AHT10_WaitEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTING);
    I2C_SendData(I2C1,0x33);
    AHT10_WaitEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTING);
    I2C_SendData(I2C1,0x00);
    AHT10_WaitEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED);
    I2C_GenerateSTOP(I2C1,ENABLE);
    Delay_ms(80);
}

uint8_t AHT10_ReadData(float *AHT10_Temp,float *AHT10_RH)
{
    uint8_t Data[6],i;
//    SoftI2C_Start();
//    SoftI2C_SendByte(0x70 | 0x01);
//    SoftI2C_ReceiveACK();
//    for(i = 0;i < 6;i++)
//    {

//        Data[i] = SoftI2C_ReceiveByte();
//        if(i == 5)
//        {
//            SoftI2C_SendACK(1);
//        }
//        else
//        {
//            SoftI2C_SendACK(0);
//        }
//    }
//    SoftI2C_Stop();
    I2C_GenerateSTART(I2C1,ENABLE);
    AHT10_WaitEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT);
    I2C_Send7bitAddress(I2C1,0x38,I2C_Direction_Receiver);
    AHT10_WaitEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);
    for(i = 0;i < 6;i++)
    {
        if(i == (6 - 1))
        {
            I2C_AcknowledgeConfig(I2C1,DISABLE);
            I2C_GenerateSTOP(I2C1,ENABLE);
        }
        AHT10_WaitEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED);
        Data[i] = I2C_ReceiveData(I2C1);
    }
    I2C_AcknowledgeConfig(I2C1,ENABLE);
    *AHT10_RH = ((uint32_t)Data[1] << 12 )| ((uint32_t)Data[2] << 8) |
                        ((uint32_t)Data[3] >> 8);
    *AHT10_RH = *AHT10_RH / 1048576.0 * 100.0;
    *AHT10_Temp = ((uint32_t)(Data[3] & 0x0F) << 16 )| ((uint32_t)Data[4] << 8) |
                    (uint32_t)Data[5];
    *AHT10_Temp = *AHT10_Temp * 200.0 / 1048576.0 - 50.0;  

    return !!(Data[0] & 0x80);

}
