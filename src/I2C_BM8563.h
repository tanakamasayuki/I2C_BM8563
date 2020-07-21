#ifndef __I2C_BM8563_H__
#define __I2C_BM8563_H__

#include <Wire.h>

#define I2C_BM8563_DEFAULT_ADDRESS 0x51

typedef struct
{
  uint8_t Hours;
  uint8_t Minutes;
  uint8_t Seconds;
} I2C_BM8563_TimeTypeDef;

typedef struct
{
  uint8_t WeekDay;
  uint8_t Month;
  uint8_t Date;
  uint16_t Year;
} I2C_BM8563_DateTypeDef;

class I2C_BM8563 {
  public:
    I2C_BM8563(uint8_t deviceAddress = I2C_BM8563_DEFAULT_ADDRESS, TwoWire &i2cPort = Wire);

    void begin(void);

    void SetTime(I2C_BM8563_TimeTypeDef* I2C_BM8563_TimeStruct);
    void SetData(I2C_BM8563_DateTypeDef* I2C_BM8563_DateStruct);

    void GetTime(I2C_BM8563_TimeTypeDef* I2C_BM8563_TimeStruct);
    void GetData(I2C_BM8563_DateTypeDef* I2C_BM8563_DateStruct);

  private:
    uint8_t Bcd2ToByte(uint8_t Value);
    uint8_t ByteToBcd2(uint8_t Value);
    uint8_t trdata[7];
    TwoWire *_i2cPort;
    int _deviceAddress;
};

#endif
