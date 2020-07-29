#include "I2C_BM8563.h"

I2C_BM8563::I2C_BM8563(uint8_t deviceAddress, TwoWire &i2cPort) {
  _deviceAddress = deviceAddress;
  _i2cPort = &i2cPort;
}

void I2C_BM8563::begin(void) {
  _i2cPort->beginTransmission(_deviceAddress);
  _i2cPort->write(0x00);
  _i2cPort->write(0x00);
  _i2cPort->write(0x00);
  _i2cPort->endTransmission();
}

uint8_t I2C_BM8563::bcd2ToByte(uint8_t value) {
  uint8_t tmp = 0;
  tmp = ((uint8_t)(value & (uint8_t)0xF0) >> (uint8_t)0x4) * 10;
  return (tmp + (value & (uint8_t)0x0F));
}

uint8_t I2C_BM8563::byteToBcd2(uint8_t value) {
  uint8_t bcdhigh = 0;

  while (value >= 10) {
    bcdhigh++;
    value -= 10;
  }

  return ((uint8_t)(bcdhigh << 4) | value);
}

void I2C_BM8563::getTime(I2C_BM8563_TimeTypeDef* I2C_BM8563_TimeStruct) {
  uint8_t buf[3] = {0};

  _i2cPort->beginTransmission(_deviceAddress);
  _i2cPort->write(0x02);
  _i2cPort->endTransmission();
  _i2cPort->requestFrom(_deviceAddress, 3);

  while (_i2cPort->available()) {
    buf[0] = _i2cPort->read();
    buf[1] = _i2cPort->read();
    buf[2] = _i2cPort->read();
  }

  I2C_BM8563_TimeStruct->seconds = bcd2ToByte(buf[0] & 0x7f);
  I2C_BM8563_TimeStruct->minutes = bcd2ToByte(buf[1] & 0x7f);
  I2C_BM8563_TimeStruct->hours   = bcd2ToByte(buf[2] & 0x3f);
}

void I2C_BM8563::setTime(I2C_BM8563_TimeTypeDef* I2C_BM8563_TimeStruct) {
  if (I2C_BM8563_TimeStruct == NULL) {
    return;
  }

  _i2cPort->beginTransmission(_deviceAddress);
  _i2cPort->write(0x02);
  _i2cPort->write(byteToBcd2(I2C_BM8563_TimeStruct->seconds));
  _i2cPort->write(byteToBcd2(I2C_BM8563_TimeStruct->minutes));
  _i2cPort->write(byteToBcd2(I2C_BM8563_TimeStruct->hours));
  _i2cPort->endTransmission();
}

void I2C_BM8563::getDate(I2C_BM8563_DateTypeDef* I2C_BM8563_DateStruct) {
  uint8_t buf[4] = {0};

  _i2cPort->beginTransmission(_deviceAddress);
  _i2cPort->write(0x05);
  _i2cPort->endTransmission();
  _i2cPort->requestFrom(_deviceAddress, 4);

  while (_i2cPort->available()) {
    buf[0] = _i2cPort->read();
    buf[1] = _i2cPort->read();
    buf[2] = _i2cPort->read();
    buf[3] = _i2cPort->read();
  }

  I2C_BM8563_DateStruct->date    = bcd2ToByte(buf[0] & 0x3f);
  I2C_BM8563_DateStruct->weekDay = bcd2ToByte(buf[1] & 0x07);
  I2C_BM8563_DateStruct->month   = bcd2ToByte(buf[2] & 0x1f);

  if (buf[2] & 0x80) {
    I2C_BM8563_DateStruct->year = 1900 + bcd2ToByte(buf[3] & 0xff);
  } else {
    I2C_BM8563_DateStruct->year = 2000 + bcd2ToByte(buf[3] & 0xff);
  }
}

void I2C_BM8563::setDate(I2C_BM8563_DateTypeDef* I2C_BM8563_DateStruct) {
  if (I2C_BM8563_DateStruct == NULL) {
    return;
  }

  _i2cPort->beginTransmission(_deviceAddress);
  _i2cPort->write(0x05);
  _i2cPort->write(byteToBcd2(I2C_BM8563_DateStruct->date));
  _i2cPort->write(byteToBcd2(I2C_BM8563_DateStruct->weekDay));

  if (I2C_BM8563_DateStruct->year < 2000) {
    _i2cPort->write(byteToBcd2(I2C_BM8563_DateStruct->month) | 0x80);
    _i2cPort->write(byteToBcd2((uint8_t)(I2C_BM8563_DateStruct->year % 100)));
  } else {
    _i2cPort->write(byteToBcd2(I2C_BM8563_DateStruct->month) | 0x00);
    _i2cPort->write(byteToBcd2((uint8_t)(I2C_BM8563_DateStruct->year % 100)));
  }

  _i2cPort->endTransmission();
}
