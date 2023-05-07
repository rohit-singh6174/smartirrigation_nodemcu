#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <Blynk.h>

char auth[] = "<YourBlynk-API>";
char ssid[] = "<YourWifi-SSID>";  // type your wifi name
char pass[] = "<Wifi-Password>";  // type your wifi password of your Wi-Fi
int water ;
int prevState = 0;
#define DHTPIN D1 
#define DHTTYPE DHT11 
BlynkTimer timer;
DHT dht(DHTPIN, DHTTYPE);


void sendSensor(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, t);
  Blynk.virtualWrite(V6, h);  
 
  Serial.println("Temperature: "+String(t)+"°C");
  Serial.println("Humidity: "+String(h)+"%");
}

void setup()
{  
Serial.begin(9600);
dht.begin();
pinMode (D3,OUTPUT); //Waterpump
pinMode (D5,INPUT); // Soil Mositure
Blynk.begin(auth, ssid, pass);
timer.setInterval(1000L, sendSensor);

}

void loop()
{
Blynk.run();


water=digitalRead(D5);
Blynk.virtualWrite(V0,water);

  if (water == HIGH && prevState == 0)
  {
  Serial.println("Pump Started, Water Flowing");
  
  digitalWrite(D3,LOW);

  //Blynk.notify(“Pump Started, Water Flowing”);
  prevState = 1;
  delay(400);
  }

  else if (water == HIGH && prevState == 1)
  {
  Serial.println("Pump Stopped, Water Not Flowing");
  delay(400);
  }
  else
  {
  digitalWrite(D3,HIGH);
  prevState =0;
  }
delay(100);
timer.run();
}
