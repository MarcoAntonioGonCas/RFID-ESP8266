//=====================================================
// WIFI
String ssid = "TP-Link_8BB6";
String password = "71904382";
String serverIp = "http://192.168.0.104:44355";
String rutaApi = "/api/ident/";

//=====================================================
// DNS
const byte DNS_PORT = 53;
//=====================================================
// Punto de acceso
IPAddress apIp(192, 168, 1, 1);

//=====================================================
// Pines
uint8_t pinLed = LED_BUILTIN;
uint8_t pinLedWIFI = D2;
uint8_t pinRS = D1;
uint8_t pinCS = D8;
//=====================================================
