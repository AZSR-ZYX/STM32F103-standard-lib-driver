#include "stm32f10x.h"

static void SoftSPI_SS(uint8_t x)
{
    GPIO_WriteBit(GPIOA,GPIO_Pin_4,(BitAction)x);
}

static void SoftSPI_SCK(uint8_t x)
{
    GPIO_WriteBit(GPIOA,GPIO_Pin_5,(BitAction)x);
}

static uint8_t SoftSPI_MISO()
{
    return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6);
}

static void SoftSPI_MOSI(uint8_t x)
{
    GPIO_WriteBit(GPIOA,GPIO_Pin_7,(BitAction)x);
}

void SoftSPI_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    SoftSPI_SS(1);
    SoftSPI_SCK(0);
}

void SoftSPI_Start(void)
{
    SoftSPI_SS(0);
}

void SoftSPI_Stop(void)
{
    SoftSPI_SS(1);
}



//uint8_t SoftSPI_SwapByte(uint8_t byte)
//{
//    uint8_t i;
//    for(i = 0;i < 8;i++)
//    {
//    SoftSPI_MOSI(!!(byte & 0x80));
//    SoftSPI_SCK(1);
//    byte <<= 1;
//    byte |= (!!SoftSPI_MISO());
//    SoftSPI_SCK(0);
//    }
//    return byte;
//}

uint8_t SoftSPI_SwapByte(uint8_t byte_send)
{
    uint8_t i,byte_receive;
    byte_receive = 0x00;
    for(i = 0;i < 8;i++)
    {
        SoftSPI_MOSI(!!(byte_send & (0x80 >> i)));
        SoftSPI_SCK(1);
//    byte_receive |= (!!SoftSPI_MISO() & (0x80 >> i)); 一样的不过下面的可读性强
        if(SoftSPI_MISO())
        {
            byte_receive |= (0x80 >> i);
        }
        SoftSPI_SCK(0);
    }
    
    return byte_receive;
}
