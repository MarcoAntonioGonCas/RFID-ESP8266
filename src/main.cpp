
//----------------------------------------
//Importamos las librerias a utlizar
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <LittleFS.h>
#include <SPI.h>
#include <DNSServer.h>
//---------------------------------------
#include <LedLib.h>
#include <MFRC522.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
//-----------------------------------------------
//Incluimos los archivos de cabezera que tenemos
//en la carpeta include
//---------------------------------------
//Objeto en donde indicamos la ruta de nuestro websocket
AsyncWebSocket asyncSocket("/ws");
//Objeto en donde indicamos el puerto por el cual se ejecutara //nuestro servidor
AsyncWebServer asyncServer(80);
DNSServer dnsServer;
bool mostrarIPSTA = true;
bool debugWifi = false;

#include "config.hpp"
#include "funciones.hpp"
#include "wifiConfig.hpp"
#include "configHelper.hpp"
//------------------------------------
#include "rfidRead.hpp"

#include "configServer.hpp"
#include "peticionesServidor.hpp"
#include "asyncSocket.hpp"
#include "asyncServer.hpp"

//------------------------------------
//Este objeto es en donde indicaremos en que pines esta conectado nuestro
//Rfid el igual

MFRC522 rfid(pinCS, pinRS);

//Creamos dos objetos los cuales nos ayudaran a mostrar el estado del Wi-fi
//y si se ha leido una tarjeta rfid
ledLibClass ledRFID;
ledLibClass ledWIFI;



//=============================================================
//Inicia los eventos websockets
void iniciarSocket(){
  asyncSocket.onEvent(&onWsEvent);
}

//=============================================================
//Inicia el servidor web al igual que el DNS
void iniciarServerYDNS(){

  dnsServer.setTTL(300);
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
  dnsServer.start(DNS_PORT,"www.rfid.com",apIp);
  //--------------------------------------------
  addRouters(asyncServer);
  asyncServer.addHandler(&asyncSocket);
  iniciarSocket();
  asyncServer.begin();
  //----------------------------

}

//=============================================================
//Indica el estado de la conexion WI-Fi atravez de un led
void actualizaEstadoWiFi(){


    if(debugWifi){
      Serial.println("Comprbando si esta conectado al wifi");
      Serial.println(WiFi.localIP().isSet());
      Serial.println(WiFi.localIP());
      Serial.println("Comprobacion exitosa");

       
    }


  if(!WiFi.localIP().isSet()){
    ledWIFI.prenderInfinito(1000,500);
    mostrarIPSTA = true;
  }else{
    ledWIFI.parar();
    ledWIFI.prender();
    if(mostrarIPSTA){
      Serial.printf("Conectado a WIFI %s con ip: %s",WiFi.SSID().c_str(),WiFi.localIP().toString().c_str());
      mostrarIPSTA = false;
    }
  }
}

//=============================================================
//Inicia los objetos leds con lo pines configurados en config.h
void iniciarLeds(){
  ledRFID.begin(pinLed, TipoLed::Catodo);
  ledWIFI.begin(pinLedWIFI,TipoLed::Anodo);
}

//=============================================================
//Inicia el sistema de archivos en nuestro ESP para leer archivos
//y escribir en el
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
//=============================================================
//Metodo auxiliar en donde se iniciaran todos los objetos de la
//con sus respectivas configuraciones de config.h
bool iniciaTodo(){
//-----------LEDS-----------------
  iniciarLeds();
  //----------LITTLEFS------------------
  iniciarLittleFS();
  leerConfig();
  //-----------WIFI-----------------
  configAPWIFI();
  conectarWiFi();
  //-----------Servidor-----------------
  iniciarServerYDNS();

  return true;
}


//=============================================================
//Metodo principal en donde inicia nuestra aplicacion
//Este solo se ejecutar una  vez, al principio

void setup()
{
  setNameAP();


  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  iniciaTodo();
  
}

//=============================================================
//Metodo loop el cual se ejecutara infinitamente
void loop()
{

if(debugWifi)Serial.println("Comprobando dns siguiente respuesta");
    dnsServer.processNextRequest();
    if(debugWifi)Serial.println("Actaulizar estado wifi");

  actualizaEstadoWiFi();
  if(debugWifi)Serial.println("CLED RFID");
  ledRFID.loop();
  if(debugWifi)Serial.println("LED WIFI");
  ledWIFI.loop();

  if (tarjetaDisponible(rfid))
  {
    ledRFID.prender(100, 50, 4);
    String strId = leerTarjeta(rfid);
    enviarPostApi(strId);
    Serial.println(strId);
    // Serial.printf("Espacio total %d \n",ESP.getFlashChipSize());
    // Serial.printf("Espacio disponible stack %d \n",ESP.getFreeContStack());
    // Serial.printf("Espacio disponible heap %d \n",ESP.getFreeHeap());
  }
  enviarInfoCada(2000,asyncSocket);
  comprobarClientes(1000,asyncSocket);
}