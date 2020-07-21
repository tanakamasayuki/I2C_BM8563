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
  dateStruct.WeekDay = 3;
  dateStruct.Month = 3;
  dateStruct.Date = 22;
  dateStruct.Year = 2019;
  rtc.SetData(&dateStruct);

  // Set RTC Time
  I2C_BM8563_TimeTypeDef timeStruct;
  timeStruct.Hours   = 18;
  timeStruct.Minutes = 56;
  timeStruct.Seconds = 10;
  rtc.SetTime(&timeStruct);
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
