#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Seeed_BME280.h>
#include <Wire.h>
#include <DHT.h>
#include <SPI.h>

//OLED Display SSD1306
#define OLED_RESET 4
Adafruit_SSD1306 oled01(OLED_RESET);

//Sensor DHT22
#define DHTPIN 9 
#define DHTTYPE DHT22 //DHT11, DHT21, DHT22
DHT sensorDHT22(DHTPIN, DHTTYPE);

//BME280 (via SCL/SDA)
BME280 sensorBME280;

void setup ()
{
   Serial.begin(9600);
   if(!sensorBME280.init()) // Wenn keine Daten vom BME abgefragt werden können...
   
       {
          Serial.println("FEHLER MIT BME280!"); // ...dann soll eine Fehlermeldung ausgegeben werden.
        }
    
   sensorDHT22.begin();


  oled01.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  oledDisplayText(oled01);
}

void loop()
{
  float Pascal;
  double Bar;
  
          Serial.print("BMW280 (Temp:");   
          Serial.print(sensorBME280.getTemperature()); // Abfrage und Ausgabe der Temperatur
          Serial.print("°C / Feuchte:");
          Serial.print(sensorBME280.getHumidity()*1.00);
          Serial.print("% )  Druck: ");
          
  Pascal = sensorBME280.getPressure();  // Abfrage des Drucks in Pascal
  Bar = (Pascal/100000); // Wandlung des Pascal-Werts in Bar
  
          Serial.print(Pascal);
          
          Serial.print(" Pascal / ");
          Serial.print(Bar);
          Serial.println(" bar)");
          
          


  float h = sensorDHT22.readHumidity(); //Luftfeuchte auslesen
  float t = sensorDHT22.readTemperature(); //Temperatur auslesen
  
  // Prüfen ob eine gültige Zahl zurückgegeben wird. Wenn NaN (not a number) zurückgegeben wird, dann Fehler ausgeben.
  if (isnan(t) || isnan(h)) 
  {
  Serial.println("DHT22 konnte nicht ausgelesen werden");
  } 
  else
  {
  Serial.print("DHT22  (Temp:");  
  Serial.print(t);
  Serial.print("°C / Feuchte:");
  Serial.print(h);
  Serial.println("% )");

  oledDisplayText(oled01,String(t) + "",2,1,15,1,1);
  oledDisplayText(oled01,"Temp",1,1,76,8,1);
  oledDisplayText(oled01,String(h) + " %",1,1,15,17,1);
  oledDisplayText(oled01,"Sensor1:DHT22",1,1,15,25,2);
  
  delay(5000); 

  oledDisplayText(oled01,String(sensorBME280.getTemperature()) + "",2,1,15,1,1);
  oledDisplayText(oled01,"Temp",1,1,76,8,1);
  oledDisplayText(oled01,String(sensorBME280.getHumidity()*1.00) + " %",1,1,15,17,1);
  oledDisplayText(oled01,"Sensor2:BME280",1,1,15,25,1);

  delay(5000);
   
  }
}


void oledDisplayText(Adafruit_SSD1306 oledScreen, String oledText="", uint8_t oledTextSize=1, uint16_t oledColor=1, int16_t oledTextPosX=1, int16_t oledTextPosY=1, uint8_t oledAction=0)
{
  // oledAction 0=Clear Display, 1=Just Prepare Content, 2=Do the refresh & update
  if (oledAction = 0) {
    oledScreen.clearDisplay();
  } else {
      oledScreen.setTextSize(oledTextSize);
      oledScreen.setTextColor(oledColor);
      oledScreen.setCursor(oledTextPosX,oledTextPosY);
      oledScreen.println(oledText);
      if (oledAction = 2) {
        oledScreen.display();
      }
    
  }
}