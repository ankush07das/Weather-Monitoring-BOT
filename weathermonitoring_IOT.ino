#define BLYNK_TEMPLATE_ID "TMPL3P0HwoWRq"
#define BLYNK_TEMPLATE_NAME "Air Quality monitoring"
#define BLYNK_AUTH_TOKEN "WGeEmDZWLdRVrrMmdeqCi3C0UD2r5Zck"


#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Adafruit_BMP085.h>
#include <DHT.h>
#include <Wire.h>
//#include <LiquidCrystal_I2C.h>
#include "MQ135.h"
Adafruit_BMP085 bmp;
#define SEA_LEVEL_PRESSURE 101325.0


String apiKey = "VU37MJ5NGXEFN5EQ";  // Enter your Write API key from ThingSpeak
const char *ssid = "Realme Narzo 30";   // replace with your WiFi SSID
const char *pass = "harharmahadev"; // replace with your WiFi password
//const char *server = "api.thingspeak.com";
const int sensorPin = 0;
int air_quality;
int flag=0;
float  pressure, Altitude, Sealevel;

//LiquidCrystal_I2C lcd(0x27, 16, 2);

//byte degree_symbol[8] =
    //{
        //0b00111,
        //0b00101,
        //0b00111,
        //0b00000,
        //0b00000,
        //0b00000,
        //0b00000,
        //0b00000};

char auth[] = BLYNK_AUTH_TOKEN;

BlynkTimer timer;

int gas = 32;
int sensorThreshold = 100;

#define DHTPIN 2     // Connect Out pin to D4 in NODE MCU
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
#define RAIN_SENSOR  35

WiFiClient client;

//void sendToThingSpeak(float temperature, float humidity, int airQuality)
//{
  //if (client.connect(server, 80)) // "184.106.153.149" or api.thingspeak.com
  //{
   // String postStr = apiKey;
    //postStr += "&field1=";
    //postStr += String(temperature);
    //postStr += "&field2=";
    //postStr += String(humidity);
    //postStr += "&field3=";
    //postStr += String(airQuality);
    //postStr += "\r\n\r\n";

    //client.print("POST /update HTTP/1.1\n");
    //client.print("Host: api.thingspeak.com\n");
    //client.print("Connection: close\n");
    //client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    //client.print("Content-Type: application/x-www-form-urlencoded\n");
    //client.print("Content-Length: ");
    //client.print(postStr.length());
    //client.print("\n\n");
    //client.print(postStr);

    //Serial.print(" Celcius in Temperature: ");
    //Serial.print(temperature);
    //Serial.print(" Percentage of  Humidity: ");
    //Serial.print(humidity);
    //Serial.print("%, Air Quality: ");
    //Serial.print(airQuality);
    //Serial.println(" PPM. Sent to ThingSpeak.");
  //}
  //client.stop();
//}

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int analogSensor = analogRead(gas);
  int isButtonPressed = analogRead(RAIN_SENSOR);
  isButtonPressed= map(isButtonPressed, 0, 1023, 0, 100);
  float pressure = bmp.readPressure();
  float Sealevel = bmp.readSealevelPressure();
  float Altitude = bmp.readAltitude();

  Blynk.virtualWrite(V2, analogSensor);
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);
  //Blynk.virtualWrite(V3, isButtonPressed);
  //Blynk.virtualWrite(V6, temp);
  Blynk.virtualWrite(V4, pressure);
  //Blynk.virtualWrite(V8, Sealevel);
  //Blynk.virtualWrite(V9, Altitude);

  Serial.print("Gas Value: ");
  Serial.println(analogSensor);
  Serial.print("Temperature : ");
  Serial.println(t);
  Serial.print("    Humidity : ");
  Serial.println(h);
  Serial.print("Rain occuring: ");
  Serial.println(isButtonPressed);
  Serial.print("Pressure is :");
  Serial.println(pressure);
  Serial.print("Altitude  is :");
  Serial.println(Altitude);
  Serial.print("Sea Level is :");
  Serial.println(Sealevel);




  // Send data to ThingSpeak
  //sendToThingSpeak(t, h, analogSensor,isButtonPressed);
}

void setup()
{
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  dht.begin();
  timer.setInterval(200L, sendSensor);
  if (!bmp.begin())
  {
  Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  while (1)
  {
    
  }
  }
 

  //lcd.begin();
  //lcd.createChar(1, degree_symbol);

  //lcd.setCursor(3, 0);
  //lcd.print("Air Quality");
  //lcd.setCursor(3, 1);
  //lcd.print("Monitoring");
  //delay(1000);
  //lcd.clear();

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop()
{
  Blynk.run();
  timer.run();

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int gasValue = analogRead(gas);
  int isButtonPressed = analogRead(RAIN_SENSOR);
  float pressure = bmp.readPressure();
  float Sealevel = bmp.readSealevelPressure();
  float Altitude = bmp.readAltitude();

  //lcd.setCursor(0, 0);
  //lcd.print("Temperature ");
  //lcd.setCursor(0, 1);
  //lcd.print(t);
  //lcd.setCursor(6, 1);
  //lcd.write(1);
  //lcd.setCursor(7, 1);
  //lcd.print("C");
  //delay(1000);
  //lcd.clear();

  //lcd.setCursor(0, 0);
  //lcd.print("Humidity ");
  //lcd.print(h);
  //lcd.print("%");
  //delay(1000);
  //lcd.clear();

  if (gasValue < 600)
  {
    //lcd.setCursor(0, 0);
    //lcd.print("Gas Value: ");
    //lcd.print(gasValue);
    //lcd.setCursor(0, 1);
    //lcd.print("Fresh Air");
    Serial.println("Fresh Air");
    delay(1000);
    //lcd.clear();
  }
  else if (gasValue > 600)
  {
    //lcd.setCursor(0, 0);
    //lcd.print("Gas Value: ");
    //lcd.print(gasValue);
    //lcd.setCursor(0, 1);
    //lcd.print("Bad Air");
    Serial.println("Bad Air");
    delay(1000);
    //lcd.clear();
  }

  if (gasValue > 600)
  {
    //Blynk.email("shameer50@gmail.com", "Alert", "Bad Air!");
    Blynk.logEvent("pollution_alert", "Bad Air");
  }
  if (isButtonPressed==0 && flag==0) {
    Serial.println("Its Raining");
     //Blynk.email("shameer50@gmail.com", "Alert", "It's Raining");
     Blynk.logEvent("rain_alert","Its Raining");
    flag=1;
  }
  else if (isButtonPressed==1)
  {
    flag=0;
  }
   
}