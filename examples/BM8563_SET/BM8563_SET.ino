#include "I2C_BM8563.h"

I2C_BM8563 rtc(I2C_BM8563_DEFAULT_ADDRESS, Wire1);

void setup() {
  // Init Serial
  Serial.begin(115200);
  delay(50);

  // Init I2C
  Wire1.begin(21, 22);

  // Init RTC
  rtc.begin();

  // Set RTC Date
  I2C_BM8563_DateTypeDef dateStruct;
  dateStruct.weekDay = 3;
  dateStruct.month = 3;
  dateStruct.date = 22;
  dateStruct.year = 2019;
  rtc.setDate(&dateStruct);

  // Set RTC Time
  I2C_BM8563_TimeTypeDef timeStruct;
  timeStruct.hours   = 18;
  timeStruct.minutes = 56;
  timeStruct.seconds = 10;
  rtc.setTime(&timeStruct);
}

void loop() {
  I2C_BM8563_DateTypeDef dateStruct;
  I2C_BM8563_TimeTypeDef timeStruct;

  // Get RTC
  rtc.getDate(&dateStruct);
  rtc.getTime(&timeStruct);

  // Print RTC
  Serial.printf("%04d/%02d/%02d %02d:%02d:%02d\n",
                dateStruct.year,
                dateStruct.month,
                dateStruct.date,
                timeStruct.hours,
                timeStruct.minutes,
                timeStruct.seconds
               );

  // Wait
  delay(1000);
}
