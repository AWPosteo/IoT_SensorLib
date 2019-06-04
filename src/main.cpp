#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Wire.h>
#include <SPI.h>

//MASTER VERSION 03.06.2019 - WORKSTATION PC
//03.06. 22:17 SynTest from Work Laptop
//04.06. 07:19 Git Username Workstation & Email
//04.06  07:24 Git Username MacbookPro & Emails

//OLED Display SSD1306
#define OLED_RESET 4
Adafruit_SSD1306 oled01(OLED_RESET);

//BME280 (via SCL/SDA)
Adafruit_BME280 sensorBME280;

#define seaLevelPressure (1013.25)

long previousMillis = 0;
long interval = 5000;

void oledDisplayText(Adafruit_SSD1306 &oledPanel, String oledText, uint8_t oledTextSize, uint16_t oledColor, int16_t oledTextPosX, int16_t oledTextPosY, uint8_t oledAction);

void setup()
{
  Serial.begin(9600);

  bool status = sensorBME280.begin(0x76);
  if (!status) {
    Serial.println("FEHLER MIT BME280!");
    while(1);
  }

  oled01.begin(SSD1306_SWITCHCAPVCC, 0x3C); // initialize with the I2C addr 0x3C (for the 128x32)
  oledDisplayText(oled01, "", 2, 1, 15, 1, 0);
}

// Test Commit from Workstation PC

void loop()
{

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis > interval)
  {

    Serial.println("BMW280");
    Serial.println("Temp:" + String(sensorBME280.readTemperature()) + "°C / Feuchte:" + String(sensorBME280.readHumidity() * 1.00) + "%");
    Serial.println("Druck: " + String(sensorBME280.readPressure() / 100.0F) + " hPa");
    Serial.println("Approx. Altitude:" + String(sensorBME280.readAltitude(seaLevelPressure)));

    // save the last time you blinked the LED
    previousMillis = currentMillis;

    oledDisplayText(oled01, String(sensorBME280.readTemperature()) + "", 2, 1, 15, 1, 0);
    oledDisplayText(oled01, "Temp", 1, 1, 76, 8, 1);
    oledDisplayText(oled01, String(sensorBME280.readHumidity() * 1.00) + " %", 1, 1, 15, 17, 1);
    oledDisplayText(oled01, "Sensor2:BME280", 1, 1, 15, 25, 2);

    byte error, address;
    int nDevices;

    nDevices = 0;
    for (address = 1; address < 127; address++)
    {
      // The i2c_scanner uses the return value of
      // the Write.endTransmisstion to see if
      // a device did acknowledge to the address.
      Wire.beginTransmission(address);
      error = Wire.endTransmission();

      if (error == 0)
      {
        Serial.print("I2C device found at address 0x");
        if (address < 16)
          Serial.print("0");
        Serial.print(address, HEX);
        Serial.println("  !");

        nDevices++;
      }
      else if (error == 4)
      {
        Serial.print("Unknown error at address 0x");
        if (address < 16)
          Serial.print("0");
        Serial.println(address, HEX);
      }
    }
    if (nDevices == 0)
      Serial.println("No I2C devices found\n");
    else
      Serial.println("done\n");
  }
}

void oledDisplayText(Adafruit_SSD1306 &oledPanel, String oledText, uint8_t oledTextSize, uint16_t oledColor, int16_t oledTextPosX, int16_t oledTextPosY, uint8_t oledAction)
{
  // oledAction 0=Clear Display, 1=Prepare Content, 2=Perform refresh & update
  if (oledAction == 0)
  {
    oledPanel.display();
    oledPanel.clearDisplay();
  }
  oledPanel.setTextSize(oledTextSize);
  oledPanel.setTextColor(oledColor);
  oledPanel.setCursor(oledTextPosX, oledTextPosY);
  oledPanel.println(oledText);
  if (oledAction == 2)
  {
    oledPanel.display();
  }
}
