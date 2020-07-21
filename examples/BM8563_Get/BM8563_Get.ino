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
}

void loop() {
  I2C_BM8563_DateTypeDef dateStruct;
  I2C_BM8563_TimeTypeDef timeStruct;

  // Get RTC
  rtc.GetData(&dateStruct);
  rtc.GetTime(&timeStruct);

  // Print RTC
  Serial.printf("%04d/%02d/%02d %02d:%02d:%02d\n",
                dateStruct.Year,
                dateStruct.Month,
                dateStruct.Date,
                timeStruct.Hours,
                timeStruct.Minutes,
                timeStruct.Seconds
               );

  // Wait
  delay(1000);
}
