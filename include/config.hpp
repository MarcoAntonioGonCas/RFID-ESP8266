#include <Arduino.h>

//=====================================================
//WIFI
const char *ssid = "TP-Link_8BB6";
const char *password = "71904382";
String ServerIp  = "http://192.168.0.104:44355";
String rutaApi = "/api/ident/";

//=====================================================
//Punto de acceso
IPAddress apIp(192,168,1,1);


//=====================================================
//Pines 
uint8_t pinLed = LED_BUILTIN;
uint8_t pinRS = D1;
uint8_t pinCS = D8;
//=====================================================
