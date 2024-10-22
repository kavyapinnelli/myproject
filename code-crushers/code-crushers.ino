#define BLYNK_TEMPLATE_ID "TMPL3Aq2CQcue"
#define BLYNK_TEMPLATE_NAME "ESP32"
#define BLYNK_AUTH_TOKEN "xrUU2WpT6mphgTaKqcLobFPs34RoX1PY"
#define BLYNK_PRINT Serial

#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <DHT.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

const char* ssid="Kavya";
const char* password="12345678";

Adafruit_BMP085 bmp;
DHT dht(23,DHT11);

int sensor=32;
int lo1=4;
int lo2=2;

void setup() {
  projectInit();
}

void loop() {
  int p=getPressure();
  int t=getTemperature();
  int h=getHumidity();

  static unsigned long startTime=millis();
  static int peakValue=0;
  int ecgValue=0;

  if((digitalRead(lo1)==1) || (digitalRead(lo2)==1)){
    Serial.println("!");
  } else {
    ecgValue=analogRead(sensor);
    if(ecgValue>peakValue){
      peakValue=ecgValue;
    }
  }

  Serial.print("P: ");
  Serial.print(p);
  Serial.print(", H: ");
  Serial.print(h);
  Serial.print(", T: ");
  Serial.print(t);
  Serial.print(", ECG: ");
  Serial.println(ecgValue);

  Blynk.virtualWrite(V0,h);
  Blynk.virtualWrite(V1,t);
  Blynk.virtualWrite(V2,p);
  Blynk.virtualWrite(V3,ecgValue);

  if(ecgValue>3000){
    Blynk.logEvent("ecg_alert","Your ECG Reading is High, Please take care");
  }
  if(t>40){
    Blynk.logEvent("temp_alert","Your body temperature detects high");
  }
  if(p>101000){
    Blynk.logEvent("pressure_alert","Your body detects high pressure");
  }
  delay(1000);
}

void projectInit(){
  Serial.begin(9600);
  dht.begin();
  pinMode(sensor,INPUT);
  pinMode(lo1,INPUT);
  pinMode(lo2,INPUT);

  Blynk.begin(BLYNK_AUTH_TOKEN,ssid,password);
  
  if (!bmp.begin()) {
  Serial.println("Could not find a valid BMP085/BMP180 sensor, check wiring!");
  while (1) {}
  }
}

int getHumidity(){
  int h=dht.readHumidity();
  if(isnan(h)){
    return -1;
  }
  return h;
}
float getTemperature(){
  return bmp.readTemperature();
}

int getPressure(){
  return bmp.readPressure();
}
