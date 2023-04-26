#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#define BLYNK_PRINT Serial
char auth[] = "QOSXaduVtlUcTstpOqJYY7nQjB10oRm9";
char ssid[] = "NPL";  // type your wifi name
char pass[] = "CSRF3993";  // type your wifi password


#define DHTPIN 2     // Digital pin connected to the DHT sensor

#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);


const int moisturePin = A0; // Analog pin for soil moisture sensor
int moistureValue = 0; 
BlynkTimer timer;
float  wet=300;
float  dry=1024;

void sendSensor()
{
   float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
 
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

   moistureValue = analogRead(moisturePin);
   float percentage = (float)((moistureValue - wet)*100)/(dry - wet);

   Blynk.virtualWrite(V0, percentage);
   
   if(percentage < 30.0){
    Serial.println("Soil is Wet");
   Serial.println(percentage);
   }
   else{
     Serial.println("Soil is Dry  Water needed");
     Serial.println(percentage);
   }

}

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  
  Blynk.begin(auth, ssid, pass);
  
  dht.begin();
  pinMode(moisturePin, INPUT);
  timer.setInterval(1000L, sendSensor);

}

void loop() {
  // put your main code here, to run repeatedly:
 Blynk.run();
  timer.run();
}
