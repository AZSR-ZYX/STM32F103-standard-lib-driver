#include "stm32f10x.h"
#include "Delay.h"


#define SoftI2C_RCC RCC_APB2Periph_GPIOB
#define SoftI2C_Port GPIOB
#define SoftI2C_SCL_Pin GPIO_Pin_6
#define SoftI2C_SDA_Pin GPIO_Pin_7



static void SoftI2C_W_SCL(uint8_t BitValue)
{
    GPIO_WriteBit(SoftI2C_Port,SoftI2C_SCL_Pin,(BitAction)BitValue);
    Delay_us(5);
}

static void SoftI2C_W_SDA(uint8_t BitValue)
{
    GPIO_WriteBit(SoftI2C_Port,SoftI2C_SDA_Pin,(BitAction)BitValue);
    Delay_us(5);
}

static uint8_t SoftI2C_R_SDA(void)
{
    uint8_t BitValue;
    BitValue = GPIO_ReadInputDataBit(SoftI2C_Port,SoftI2C_SDA_Pin);
    Delay_us(5);
    
    return BitValue;
}

void SoftI2C_Init(void)
{
    RCC_APB2PeriphClockCmd(SoftI2C_RCC,ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Pin = SoftI2C_SCL_Pin | SoftI2C_SDA_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SoftI2C_Port,&GPIO_InitStructure);
    
    GPIO_SetBits(SoftI2C_Port,SoftI2C_SCL_Pin | SoftI2C_SDA_Pin);
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
