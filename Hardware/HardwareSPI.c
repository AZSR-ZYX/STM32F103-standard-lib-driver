#include "stm32f10x.h"



static void HardwareSPI_SS(uint8_t x)
{
    GPIO_WriteBit(GPIOA,GPIO_Pin_4,(BitAction)x); /* SS片选函数 */
}

void HardwareSPI_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;    /* SPI的SS引脚配置为推挽输出 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    /* SPI的SCK、MOSI配置为复用推挽输出 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;    /* SPI的MISO配置为上拉输入 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    SPI_InitTypeDef SPI_InitStructure;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;  /* 配置SPI分频系数72MHZ/128 */
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;  /* 此处配置模式0所以在第一个边沿移入 */
    SPI_InitStructure.SPI_CRCPolynomial = 7;      /* CRC校验使用初始化值7 */
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; /* SPI数据大小 */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; /* 这个参数用于裁剪SPI，这里配置为双线全双工 */
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; /* 数据高位先行 */
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master; /* SPI主模式 */
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; /* 软件NSS */
    SPI_Init(SPI1,&SPI_InitStructure);
    
    SPI_Cmd(SPI1,ENABLE); /* SPI1使能 */
    
    HardwareSPI_SS(1); /* 置SS为高电平，即不选中设备 */
}

void HardwareSPI_Start(void)
{
    HardwareSPI_SS(0); /* 拉低选择 */
}

void HardwareSPI_Stop(void)
{
    HardwareSPI_SS(1);  /* 拉高不选 */
}

uint8_t HardwareSPI_SwapByte(uint8_t byte_send)
{
    SPI_I2S_SendData(SPI1,byte_send); /* 把数据写入DR */
    
    while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) != SET); /* 等待数据移入移位寄存器 */
    
    while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) != SET); /* 等待数据接收完成 */
    
    return SPI_I2S_ReceiveData(SPI1); /* 读DR，返回接收到的数据 */
}
