#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Seeed_BME280.h>
//#include <Adafruit_BME280.h>
#include <Wire.h>
#include <DHT.h>
#include <SPI.h>

//OLED DISPLAY SSD1306
#define OLED_RESET 4
Adafruit_SSD1306 disp1(OLED_RESET);

//Taskcsdskk

//DHT22 check
#define DHTPIN 9 
#define DHTTYPE DHT22 //DHT11, DHT21, DHT22

DHT dht(DHTPIN, DHTTYPE);

//BME280 kann über A4 und A5 SCL/SDA genutzt werden, momentan durch OLED belegt
//BME280
BME280 bme280;

void setup ()
{
   Serial.begin(9600); // Einstellen der Baudrate
   if(!bme280.init()) // Wenn keine Daten vom BME abgefragt werden können...
   
       {
          Serial.println("FEHLER MIT BME280!"); // ...dann soll eine Fehlermeldung ausgegeben werden.
        }
    
   dht.begin();

   //display.begin(SSD1306_SWITCHCAPVCC, 0x76);  // initialize with the I2C addr 0x3C (for the 128x32)
   // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  disp1.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done
  
  // Show image buffer on the disp1 hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will disp1 the splashscreen.
  
  //??????????????????? Könte Probleme machen!??!?!?!
  //disp1.display();
  //disp1.clearDisplay();
  oledDisplayText(disp1);
}

void loop() // Starten unserer Schleife

{
  float Pascal;
  double Bar;
  
          Serial.print("BMW280 (Temp:");   
          Serial.print(bme280.getTemperature()); // Abfrage und Ausgabe der Temperatur
          Serial.print("°C / Feuchte:");
          Serial.print(bme280.getHumidity()*1.00);
          Serial.print("% )  Druck: ");
          
  Pascal = bme280.getPressure();  // Abfrage des Drucks in Pascal
  Bar = (Pascal/100000); // Wandlung des Pascal-Werts in Bar
  
          Serial.print(Pascal);
          
          Serial.print(" Pascal / ");
          Serial.print(Bar);
          Serial.println(" bar)");
          
          


  float h = dht.readHumidity(); //Luftfeuchte auslesen
  float t = dht.readTemperature(); //Temperatur auslesen
  
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
   
   
//   disp1.setTextSize(1);
//   disp1.setTextColor(WHITE);
//   disp1.setCursor(1,1);
//   disp1.clearDisplay();
//   disp1.println(String(t) + "C / " + String(bme280.getTemperature()) + "C");
//   disp1.println(String(h) + "% / " + String(bme280.getHumidity()*1.00) + "%");
//   disp1.display();
//   delay(3000);

  oledDisplayText(disp1,String(t) + "",2,1,15,1,1);
  oledDisplayText(disp1,"Temp",1,1,76,8,1);
  oledDisplayText(disp1,String(h) + " %",1,1,15,17,1);
  oledDisplayText(disp1,"Sensor1:DHT22",1,1,15,25,2);


  //disp1.setTextSize(2);
  //disp1.setTextColor(WHITE);
  //disp1.setCursor(15,1);
  //disp1.clearDisplay();
  //disp1.println(String(t) + "");
  //disp1.setCursor(76,8);
  //disp1.setTextSize(1);
  //disp1.println("Temp");
   

   
  //disp1.setCursor(15,17);
  //disp1.setTextSize(1);
  //
  //disp1.println(String(h) + " %");
  //disp1.setCursor(15,25);
  //disp1.println("Sensor1:DHT22");
  //  
  //disp1.display();
  
  
  delay(5000); 

  oledDisplayText(disp1,String(bme280.getTemperature()) + "",2,1,15,1,1);
  oledDisplayText(disp1,"Temp",1,1,76,8,1);
  oledDisplayText(disp1,String(bme280.getHumidity()*1.00) + " %",1,1,15,17,1);
  oledDisplayText(disp1,"Sensor2:BME280",1,1,15,25,1);

   delay(5000);




  //disp1.setTextSize(2);
  //disp1.setTextColor(WHITE);
  //disp1.setCursor(15,1);
  //disp1.clearDisplay();
  //disp1.println(String(bme280.getTemperature()) + "");
  //disp1.setCursor(76,8);
  //disp1.setTextSize(1);
  //disp1.println("Temp");
  //
  //disp1.setCursor(15,17);
  //disp1.setTextSize(1);
  //disp1.println(String(bme280.getHumidity()*1.00) + " %");
  //disp1.setCursor(15,25);
  //disp1.println("Sensor2:BME280");
  //
  //disp1.display();
  //delay(5000);
   
   }
   //delay(3000); 
  
  

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