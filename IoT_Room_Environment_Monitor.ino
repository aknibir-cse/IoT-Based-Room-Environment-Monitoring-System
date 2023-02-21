#define BLYNK_TEMPLATE_ID "TMPLOrNCUJEU"
#define BLYNK_DEVICE_NAME "IoT Room Environment Monitor"
#define BLYNK_AUTH_TOKEN "sWpqqzXKEddBMsBhkYbK13W_Qo7io3v8"

#define BLYNK_PRINT Serial

#define DHTTYPE DHT11
#define DHTPIN D2

#include <ESP8266WiFi.h>
#include <DHT.h>
#include <BlynkSimpleEsp8266.h>

 
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Alkayda";  
char pass[] = "0987654321";


int gas_sensor = A0;
int buzzer = D8;
int redLed = D7;
int greenLed = D5;
int yellowLed = D4;

int data = 0;
int sensorThres = 130;
float overHeat = 35.00;
float humidity,temperature;

BlynkTimer timer;
 
DHT dht(DHTPIN, DHTTYPE);

void gasSensor(){
  
    int data = analogRead(gas_sensor);
    Blynk.virtualWrite(V0, data);
    Serial.print("Gas Level : ");
    Serial.println(data);
  
    if(data > sensorThres){
         Blynk.email("aknibir.cse@gmail.com", "Alert", "Toxic Gas Detected !");
         Blynk.logEvent("gas_alert","Toxic Gas Detected !");
         digitalWrite(yellowLed, HIGH);
         digitalWrite(greenLed, LOW);
         tone(buzzer, 5500, 10000);
    }
  
    else{
         digitalWrite(yellowLed, LOW);
         digitalWrite(greenLed, HIGH);
         noTone(buzzer);
    }
  
}

void dhtSensor(){
 
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();

    if (isnan(humidity) || isnan(temperature)) {
        Serial.println("Failed to read from DHT sensor !");
        return;
    }
    
    else{
        Blynk.virtualWrite(V1, temperature);
        Blynk.virtualWrite(V2, humidity);
        Serial.print("Temperature : ");
        Serial.print(temperature);
        Serial.print("\n");
        Serial.print("Humidity : ");
        Serial.println(humidity);
    }

    if(temperature > overHeat){
        Blynk.email("aknibir.cse@gmail.com", "Alert", "Overheated your Room !");
        Blynk.logEvent("over_heat","Overheated your Room");
        digitalWrite(yellowLed, HIGH);
        digitalWrite(greenLed, LOW);
        digitalWrite(redLed, LOW);
        tone(buzzer, 5500, 4000);
    }
    
    else{
        digitalWrite(yellowLed, LOW);
        digitalWrite(greenLed, HIGH);
        noTone(buzzer);
    }

}

void fireAlert(){
  
    if(data > sensorThres || temperature > overHeat ){
        Blynk.email("aknibir.cse@gmail.com", "Alert", "Fire detected in Room !");
        Blynk.logEvent("fire_alert","Fire detected in Room !");
        digitalWrite(redLed, HIGH);
        digitalWrite(yellowLed, LOW);
        digitalWrite(greenLed, LOW);
        tone(buzzer, 5500, 10000);
    }
    
    else{
        digitalWrite(greenLed, HIGH);
        digitalWrite(redLed, LOW);
        noTone(buzzer);
    }
  
}

void setup(){
  
    Serial.begin(115200);
    pinMode(redLed, OUTPUT);
    pinMode(greenLed, OUTPUT);
    pinMode(yellowLed, OUTPUT);
    pinMode(buzzer, OUTPUT);
    pinMode(gas_sensor, INPUT);
   
    Blynk.begin(auth, ssid, pass);
    dht.begin();
  
    timer.setInterval(100L, gasSensor);
    timer.setInterval(100L, dhtSensor);
    timer.setInterval(100L, fireAlert);    
    
}

void loop(){
  
    Blynk.run();
    timer.run();
  
}
