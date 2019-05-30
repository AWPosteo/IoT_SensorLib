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



//DHT22 
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
  disp1.display();
  disp1.clearDisplay();

  // Clear the buffer.

  // draw a single pixel
  //display.drawPixel(10, 10, WHITE);
  // Show the display buffer on the hardware.
  // NOTE: You _must_ call display after making any drawing commands
  // to make them visible on the display hardware!
  //display.display();
  //delay(2000);
  //display.clearDisplay();


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

   disp1.setTextSize(2);
   disp1.setTextColor(WHITE);
   disp1.setCursor(15,1);
   disp1.clearDisplay();
   disp1.println(String(t) + "");
   disp1.setCursor(76,8);
   disp1.setTextSize(1);
   disp1.println("Temp");
   
   
   disp1.setCursor(15,17);
   disp1.setTextSize(1);
   
   disp1.println(String(h) + " %");
   disp1.setCursor(15,25);
   disp1.println("Sensor1:DHT22");
     
   disp1.display();
   delay(5000); 

     
   disp1.setTextSize(2);
   disp1.setTextColor(WHITE);
   disp1.setCursor(15,1);
   disp1.clearDisplay();
   disp1.println(String(bme280.getTemperature()) + "");
   disp1.setCursor(76,8);
   disp1.setTextSize(1);
   disp1.println("Temp");
   
   disp1.setCursor(15,17);
   disp1.setTextSize(1);
   disp1.println(String(bme280.getHumidity()*1.00) + " %");
   disp1.setCursor(15,25);
   disp1.println("Sensor2:BME280");
   
   disp1.display();
   delay(5000);
   
   }
   //delay(3000); 
  
}
