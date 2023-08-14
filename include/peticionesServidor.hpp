// Documento json que contendra informacion para solicitudes htpp
StaticJsonDocument<350> json;
String jsonStr;


//Cliente htpp para realizar solicituedes http al servidor
//Declarado globalmente para evitar saturacion de memoria 
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

         // Limpia la peticion http
         http.end();
         cli->flush();
         cli->stop();
         
    }


   Serial.println("Probando conexion con el seridor codgo de respuesta " + String(httpCode));
   return httpCode;

}





/// @brief Procesa la respuesta de un registro mandado
void procesarRespuestaRegistro(){

}


//Enumeracion del tipo de asistencia recibida por el servidor
enum class TipoAsistencia:short{
   Salida,
   Entrada,
   Ninguna,
   NoRegistrado
};

//Convierto un tipo short a una enumeracion de TipoAsistencia 
TipoAsistencia ConvertirATipoAsistencia(short valor){

   return static_cast<TipoAsistencia>(valor);
}

/// @brief Procesa la respuesta de un acceso
void procesarRespuestaAcceso(JsonObject& json ){
   Serial.println("Respuesta");

   if(!json.containsKey("OK") and !json.containsKey("TipoAsistencia")){
      return;
   }
   TipoAsistencia tipo = ConvertirATipoAsistencia(json["TipoAsistencia"].as<short>());
   bool ok = json["OK"].as<bool>();
   if(!ok) return;


   if(tipo == TipoAsistencia::Entrada){
      ledAcceso.prender(200);
   }else if(tipo == TipoAsistencia::Salida){
      ledAcceso.prender(100,50,3);
   }else if(tipo == TipoAsistencia::NoRegistrado){

   }



   serializeJsonPretty(json,Serial);


}


//===================================================================
// Realiza una peticion POST a nuestra api
// Utiliza la ruta de la Api configurada en archivos de configuracion
//===================================================================
void enviarPostApi(String &uuid)
{
   if (!wifiConectado())
   {
      Serial.println("Wifi no conectado");
      return;
   }

   if(!wifiClientConfigurado) configurarClienteHttp();
   
   json["codigoTarjeta"] = uuid;
   json["codigoIntercambio"] = codigoIntercambio;
   json["salon"] = salon;

   serializeJsonPretty(json,Serial);
   serializeJson(json, jsonStr);

   WiFiClient* cli = ObtenerClienteWifi();
   String url = "";
   if(modoRegistro){
      url = serverIp + rutaApiRegistro; 
   }else{
      url = serverIp + rutaApi; 
   }
   
   Serial.println("Enviando peticion POST Tarjeta detectada a: "+url);
   http.begin(*cli,url);
      
      addHeader(http);
      http.addHeader("Connection", "keep-alive");
      http.addHeader("Content-Type", "application/json");

      int code = http.POST(jsonStr.c_str());

      if (code > 0)
      {
         Serial.println(F("Peticion realizada con exito"));
         if(modoRegistro){
            procesarRespuestaRegistro();
         }else{
            json.clear();
            jsonStr.clear();
            jsonStr = http.getString();


            deserializeJson(json,jsonStr);

            JsonObject obj = json.as<JsonObject>();
            procesarRespuestaAcceso(obj);
         }
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
