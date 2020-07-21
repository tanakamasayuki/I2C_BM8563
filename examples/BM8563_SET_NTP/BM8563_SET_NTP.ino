#include "I2C_BM8563.h"
#include <WiFi.h>

I2C_BM8563 rtc(I2C_BM8563_DEFAULT_ADDRESS, Wire1);

const char* ntpServer = "time.cloudflare.com";

void setup() {
  // Init Serial
  Serial.begin(115200);
  delay(50);

  // Connect to an access point
  WiFi.begin();                 // Connect to the access point of the last connection
  //WiFi.begin("SSID", "KEY");  // Or, Connect to the specified access point

  Serial.print("Connecting to Wi-Fi ");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" CONNECTED");

  // Set ntp time to local
  configTime(9 * 3600, 0, ntpServer);

  // Init I2C
  Wire1.begin(21, 22);

  // Init RTC
  rtc.begin();

  // Get local time
  struct tm timeInfo;
  if (getLocalTime(&timeInfo)) {
    // Set RTC time
    I2C_BM8563_TimeTypeDef timeStruct;
    timeStruct.Hours   = timeInfo.tm_hour;
    timeStruct.Minutes = timeInfo.tm_min;
    timeStruct.Seconds = timeInfo.tm_sec;
    rtc.SetTime(&timeStruct);

    // Set RTC Date
    I2C_BM8563_DateTypeDef dateStruct;
    dateStruct.WeekDay = timeInfo.tm_wday;
    dateStruct.Month   = timeInfo.tm_mon + 1;
    dateStruct.Date    = timeInfo.tm_mday;
    dateStruct.Year    = timeInfo.tm_year + 1900;
    rtc.SetData(&dateStruct);
  }
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
