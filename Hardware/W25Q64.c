#include "stm32f10x.h"
#include "W25Q64_Ins.h"
#include "SoftSPI.h"

void W25Q64_Init(void) /* 初始化软件SPI */
{
    SoftSPI_Init();
}

static void W25Q64_WaitBusy(void) /* 等待不忙 */
{
    uint32_t count;
    count = 100000;
    SoftSPI_Start();
    while(count--)
    {
        SoftSPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);
        if((SoftSPI_SwapByte(0xff) & 0x01) == 0x00)
        {
            break;
        }
    }
    SoftSPI_Stop();
}

/* 写使能 */
void W25Q64_WriteEnable()
{
    SoftSPI_Start();
    SoftSPI_SwapByte(W25Q64_WRITE_ENABLE);
    SoftSPI_Stop();
}

uint16_t W25Q64_ReadID(void) /* 读取芯片厂商和ID */
{
    uint8_t manufacturer,device;
    manufacturer = 0;
    device = 0;
    SoftSPI_Start();
    SoftSPI_SwapByte(W25Q64_MANUFACTURER_DEVICE_ID);
    SoftSPI_SwapByte(W25Q64_DUMMY_BYTE);
    SoftSPI_SwapByte(W25Q64_DUMMY_BYTE);
    SoftSPI_SwapByte(0x00);
    manufacturer = SoftSPI_SwapByte(W25Q64_DUMMY_BYTE);
    device = SoftSPI_SwapByte(W25Q64_DUMMY_BYTE);
    return (manufacturer << 8) | device;
}


/* 读取 */

void W25Q64_ReadData(uint32_t addr,uint8_t *data,uint32_t len)
{
    uint32_t i;
    SoftSPI_Start();
    SoftSPI_SwapByte(W25Q64_READ_DATA);
    SoftSPI_SwapByte(addr >> 16);
    SoftSPI_SwapByte(addr >> 8);
    SoftSPI_SwapByte(addr);
    for(i = 0;i < len;i++)
    {
        data[i] = SoftSPI_SwapByte(0xff);
    }
    SoftSPI_Stop();
}


/* 擦除指定地址对应的扇区 */
void W25Q64_SectorErase(uint32_t addr)
{
    W25Q64_WriteEnable();
    SoftSPI_Start();
    SoftSPI_SwapByte(W25Q64_SECTOR_ERASE_4KB);
    SoftSPI_SwapByte(addr >> 16);
    SoftSPI_SwapByte(addr >> 8);
    SoftSPI_SwapByte(addr);
    SoftSPI_Stop();
    W25Q64_WaitBusy();
}

/* 写入 */

void W25Q64_WritePage(uint32_t addr,uint8_t *data,uint16_t len)
{
    uint8_t i;
    W25Q64_WriteEnable();
    SoftSPI_Start();
    SoftSPI_SwapByte(W25Q64_PAGE_PROGRAM);
    SoftSPI_SwapByte(addr >> 16);
    SoftSPI_SwapByte(addr >> 8);
    SoftSPI_SwapByte(addr);
    for(i = 0;i < len;i++)
    {
        SoftSPI_SwapByte(data[i]);
    }
    SoftSPI_Stop();
    W25Q64_WaitBusy();
}




