//=====================================================
//Archivo de configuraciones
//=====================================================


//=====================================================
// WIFI pagina web
//=====================================================

String ssid = "TP-Link_8BB6";
String password = "71904382";
bool proxyHabilitado = false;
IPAddress proxy;
int puerto = 0;

//=====================================================
// Ruta del servidor
//=====================================================
String serverIp = "http://192.168.0.104:44355";
String rutaApi = "/api/ident/";
String rutaApiRegistro = "/api/registro/";

String certificadoHttps = "";
String codigoIntercambio = "";


bool modoRegistro = false;


//=====================================================
//Punto de acceso
//=====================================================
String ssidAP = "RFID_AP";
String passwordAP ="";
bool apHabilitado = true;

IPAddress apIp(192, 168, 1, 1);
//=====================================================
// DNS
//=====================================================
const byte DNS_PORT = 53;

//=====================================================
//usuerio y contraseña para la login  pagina web embebida
String usuarioLogin = "admin";
String contraLogin = "admin";

//Token para realizar la peticion http a webserver IIS ASP
String token = "api123";


bool interrumpirSocked = false;