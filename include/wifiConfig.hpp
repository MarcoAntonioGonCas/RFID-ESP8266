#include <Arduino.h>
#include <ESP8266WiFi.h>

void iniciarWifi(const char* ssid,const char* password){
  WiFi.mode(WiFiMode::WIFI_AP_STA);
  WiFi.begin(ssid,password);


  Serial.println("Conectando");
  while(WiFi.status() != wl_status_t::WL_CONNECTED){
    Serial.print(".");
    delay(100);
  }
  Serial.println("Conectado");
}