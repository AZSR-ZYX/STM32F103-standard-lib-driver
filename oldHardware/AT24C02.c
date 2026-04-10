#include "stm32f10x.h"
#include "SoftI2C.h"
#include "Delay.h"

void AT24C02_Init()
{
    SoftI2C_Init();
}

void AT24C02_WirteByte(uint8_t addr,uint8_t Byte)
{
    SoftI2C_Start();
    SoftI2C_SendByte(0xA0);
    SoftI2C_ReceiveACK();
    SoftI2C_SendByte(addr);
    SoftI2C_ReceiveACK();
    SoftI2C_SendByte(Byte);
    SoftI2C_ReceiveACK();
    SoftI2C_Stop();
}
uint8_t AT24C02_ReadByte(uint8_t addr)
{
    uint8_t Byte;
    SoftI2C_Start();
    SoftI2C_SendByte(0xA0);
    SoftI2C_ReceiveACK();
    SoftI2C_SendByte(addr);
    SoftI2C_ReceiveACK();
    
    SoftI2C_Start();
    SoftI2C_SendByte(0xA0 | 0x01);
    SoftI2C_ReceiveACK();
    Byte = SoftI2C_ReceiveByte();
    SoftI2C_SendACK(1);
    SoftI2C_Stop();
    return Byte;
}

uint8_t AT24C02_ReadMoreByte(uint8_t *Data,uint8_t addr,uint8_t len)
{
    uint8_t i,result = 0;
    if((addr + len) < 1 || (addr + len) > 256 || Data == 0)
    {
        result = 1;
        return result;
    }

    SoftI2C_Start();
    SoftI2C_SendByte(0xA0);
    SoftI2C_ReceiveACK();
    SoftI2C_SendByte(addr);
    SoftI2C_ReceiveACK();
    
    SoftI2C_Start();
    SoftI2C_SendByte(0xA0 | 0x01);
    SoftI2C_ReceiveACK();
    for(i = 0;i < len;i++)
    {
        Data[addr + i] = SoftI2C_ReceiveByte();
        SoftI2C_SendACK(0);
    }
    SoftI2C_SendACK(1);
    SoftI2C_Stop();
    return result;
}

uint8_t AT24C02_WirteMoreByte(uint8_t *Data,uint8_t addr,uint8_t len)
{
    uint8_t i;
    uint8_t result = 0;
    if((addr + len) < 1 || (addr + len) > 256 || Data == 0)
    {
        result = 1;
        return result;
    }
    SoftI2C_Start();
    SoftI2C_SendByte(0xA0);
    SoftI2C_ReceiveACK();
    SoftI2C_SendByte(addr);
    SoftI2C_ReceiveACK();
    for(i = 0;i < len;i++)
    {
    SoftI2C_SendByte(Data[addr + i]);
    SoftI2C_ReceiveACK();
    }
    SoftI2C_Stop();
    return result;
}
