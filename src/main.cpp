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

void oledDisplayText(String oledText, uint8_t oledTextSize, uint16_t oledColor, int16_t oledTextPosX, int16_t oledTextPosY, uint8_t oledAction);

void setup ()
{
   Serial.begin(9600);
   if(!sensorBME280.init()) // Wenn keine Daten vom BME abgefragt werden können...
   
       {
          Serial.println("FEHLER MIT BME280!"); // ...dann soll eine Fehlermeldung ausgegeben werden.
        }
    
   sensorDHT22.begin();


  oled01.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  oledDisplayText("",2,1,1,1,0);

}

// Test Commit from Workstation PC

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

  
  oledDisplayText(String(t) + "",2,1,15,1,0);
  oledDisplayText("Temp",1,1,76,8,1);  
  oledDisplayText(String(h) + " %",1,1,15,17,1);
  oledDisplayText("Sensor1:DHT22",1,1,15,25,2);  
  
  delay(1500); 

  oledDisplayText(String(sensorBME280.getTemperature()) + "",2,1,15,1,0);
  oledDisplayText("Temp",1,1,76,8,1);
  oledDisplayText(String(sensorBME280.getHumidity()*1.00) + " %",1,1,15,17,1);
  oledDisplayText("Sensor2:BME280",1,1,15,25,2);

  delay(1500); 
   
  }
}


void oledDisplayText(String oledText, uint8_t oledTextSize, uint16_t oledColor, int16_t oledTextPosX, int16_t oledTextPosY, uint8_t oledAction)
{
  // TODO: passby OLED display via function to use the function for several oleds
  // Serial.println("FunctionCall: OledAction: "+ String(oledAction));   
  // oledAction 0=Clear Display, 1=Just Prepare Content, 2=Do the refresh & update
  if (oledAction == 0) {
    oled01.display();
    oled01.clearDisplay();
  }
  oled01.setTextSize(oledTextSize);
  oled01.setTextColor(oledColor);
  oled01.setCursor(oledTextPosX,oledTextPosY);
  oled01.println(oledText);
  if (oledAction == 2) {
    oled01.display();
  }
  
  //if (oledAction == 0) {
  //  oledScreen.display();
  //  oledScreen.clearDisplay();
  //} else {
  //    oledScreen.setTextSize(oledTextSize);
  //    oledScreen.setTextColor(oledColor);
  //    oledScreen.setCursor(oledTextPosX,oledTextPosY);
  //    oledScreen.println(oledText);
  //    if (oledAction == 2) {
  //      oledScreen.display();
  //    }
  //}
}