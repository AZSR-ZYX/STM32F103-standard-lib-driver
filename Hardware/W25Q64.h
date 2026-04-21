#ifndef __W25Q64_H
#define __W25Q64_H

void W25Q64_Init(void);
uint16_t W25Q64_ReadID(void);
uint8_t W25Q64_ReadData(uint32_t addr,uint8_t *data,uint32_t len);
void W25Q64_SectorErase(uint32_t addr);
void W25Q64_WritePage(uint32_t addr,uint8_t *data,uint16_t len);







#endif
