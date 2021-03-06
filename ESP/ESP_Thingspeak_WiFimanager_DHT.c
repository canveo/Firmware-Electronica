#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <SimpleDHT.h>

String apiKey = "EHW19OYPTK9J7F40";
const char* server = "api.thingspeak.com";
int pinDHT11 = D0;
int timeOut = 0;
SimpleDHT11 dht11;
WiFiClient client;

void setup(){
   WiFiManager wifiManager;
   Serial.begin(115200);
   wifiManager.resetSettings(); //Poner en un boton de reset en loop por tiempo
   delay(10);
   pinMode(A0, INPUT);
   wifiManager.autoConnect("AutoConnectAP");
   Serial.println("connected...yeey :)");
}
void loop(){
   byte temperatura = 0;
   byte humedad = 0;
   float luminosidad = analogRead(A0);
   if(dht11.read(pinDHT11, &temperatura, &humedad, NULL)){
      Serial.print("Read DHT11 failed.");
      return;
   }
   if(timeOut==30){
      if(client.connect(server,80)){
         String postStr = apiKey;
         postStr +="&field1=";
         postStr += String(luminosidad);
         postStr +="&field2=";
         postStr += String((int)temperatura);
         postStr +="&field3=";
         postStr += String((int)humedad);
         postStr += "\r\n\r\n";
         client.print("POST /update HTTP/1.1\n");
         client.print("Host: api.thingspeak.com\n");
         client.print("Connection: close\n");
         client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
         client.print("Content-Type: application/x-www-form-urlencoded\n");
         client.print("Content-Length: ");
         client.print(postStr.length());
         client.print("\n\n");
         client.print(postStr);
         Serial.println("% send to Thingspeak");
      }
      client.stop();
      Serial.print("luminosidad: ");
      Serial.print(luminosidad);
      Serial.print(" | temperatura: ");
      Serial.print(temperatura);
      Serial.print(" | humedad: ");
      Serial.println(humedad);
      timeOut=0;
      
   }
   delay(1000);
   timeOut++;
   Serial.print("timeOut");
   Serial.println(timeOut);
}
