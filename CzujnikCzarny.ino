#include "Wire.h"              // For I2C
#include "LCD.h"               // For LCD
#include "LiquidCrystal_I2C.h" // Added library*
#include <BMP180.h>
#include <DHT.h>
#include "DHT.h"
#define DHTPIN 4      // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22 // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);
//Set the pins on the I2C chip used for LCD connections
//ADDR,EN,R/W,RS,D4,D5,D6,D7
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7); // 0x27 is the default I2C bus address of the backpack-see article
BMP180 myBMP(BMP180_ULTRAHIGHRES);

int buzzer = 10;
int smokeA0 = A5;
int gasA4 = A4;

int sensorThres = 100;
int sensorThresGas = 120;

void setup()
{
    //  Serial.begin(115200);
    dht.begin();

    myBMP.begin();
    // Set off LCD module
    lcd.begin(20, 4);                 // 16 x 2 LCD module
    lcd.setBacklightPin(3, POSITIVE); // BL, BL_POL
    lcd.setBacklight(HIGH);

    pinMode(buzzer, OUTPUT);
    pinMode(smokeA0, INPUT);
    pinMode(gasA4, INPUT);
}
void loop()
{
    int mq2Sensor = analogRead(smokeA0);
    int mq5Sensor = analogRead(gasA4);

    float h = dht.readHumidity();
    float t = dht.readTemperature();

    lcd.setCursor(0, 0);
    lcd.print(String("Temp: ") + String(myBMP.getTemperature()) + String(" ") + String(t) + char(223) + String("C"));
    lcd.setCursor(0, 1);
    lcd.print(String("Cisnienie: ") + String(myBMP.getPressure() / 100) + String("HPa"));
    lcd.setCursor(0, 2);
    lcd.print(String("Wilgotnosc: ") + String(h) + String("%"));
    lcd.setCursor(0, 3);
    lcd.print(String("Gaz: ") + String(mq2Sensor) + String(" Dym: ") + String(mq5Sensor) + String("  "));

    if ((mq2Sensor > sensorThres) || (mq5Sensor>sensorThresGas))
    {
        tone(buzzer, 2000, 1000);
    }
    else
    {

        noTone(buzzer);
    }

    delay(2000);
}
