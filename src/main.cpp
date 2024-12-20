//----------------------------------------
// Importamos las librerias a utlizar
//----------------------------------------
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
#include <WiFiClientSecure.h>


//Obtiene  la memoria libre al iniciar el ESP
uint32_t stackInicial = ESP.getFreeHeap();

// Objeto en donde indicamos la ruta de nuestro websocket
AsyncWebSocket asyncSocket("/ws");

// Objeto en donde indicamos el puerto por el cual se ejecutara //nuestro servidor
AsyncWebServer asyncServer(80);
DNSServer dnsServer;

// Creamos dos objetos leds los cuales nos ayudaran a mostrar el estado del Wi-fi
// y si se ha leido una tarjeta rfid
//-----------------------------------------------
ledLibClass ledRFID;
ledLibClass ledWIFI;
ledLibClass ledAcceso;

bool enviarEstado = false;
//-----------------------------------------------
// Incluimos los archivos de cabezera que tenemos
// en el archivo includes.hpp
//-----------------------------------------------
#include "includes.hpp"


//=============================================================
// Inicia la configuracion del wifi tanto modo estacion como modo
// punto de acceso 
//=============================================================
void iniciarWIFIAP(){

  delayMicroseconds(100);
   
  configAPWIFI();
  iniciarAP();
  conectarWiFi();
}
//=============================================================
// Inicia los eventos websockets
//=============================================================
void iniciarSocket()
{
  asyncSocket.onEvent(&onWsEvent);
}

//=============================================================
// Inicia el servidor web al igual que el DNS
//=============================================================
void iniciarServerYDNS()
{
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
// Inicia los objetos leds con lo pines configurados en config.h
//=============================================================
void iniciarLeds()
{
  ledRFID.begin(pinLedInfoRfid, TipoLed::Catodo);
  ledWIFI.begin(pinLedWIFI, TipoLed::Anodo);
  ledAcceso.begin(pinLedEstadoAcceso,TipoLed::Anodo);
}

//=============================================================
// Inicia el sistema de archivos en nuestro ESP para leer archivos
// y escribir en el
//=============================================================


bool iniciarLittleFS()
{
  Serial.println();
  Serial.println(F("Lectura de Tarjeta"));

  Serial.println(F("Montando LittleFS"));
  if (!LittleFS.begin())
  {
    Serial.println(F("LittleFS Error al montar"));
    return false;
  }

  return true;
}
//=============================================================
// Metodo auxiliar en donde se iniciaran todos los objetos de la
// con sus respectivas configuraciones de config.h
//=============================================================
bool iniciaTodo()
{
  setNameAP();
  //-----------LEDS-----------------
  iniciarLeds();
  //----------LITTLEFS------------------
  iniciarLittleFS();
  leerConfig();
  //-----------WIFI-----------------
  iniciarWIFIAP();
  //-----------Servidor-----------------
  iniciarServerYDNS();
  //-----------Boton
  pinMode(pinbtnAPReset,INPUT_PULLUP);
  return true;
}

//=============================================================
// Metodo principal en donde inicia nuestra aplicacion
// Este solo se ejecutar una  vez, al principio
//=============================================================
void setup()
{
  
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  
  iniciaTodo();
}

//=============================================================
// Metodo loop el cual se ejecutara infinitamente
//=============================================================
void loop()
{
  
  //Indica si se enviara en mensaje atravez de websocket
  //El cual inidicara si se establecio conexion exitosa con el 
  //servidor
  if(enviarEstado){
    enviaEatadoServidorSocket();
    delayMicroseconds(100);
    enviarEstado = false;
  }

  
  loopRestart();
  dnsServer.processNextRequest();


  ledWIFI.loop();
  ledRFID.loop();
  ledAcceso.loop();


  //Lops
  loopWiFi();
  loopAP();
  loopRfid();
  loopButtonAPReset();
  loopSocket();
}