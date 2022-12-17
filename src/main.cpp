#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <LittleFS.h>
#include <SPI.h>
#include <DNSServer.h>
//-----------------Pantalla oled-----------------------
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "pantallaOled.hpp"

//---------------------------------------
#include <LedLib.h>
#include <MFRC522.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

//---------------------------------------
#include "config.hpp"
#include "wifiConfig.hpp"
#include "configHelper.hpp"
//------------------------------------
#include "rfidHelp.hpp"
#include "asyncServer.hpp"
//------------------------------------
MFRC522 rfid(pinCS, pinRS);
ledLibClass ledRFID;
ledLibClass ledWIFI;


AsyncWebSocket asyncSocket("/ws");
AsyncWebServer asyncServer(80);
DNSServer dnsServer;


//----------------------------------------


void iniciarSocket(){

}
void iniciarServerYDNS(){

  dnsServer.setTTL(300);
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
  dnsServer.start(DNS_PORT,"www.rfid.com",apIp);
  //--------------------------------------------
  addRouters(asyncServer);
  asyncServer.addHandler(&asyncSocket);
  asyncServer.begin();
  //----------------------------

}
void actualizaEstadoWiFi(){
  if(!WiFi.localIP().isSet()){
    ledWIFI.prenderInfinito(1000,500);
  }else{
    ledWIFI.parar();
    ledWIFI.prender();
  }
}
void iniciarLeds(){
  ledRFID.begin(pinLed, TipoLed::Catodo);
  ledWIFI.begin(pinLedWIFI,TipoLed::Anodo);
}
bool iniciarLittleFS(){
  Serial.println();
  Serial.println(F("Lectura de Tarjeta"));
  
  
  Serial.println(F("Montando LittleFS"));
  if (!LittleFS.begin()) {
    Serial.println(F("LittleFS Error al montar"));
    return false;
  }

  return true;
}
void setup()
{
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  //-----------LEDS-----------------
  iniciarLeds();
  //----------LITTLEFS------------------
  iniciarLittleFS();
  cargarConfi();
  //-----------WIFI-----------------
  iniciarSTAWiFI();
  conectarWiFi();
  //-----------Servidor-----------------
  iniciarServerYDNS();
}

void loop()
{
  dnsServer.processNextRequest();
  actualizaEstadoWiFi();
  ledRFID.loop();
  ledWIFI.loop();
  if (tarjetaDisponible(rfid))
  {
    ledRFID.prender(100, 50, 4);
    String strId = leerTarjeta(rfid);
    enviarPostApi(strId);
    Serial.println(strId);

    Serial.printf("Espacio total %d \n",ESP.getFlashChipSize());
    Serial.printf("Espacio disponible stack %d \n",ESP.getFreeContStack());
    Serial.printf("Espacio disponible heap %d \n",ESP.getFreeHeap());
  }
}