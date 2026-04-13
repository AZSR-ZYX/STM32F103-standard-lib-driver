#ifndef __SoftI2C_H
#define __SoftI2C_H

void SoftI2C_Init(void);
void SoftI2C_Start(void);
void SoftI2C_Stop(void);
void SoftI2C_SendByte(uint8_t Byte);
uint8_t SoftI2C_ReceiveByte(void);
void SoftI2C_SendACK(uint8_t AckBit);
uint8_t SoftI2C_ReceiveACK(void);







#endif
