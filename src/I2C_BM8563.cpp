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

uint8_t I2C_BM8563::Bcd2ToByte(uint8_t Value) {
  uint8_t tmp = 0;
  tmp = ((uint8_t)(Value & (uint8_t)0xF0) >> (uint8_t)0x4) * 10;
  return (tmp + (Value & (uint8_t)0x0F));
}

uint8_t I2C_BM8563::ByteToBcd2(uint8_t Value) {
  uint8_t bcdhigh = 0;

  while (Value >= 10) {
    bcdhigh++;
    Value -= 10;
  }

  return ((uint8_t)(bcdhigh << 4) | Value);
}

void I2C_BM8563::GetTime(I2C_BM8563_TimeTypeDef* I2C_BM8563_TimeStruct) {
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

  I2C_BM8563_TimeStruct->Seconds = Bcd2ToByte(buf[0] & 0x7f);
  I2C_BM8563_TimeStruct->Minutes = Bcd2ToByte(buf[1] & 0x7f);
  I2C_BM8563_TimeStruct->Hours   = Bcd2ToByte(buf[2] & 0x3f);
}

void I2C_BM8563::SetTime(I2C_BM8563_TimeTypeDef* I2C_BM8563_TimeStruct) {
  if (I2C_BM8563_TimeStruct == NULL) {
    return;
  }

  _i2cPort->beginTransmission(_deviceAddress);
  _i2cPort->write(0x02);
  _i2cPort->write(ByteToBcd2(I2C_BM8563_TimeStruct->Seconds));
  _i2cPort->write(ByteToBcd2(I2C_BM8563_TimeStruct->Minutes));
  _i2cPort->write(ByteToBcd2(I2C_BM8563_TimeStruct->Hours));
  _i2cPort->endTransmission();
}

void I2C_BM8563::GetData(I2C_BM8563_DateTypeDef* I2C_BM8563_DateStruct) {
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

  I2C_BM8563_DateStruct->Date    = Bcd2ToByte(buf[0] & 0x3f);
  I2C_BM8563_DateStruct->WeekDay = Bcd2ToByte(buf[1] & 0x07);
  I2C_BM8563_DateStruct->Month   = Bcd2ToByte(buf[2] & 0x1f);

  if (buf[2] & 0x80) {
    I2C_BM8563_DateStruct->Year = 1900 + Bcd2ToByte(buf[3] & 0xff);
  } else {
    I2C_BM8563_DateStruct->Year = 2000 + Bcd2ToByte(buf[3] & 0xff);
  }
}

void I2C_BM8563::SetData(I2C_BM8563_DateTypeDef* I2C_BM8563_DateStruct) {
  if (I2C_BM8563_DateStruct == NULL) {
    return;
  }

  _i2cPort->beginTransmission(_deviceAddress);
  _i2cPort->write(0x05);
  _i2cPort->write(ByteToBcd2(I2C_BM8563_DateStruct->Date));
  _i2cPort->write(ByteToBcd2(I2C_BM8563_DateStruct->WeekDay));

  if (I2C_BM8563_DateStruct->Year < 2000) {
    _i2cPort->write(ByteToBcd2(I2C_BM8563_DateStruct->Month) | 0x80);
    _i2cPort->write(ByteToBcd2((uint8_t)(I2C_BM8563_DateStruct->Year % 100)));
  } else {
    _i2cPort->write(ByteToBcd2(I2C_BM8563_DateStruct->Month) | 0x00);
    _i2cPort->write(ByteToBcd2((uint8_t)(I2C_BM8563_DateStruct->Year % 100)));
  }

  _i2cPort->endTransmission();
}
