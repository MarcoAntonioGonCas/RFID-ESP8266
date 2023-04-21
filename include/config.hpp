//=====================================================
//Archivo de configuraciones
//=====================================================
// Cliente htpp para realizar solicituedes http al servidor
//Correcion de error
HTTPClient http;
WiFiClient cli;

//=====================================================
// WIFI pagina web
//=====================================================

String ssid = "TP-Link_8BB6";
String password = "71904382";

bool proxyHabilitado = false;
IPAddress* proxy;
int puerto = 0;


//=====================================================
// Ruta del servidor
//=====================================================
String serverIp = "http://192.168.0.104:44355";
String rutaApi = "/api/ident/";
bool modoRegistro = false;


//=====================================================
//Punto de acceso
//=====================================================
String ssidAP = "RFID_AP";
String passwordAP ="RFID2022MA";
bool apHabilitado = true;

//=====================================================
// DNS
//=====================================================
const byte DNS_PORT = 53;
//=====================================================
// Punto de acceso
//=====================================================
IPAddress apIp(192, 168, 1, 1);

//=====================================================
// Pines
uint8_t pinLed = LED_BUILTIN;
uint8_t pinLedWIFI = D2;
uint8_t pinRS = D0;
uint8_t pinCS = D8;
//=====================================================
//usuerio y contraseña para la login  pagina web embebida
const char *usario = "rfid";
const char *contra = "1234";


//Token para realizar la peticion http a webserver IIS ASP
const char* token = "api123";