#include "stm32f10x.h"
#include "W25Q64_Ins.h"
#include "SPI_Driver_Interface.h"
#include "SoftSPI.h"
//#include "HardwareSPI.h"

static const SPI_Driver_Interface_t *s_spi = 0;


void W25Q64_Init(const SPI_Driver_Interface_t *spi_bus) /* 初始化软件SPI */
{
    s_spi = spi_bus;
}

static void W25Q64_WaitBusy(void) /* 等待不忙 */
{
    uint32_t count;
    count = 100000;
    s_spi->start();
    while(count--)
    {
        s_spi->swap(W25Q64_READ_STATUS_REGISTER_1);
        if((s_spi->swap(0xff) & 0x01) == 0x00)
        {
            break;
        }
    }
    s_spi->stop();
}

/* 写使能 */
void W25Q64_WriteEnable()
{
    s_spi->start();
    s_spi->swap(W25Q64_WRITE_ENABLE);
    s_spi->stop();
}

uint16_t W25Q64_ReadID(void) /* 读取芯片厂商和ID */
{
    uint8_t manufacturer,device;
    manufacturer = 0;
    device = 0;
    s_spi->start();
    s_spi->swap(W25Q64_MANUFACTURER_DEVICE_ID);
    s_spi->swap(W25Q64_DUMMY_BYTE);
    s_spi->swap(W25Q64_DUMMY_BYTE);
    s_spi->swap(0x00);
    manufacturer = s_spi->swap(W25Q64_DUMMY_BYTE);
    device = s_spi->swap(W25Q64_DUMMY_BYTE);
    s_spi->stop();
    return (manufacturer << 8) | device;
}


/* 读取 */

void W25Q64_ReadData(uint32_t addr,uint8_t *data,uint32_t len)
{
    uint32_t i;
    s_spi->start();
    s_spi->swap(W25Q64_READ_DATA);
    s_spi->swap(addr >> 16);
    s_spi->swap(addr >> 8);
    s_spi->swap(addr);
    for(i = 0;i < len;i++)
    {
        data[i] = s_spi->swap(0xff);
    }
    s_spi->stop();
}


/* 擦除指定地址对应的扇区 */
void W25Q64_SectorErase(uint32_t addr)
{
    W25Q64_WriteEnable();
    s_spi->start();
    s_spi->swap(W25Q64_SECTOR_ERASE_4KB);
    s_spi->swap(addr >> 16);
    s_spi->swap(addr >> 8);
    s_spi->swap(addr);
    s_spi->stop();
    W25Q64_WaitBusy();
}

/* 写入 */

void W25Q64_WritePage(uint32_t addr,uint8_t *data,uint16_t len)
{
    uint8_t i;
    W25Q64_WriteEnable();
    s_spi->start();
    s_spi->swap(W25Q64_PAGE_PROGRAM);
    s_spi->swap(addr >> 16);
    s_spi->swap(addr >> 8);
    s_spi->swap(addr);
    for(i = 0;i < len;i++)
    {
        s_spi->swap(data[i]);
    }
    s_spi->stop();
    W25Q64_WaitBusy();
}




