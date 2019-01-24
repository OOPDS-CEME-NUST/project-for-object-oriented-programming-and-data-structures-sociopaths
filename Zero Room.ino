//#include <Wire.h>
#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>
#include "DHTesp.h"

int temp_w = 30;
int temp_s = 26;
int gas_ = 125;
int humidity_h = 50;
int humidity_l = 30;
char mode_ = 'W';
const char* ssid = "Sufi";
const char* password = "generic pass";
//pinMode (A0, INPUT);

class Data
{
  public:
    Data(){};
    float  value;
};
Data humidity, temperature, gas;
//float gas;

const char* host = "dweet.io";
const char* jeff= "zero_room";

LiquidCrystal_I2C lcd(0x3F, 16, 4);

DHTesp dht;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(10);
  dht.setup(4, DHTesp::DHT11);
  lcd.begin(0, 2);
  lcd.print("Connecting to");
  lcd.setCursor(0, 2);
  lcd.print(ssid);
  pinMode(D8, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  //Connecting to WiFi
  Serial.println();
  Serial.println();
  Serial.print( "Connecting to \n");

  Serial.println(ssid);
  delay(3000);
  //lcd.clear();
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    lcd.print("Connecting");
    delay(250);
    lcd.clear();
    lcd.print("Connecting.");
    delay(250);
    lcd.clear();
    lcd.print("Connecting..");
    delay(250);
    lcd.clear();
    lcd.print("Connecting...");
    delay(250);
    lcd.clear();

    Serial.print(".");
  }

  delay(500);
  lcd.clear();
  lcd.print("Connected");
  delay(2000);
  lcd.clear();
  Serial.println("");
  Serial.println("WiFi connected");
}

int x = 1;


void loop() {
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    lcd.clear();
    lcd.print("Connecting to");
    lcd.setCursor(0, 1);
    lcd.print("Freeboard.io");
    return;
  }
  // put your main code here, to run repeatedly:
  //if (millis() % 2000 == 0)
  // {
  lcd.clear();
  humidity.value = dht.getHumidity();
  temperature.value = dht.getTemperature();

  lcd.print("Temp: ");

  lcd.print(temperature.value);
  if (temperature.value > temp_w && mode_ == 'W')
  {
    digitalWrite(D8, LOW);
    Serial.println("abcd");
  }
  else if (temperature.value < temp_w && mode_ == 'W')
  {
    Serial.println("abc");
    digitalWrite(D8, HIGH);
  }
  if (temperature.value > temp_s && mode_ == 'S')
  {
    digitalWrite(D1, HIGH);
  }
  else if (temperature.value < temp_s && mode_ == 'S')
  {
    digitalWrite(D1, LOW);
  }


  lcd.setCursor(0, 1);

  lcd.print("Humid: ");

  lcd.print(humidity.value);
  lcd.print("%");
  if (humidity.value < humidity_l)
  {
    digitalWrite(D5, HIGH);
  }
  else if (humidity.value > humidity_h)
  {
    digitalWrite(D5, LOW);
  }
  gas.value = analogRead(A0);

  lcd.setCursor(-4, 2);

  lcd.print("Gas: ");
  lcd.print(gas.value);
  if (gas.value > 125)
  {
    digitalWrite(D6, HIGH);
  }
  else {
    digitalWrite(D6, LOW);
  }
  client.print(String("GET /dweet/for/") + String(jeff) + String ("?temperature=") + String(temperature.value) + "&humidity=" + String(humidity.value) + "&gas=" + String(gas.value) + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n\r\n");

  delay(1000);
  // }

}
