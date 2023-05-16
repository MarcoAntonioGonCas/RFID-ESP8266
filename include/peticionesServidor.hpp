// Documento json que contendra informacion para solicitudes htpp
StaticJsonDocument<48> json;
String jsonStr;




//Cliente htpp para realizar solicituedes http al servidor
//Correcion de error
HTTPClient http;
WiFiClient cli;
WiFiClientSecure *cliHttps = new WiFiClientSecure();

//Objeto para guardar certificados
X509List* listaCertificados;
bool wifiClientConfigurado = false;


//Indica si el url sel servidor es una conexion HTTPS
static bool esHttps(){
   return !certificadoHttps.isEmpty() and 
         (!serverIp.startsWith("http://") or !serverIp.startsWith("HTTP://"));
}

//Configura el cliente
//Si es una conexion HTTPS se agrega el certificado
void configurarClienteHttp(){
   if(esHttps()){
      listaCertificados = new X509List(certificadoHttps.c_str());
      cliHttps->setTrustAnchors(listaCertificados);
      
   }
   wifiClientConfigurado = true;
}


//Agrega las cabezeras al cliente HTTP
void addHeader(HTTPClient& http){
   
   http.addHeader("Authorization", "Bearer " + String(token));
}


//Obtiene el cliente de acuerdo a si es una conexion HTTP o HTTPS
static WiFiClient* ObtenerClienteWifi(){

   if(esHttps()){
      return cliHttps;
   }else{
      return &cli;
   }
}


/// @brief Prueba la conexion con el servidor y devuelve el codgigo de respúesta
int probarConexionServidor(){
   if(!wifiClientConfigurado)configurarClienteHttp();

   WiFiClient* cli = ObtenerClienteWifi();
   int httpCode  = -1;

   Serial.println("Realizando peticion get server");

   //TODO: Verificar si funciona el proxy
   if(proxyHabilitado) cli->connect(proxy.toString(),puerto);
   if(http.begin(*cli,serverIp)){
         addHeader(http);
         Serial.println("Realizando peticion get server 2");
         httpCode = http.GET();
         Serial.println("Realizando peticion get server 2");


         if(httpCode > 0){
            Serial.println(F("Peticion realizada con exito"));
         }else{
            Serial.println(F("Erro al realizar la peticion"));
            Serial.println(http.errorToString(httpCode));
         }
         // http.end();
         // cli->flush();
         // cli->stop();
         
    }


   Serial.println("Probando conexion con el seridor codgo de respuesta " + String(httpCode));
   return httpCode;

}

//===================================================================
// Realiza una peticion POST a nuestra api
// Utiliza la ruta de la Api configurada en archivos de configuracion
//===================================================================
void enviarPostApi(String &uuid)
{
   if(!wifiClientConfigurado)configurarClienteHttp();
   if (WiFi.status() != WL_CONNECTED)
   {
      Serial.println("Wifi no conectado");
      return;
   }
   
   json["uuid"] = uuid;
   serializeJson(json, jsonStr);

   WiFiClient* cli = ObtenerClienteWifi();
   Serial.println("Enviando peticion POST Tarjeta detectada");

   if(http.begin(*cli,(serverIp + modoRegistro ? rutaApiRegistro:rutaApi))){
      
      addHeader(http);
      http.addHeader("Connection", "keep-alive");
      http.addHeader("Content-Type", "application/json");

      int code = http.POST(jsonStr.c_str());

      if (code > 0)
      {
         Serial.println(F("Peticion realizada con exito"));
      }
      else
      {
         Serial.println(F("Erro al realizar la peticion"));
         Serial.println(http.errorToString(code));
      }

      http.end();
      cli->stop();
      cli->flush();
      json.clear();
      jsonStr.clear();
   }  
   
}
