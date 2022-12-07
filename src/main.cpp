#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <LedLib.h>
//-------------------------------
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <DNSServer.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <LittleFS.h>
//---------------------------------------
#include "config.hpp"
#include "wifiConfig.hpp"
//------------------------------------
#include "rfidHelp.hpp"
#include "asyncServer.hpp"
//------------------------------------


MFRC522 rfid(pinCS, pinRS);
ledLibClass le;
AsyncWebSocket asyncSocket("/ws");
AsyncWebServer asyncServer(80);
DNSServer dnsServer;

const byte DNS_PORT = 53;

void ProcessRequest(AsyncWebSocketClient* client,String mensaje){

}
void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){ 
   if(type == WS_EVT_CONNECT){
      //Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
      client->printf("Hello Client %u :)", client->id());
      client->ping();
   } else if(type == WS_EVT_DISCONNECT){
      //Serial.printf("ws[%s][%u] disconnect: %u\n", server->url(), client->id());
   } else if(type == WS_EVT_ERROR){
      //Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
   } else if(type == WS_EVT_PONG){
      //Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len)?(char*)data:"");
   } else if(type == WS_EVT_DATA){
      AwsFrameInfo * info = (AwsFrameInfo*)arg;
      String msg = "";
      if(info->final && info->index == 0 && info->len == len){
         if(info->opcode == WS_TEXT){
            for(size_t i=0; i < info->len; i++) {
               msg += (char) data[i];
            }
         } else {
            char buff[3];
            for(size_t i=0; i < info->len; i++) {
               sprintf(buff, "%02x ", (uint8_t) data[i]);
               msg += buff ;
            }
         }
         if(info->opcode == WS_TEXT){

         }
         
      } else {
         //message is comprised of multiple frames or the frame is split into multiple packets
         if(info->opcode == WS_TEXT){
            for(size_t i=0; i < len; i++) {
               msg += (char) data[i];
            }
         } else {
            char buff[3];
            for(size_t i=0; i < len; i++) {
               sprintf(buff, "%02x ", (uint8_t) data[i]);
               msg += buff ;
            }
         }
         Serial.printf("%s\n",msg.c_str());
         if((info->index + len) == info->len){
            if(info->final){
               if(info->message_opcode == WS_TEXT)
               ProcessRequest(client, msg);
            }
         }
      }
   }
}
void iniciarSocket(){

}
void iniciarServerYDNS(){
  WiFi.mode(WiFiMode::WIFI_AP_STA);
  WiFi.softAPConfig(apIp,apIp,IPAddress(255,255,255,0));
  //=======================================================
  while(!WiFi.softAP("RFID_AP","RFID2022MA")){
      Serial.print(".");
      delay(100);
  }

  //dns
  dnsServer.setTTL(300);
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
  dnsServer.start(DNS_PORT,"www.rfid.com",apIp);
  //--------------------------------------------
  addRouters(asyncServer);
  asyncServer.addHandler(&asyncSocket);
  asyncServer.begin();
}


void listDir(const char *dirname) {
  Serial.printf("Listing directory: %s\n", dirname);

  Dir root = LittleFS.openDir(dirname);

  while (root.next()) {
    File file = root.openFile("r");
    Serial.print("  FILE: ");
    Serial.print(root.fileName());
    Serial.print("  SIZE: ");
    Serial.print(file.size());
    time_t cr = file.getCreationTime();
    time_t lw = file.getLastWrite();
    file.close();
    struct tm *tmstruct = localtime(&cr);
    Serial.printf("    CREATION: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
    tmstruct = localtime(&lw);
    Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
  }
}
void setup()
{
  Serial.begin(9600);
//   SPI.begin();
//   rfid.PCD_Init();
//   //----------------------------
  Serial.println();
  Serial.println("Lectura de Tarjeta");
  le.begin(pinLed, TipoLed::Catodo);

  //----------Wifi
  //iniciarWifi();
  Serial.println("Mount LittleFS");
  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount failed");
    return;
  }

//   iniciarServerYDNS();
}

void loop()
{
   listDir("/");
   delay(3000);
//   dnsServer.processNextRequest();
//   le.loop();
//   if (tarjetaDisponible(rfid))
//   {
//     le.prender(100, 50, 4);
//     String strId = leerTarjeta(rfid);
//     Serial.println(strId);

//   }
}