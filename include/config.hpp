//=====================================================
//Archivo de configuraciones
//=====================================================


//=====================================================
// WIFI pagina web
//=====================================================

bool proxyHabilitado = false;
bool reintentarConexionWifi = true;
int puerto = 0;
IPAddress proxy;
String password = "Redes1245@";
String ssid = "INFINITUMBCF1_2.4";



//=====================================================
// Ruta del servidor
//=====================================================
// Indica la ruta de nuestro servidor
String serverIp = "http://192.168.0.104:44355";

// Indica la ruta  en donde se recibira nuestro codigo RFID
String rutaApi = "/api/AccesoRfid/";

// Indica la ruta  en donde se recibira nuestro codigo RFID para el modo registro
String rutaApiRegistro = "/api/RegistroRfid/";


// En caso de se un servidor https se debera agregar ese certificado
String certificadoHttps = "";

// Codigo de intercambio para evitar que se envie a todos los usuarios 
String codigoIntercambio = "";

// Salon en el cual se encuenta el modulo
String salon = "";



// indica si el modulo se encuentra en modo registro o modo de acceso
bool modoRegistro = false;


//=====================================================
//Punto de acceso
//=====================================================


bool apHabilitado = true;
IPAddress apIp(192, 168, 10, 1);
String passwordAP ="";
String ssidAP = "RFID_AP";

//=====================================================
// DNS
//=====================================================

const byte DNS_PORT = 53;

//=====================================================
// usuerio y contraseña para la login  pagina web embebida
String contraLogin = "admin";
String usuarioLogin = "admin";

// Token para realizar la peticion http a webserver IIS ASP
String token = "api123";

// Interrumpe el socket
bool interrumpirSocked = false;

