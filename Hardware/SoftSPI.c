#include "stm32f10x.h"
#include "SPI_Driver_Interface.h"  /* 驱动抽象层 */

/* 引脚重命名 */
#define SPI_RCC_GPIO RCC_APB2Periph_GPIOB  
#define SPI_GPIO_Port GPIOB
#define SPI_GPIO_SS_Pin GPIO_Pin_12
#define SPI_GPIO_SCK_Pin GPIO_Pin_13
#define SPI_GPIO_MISO_Pin GPIO_Pin_14
#define SPI_GPIO_MOSI_Pin GPIO_Pin_15

#define SoftSPI_SS_H ((SPI_GPIO_Port)->BSRR = ((uint32_t)SPI_GPIO_SS_Pin))  /* 利用BSRR宏定义引脚的高低电平 */
#define SoftSPI_SCK_H ((SPI_GPIO_Port)->BSRR = ((uint32_t)SPI_GPIO_SCK_Pin))/* uint32强转是为了统一数据类型 */
#define SoftSPI_MOSI_H ((SPI_GPIO_Port)->BSRR = ((uint32_t)SPI_GPIO_MOSI_Pin))/* 防止在if-else中检查报warnning */

#define SoftSPI_SS_L ((SPI_GPIO_Port)->BSRR = ((uint32_t)SPI_GPIO_SS_Pin << 16U)) /* 16U表示这是无符号移位操作 */
#define SoftSPI_SCK_L ((SPI_GPIO_Port)->BSRR = ((uint32_t)SPI_GPIO_SCK_Pin << 16U))
#define SoftSPI_MOSI_L ((SPI_GPIO_Port)->BSRR = ((uint32_t)SPI_GPIO_MOSI_Pin << 16U))

#define SoftSPI_MISO_RAW() ((SPI_GPIO_Port)->IDR & (SPI_GPIO_MISO_Pin))


static void SoftSPI_Init(void)
{
    RCC_APB2PeriphClockCmd(SPI_RCC_GPIO,ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;  /* 对SPI引脚初始化SS、SCK、MOSI推挽输出，MISO上拉输入 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = SPI_GPIO_SS_Pin | SPI_GPIO_SCK_Pin | SPI_GPIO_MOSI_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI_GPIO_Port,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = SPI_GPIO_MISO_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI_GPIO_Port,&GPIO_InitStructure);
    
    SoftSPI_SS_H; /* 拉低SS即不选中设备 */
    SoftSPI_SCK_L; /* 拉低时钟 */
}

static inline void SoftSPI_Start()  /* 选中设备 */
{
    SoftSPI_SS_L;
}

static inline void SoftSPI_Stop()  /* 取消选中设备 */
{
    SoftSPI_SS_H;
}

static uint8_t SoftSPI_SwapByte(uint8_t byte_send)  /* 交换一个字节 */
{
    uint8_t i,byte_receive;
    byte_receive = 0x00;
    for(i = 0;i < 8;i++)
    {

        if ((byte_send & (0x80 >> i))) 
        {
            SoftSPI_MOSI_H;
        }
        else
        {
            SoftSPI_MOSI_L;
        }
        SoftSPI_SCK_H;
//    byte_receive |= (!!SoftSPI_MISO() & (0x80 >> i)); 一样的不过下面的可读性强
        if(SoftSPI_MISO_RAW())
        {
            byte_receive |= (0x80 >> i);
        }
        SoftSPI_SCK_L;
    }
    
    return byte_receive;
}

const SPI_Driver_Interface_t g_softspi_driver = /* 硬件抽象，为将来多实例提供支持 */
{                                              /* 未来添加id功能以支持多设备 */
    .init = SoftSPI_Init,
    .start = SoftSPI_Start,
    .stop = SoftSPI_Stop,
    .swap = SoftSPI_SwapByte,
};
