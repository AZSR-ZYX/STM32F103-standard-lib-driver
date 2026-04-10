#ifndef __AT24C02_H
#define __AT24C02_H

void AT24C02_Init(void);
void AT24C02_WirteByte(uint8_t addr,uint8_t Byte);
uint8_t AT24C02_ReadByte(uint8_t addr);
uint8_t AT24C02_WirteMoreByte(uint8_t *Data,uint8_t addr,uint8_t len);
uint8_t AT24C02_ReadMoreByte(uint8_t *Data,uint8_t addr,uint8_t len);










#endif
