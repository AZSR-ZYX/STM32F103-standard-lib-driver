#include "stm32f10x.h"
#include "Delay.h"



void SoftI2C_W_SCL(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOB,GPIO_Pin_6,(BitAction)BitValue);
    Delay_ms(10);
}

void SoftI2C_W_SDA(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOB,GPIO_Pin_7,(BitAction)BitValue);
    Delay_ms(10);
}

uint8_t SoftI2C_R_SDA(void)
{
    uint8_t BitValue;
    BitValue = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7);
    Delay_ms(10);
    
    return BitValue;
}

void SoftI2C_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    
    GPIO_SetBits(GPIOB,GPIO_Pin_6 | GPIO_Pin_7);
}

void SoftI2C_Start(void)
{
    SoftI2C_W_SDA(1);
    SoftI2C_W_SCL(1);
    SoftI2C_W_SDA(0);
    SoftI2C_W_SCL(0);
}

void SoftI2C_Stop(void)
{
    SoftI2C_W_SDA(0);
    SoftI2C_W_SCL(1);
    SoftI2C_W_SDA(1);
}

void SoftI2C_SendByte(uint8_t Byte)
{
    uint8_t i;
    for(i = 0;i < 8;i++)
    {
        SoftI2C_W_SDA(!!(Byte & (0x80 >> i))); /* !!把非零值取反保证值为1 or 0 */
        SoftI2C_W_SCL(1);
        SoftI2C_W_SCL(0);
    }
}

uint8_t SoftI2C_ReceiveByte(void)
{
    uint8_t Byte = 0x00;
    uint8_t i;
    SoftI2C_W_SDA(1);
    for(i = 0;i < 8;i++)
    {
        SoftI2C_W_SCL(1);
        if(SoftI2C_R_SDA() == 1)
        {
            Byte |= (0x80 >> i);
        }
        SoftI2C_W_SCL(0);
    }
    return Byte;
}

void SoftI2C_SendACK(uint8_t AckBit)
{
    SoftI2C_W_SDA((BitAction)AckBit);
    SoftI2C_W_SCL(1);
    SoftI2C_W_SCL(0);
}

uint8_t SoftI2C_ReceiveACK(void)
{
    uint8_t AckBit=0;
    SoftI2C_W_SDA(1);
    SoftI2C_W_SCL(1);
    AckBit = SoftI2C_R_SDA();
    SoftI2C_W_SCL(0);
    return AckBit;
}
