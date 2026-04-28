#ifndef __SPI_Driver_Interface_H
#define __SPI_Driver_Interface_H



typedef enum SPI_Status  /* SPI内函数返回值 */
{
    SPI_OK = 0,
    SPI_ERROR,
}SPI_Status_t;

typedef struct SPI_Driver_Interface  /* SPI虚函数表 */
{
    void (*init)(void);
    void (*start)(void);
    void (*stop)(void);
    uint8_t (*swap)(uint8_t byte_send);
}SPI_Driver_Interface_t;




#endif

