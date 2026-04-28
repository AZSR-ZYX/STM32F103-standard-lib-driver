#include "stm32f10x.h"

#include "SPI_Driver_Interface.h"

#define SPI_RCC_GPIO RCC_APB2Periph_GPIOB
#define SPI_GPIO_Port GPIOB
#define SPI_GPIO_SS_Pin GPIO_Pin_12
#define SPI_GPIO_SCK_Pin GPIO_Pin_13
#define SPI_GPIO_MISO_Pin GPIO_Pin_14
#define SPI_GPIO_MOSI_Pin GPIO_Pin_15

static void SoftSPI_SS(uint8_t x)
{
    GPIO_WriteBit(SPI_GPIO_Port,SPI_GPIO_SS_Pin,(BitAction)x);
}

static void SoftSPI_SCK(uint8_t x)
{
    GPIO_WriteBit(SPI_GPIO_Port,SPI_GPIO_SCK_Pin,(BitAction)x);
}

static uint8_t SoftSPI_MISO()
{
    return GPIO_ReadInputDataBit(SPI_GPIO_Port,SPI_GPIO_MISO_Pin);
}

static void SoftSPI_MOSI(uint8_t x)
{
    GPIO_WriteBit(SPI_GPIO_Port,SPI_GPIO_MOSI_Pin,(BitAction)x);
}

static void SoftSPI_Init(void)
{
    RCC_APB2PeriphClockCmd(SPI_RCC_GPIO,ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = SPI_GPIO_SS_Pin | SPI_GPIO_SCK_Pin | SPI_GPIO_MOSI_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI_GPIO_Port,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = SPI_GPIO_MISO_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI_GPIO_Port,&GPIO_InitStructure);
    
    SoftSPI_SS(1);
    SoftSPI_SCK(0);
}

static void SoftSPI_Start(void)
{
    SoftSPI_SS(0);
}

static void SoftSPI_Stop(void)
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

static uint8_t SoftSPI_SwapByte(uint8_t byte_send)
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

const SPI_Driver_Interface_t g_softspi_driver = 
{
    .init = SoftSPI_Init,
    .start = SoftSPI_Start,
    .stop = SoftSPI_Stop,
    .swap = SoftSPI_SwapByte,
};
