#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Seeed_BME280.h>
#include <Wire.h>
#include <DHT.h>
#include <SPI.h>

//MASTER VERSION 03.06.2019 - WORKSTATION PC

//OLED Display SSD1306
#define OLED_RESET 4
Adafruit_SSD1306 oled01(OLED_RESET);

//Sensor DHT22
#define DHTPIN 9
#define DHTTYPE DHT22 //DHT11, DHT21, DHT22
DHT sensorDHT22(DHTPIN, DHTTYPE);

//BME280 (via SCL/SDA)
BME280 sensorBME280;

long previousMillis = 0;
long interval = 3000;

void oledDisplayText(Adafruit_SSD1306 &oledPanel, String oledText, uint8_t oledTextSize, uint16_t oledColor, int16_t oledTextPosX, int16_t oledTextPosY, uint8_t oledAction);

void setup()
{
  Serial.begin(9600);
  if (!sensorBME280.init()) // Wenn keine Daten vom BME abgefragt werden können...
  {
    Serial.println("FEHLER MIT BME280!"); // ...dann soll eine Fehlermeldung ausgegeben werden.
  }

  sensorDHT22.begin();

  oled01.begin(SSD1306_SWITCHCAPVCC, 0x3C); // initialize with the I2C addr 0x3C (for the 128x32)
  oledDisplayText(oled01, "", 2, 1, 15, 1, 0);
}

// Test Commit from Workstation PC

void loop()
{

  unsigned long currentMillis = millis();
  Serial.println("previousMillis:"+String(previousMillis)+" / currentMillis:" + String(currentMillis));

  if (currentMillis - previousMillis > interval)
  {

    float Pascal;
    double Bar;

    Serial.println("BMW280 (Temp:" + String(sensorBME280.getTemperature()) + "°C / Feuchte:" + String(sensorBME280.getHumidity() * 1.00) + "% )  Druck: ");

    Pascal = sensorBME280.getPressure(); // Abfrage des Drucks in Pascal
    Bar = (Pascal / 100000);             // Wandlung des Pascal-Werts in Bar

    Serial.println(String(Pascal) + " Pascal / " + String(Bar) + " bar)");

    float h = sensorDHT22.readHumidity();    //Luftfeuchte auslesen
    float t = sensorDHT22.readTemperature(); //Temperatur auslesen

    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // Prüfen ob eine gültige Zahl zurückgegeben wird. Wenn NaN (not a number) zurückgegeben wird, dann Fehler ausgeben.
    if (isnan(t) || isnan(h))
    {
      Serial.println("DHT22 konnte nicht ausgelesen werden");
    }
    else
    {
      Serial.println("DHT22  (Temp:" + String(t) + "°C / Feuchte:" + String(h) + "% )");

      oledDisplayText(oled01, String(t) + "", 2, 1, 15, 1, 0);
      oledDisplayText(oled01, "Temp", 1, 1, 76, 8, 1);
      oledDisplayText(oled01, String(h) + " %", 1, 1, 15, 17, 1);
      oledDisplayText(oled01, "Sensor1:DHT22", 1, 1, 15, 25, 2);
    }

    //oledDisplayText(oled01, String(sensorBME280.getTemperature()) + "", 2, 1, 15, 1, 0);
    //oledDisplayText(oled01, "Temp", 1, 1, 76, 8, 1);
    //oledDisplayText(oled01, String(sensorBME280.getHumidity() * 1.00) + " %", 1, 1, 15, 17, 1);
    //oledDisplayText(oled01, "Sensor2:BME280", 1, 1, 15, 25, 2);
    //
    //delay(1500);

    byte error, address;
    int nDevices;

    Serial.println("Scanning...");

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
