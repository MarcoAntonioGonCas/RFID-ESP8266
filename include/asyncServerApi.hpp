//=============================================================
// Envia un json con las conexiones wifi actuales
// const String NombreSeguridad [] = {"ABIERTA","WEP","WPA_PSK","WPA2_PSK","WPA_WPA2_PSK","AUTH_MAX"};

// -------------------------------------------------------------------
// Petición de informacion de redes disponibles
// url: "/api/redes"
// Método: GET
// -------------------------------------------------------------------
void handleApiGetNetworks(AsyncWebServerRequest *req)
{

   int redesDispo = WiFi.scanComplete();

   StaticJsonDocument<600> jsonWifi;
   JsonArray jsonRedes = jsonWifi.createNestedArray("redes");
   
   if (redesDispo == -2)
   {
      jsonWifi["count"] = 0;
      WiFi.scanNetworks(true);
   }
   else if(redesDispo)
   {

      
      jsonWifi["count"] = redesDispo;
      for (int i = 0; i < redesDispo; ++i)
      {
         JsonObject jsonRed = jsonRedes.createNestedObject();
         jsonRed["ssid"] = WiFi.SSID(i);
         jsonRed["bssid"] = WiFi.BSSIDstr(i);
         jsonRed["rssi"] = WiFi.RSSI(i);
         // jsonRed["enc"] = NombreSeguridad[WiFi.encryptionType(i)];
         jsonRed["enc"] = encryptionTypeToString(WiFi.encryptionType(i));
         jsonRed["channel"] = WiFi.channel(i);
     }
     
      WiFi.scanDelete();

      if(WiFi.scanComplete() == -2){
         WiFi.scanNetworks(true);
      }
   }

   String jsonString;
   serializeJson(jsonWifi, jsonString);

   AsyncWebServerResponse *response = req->beginResponse(200, "application/json", jsonString);
   response->addHeader("Access-Control-Allow-Origin", "*");
   req->send(response);

}

// -------------------------------------------------------------------
// Petición para obtener el estado de conexion del servidor
// url: "/api/estadoServer"
// Método: GET
// -------------------------------------------------------------------
void handleApiGetStatusServer(AsyncWebServerRequest *req){

   interrumpirSocked = true;
   int httpCodeServidor = probarConexionServidor();
   String cuerpo;
   cuerpo+="{";
   cuerpo+="estado:"+String(httpCodeServidor > 0)+",";
   cuerpo+="codigo:"+String(httpCodeServidor);
   cuerpo+="}";

   delay(500);
   AsyncWebServerResponse *response = req->beginResponse(200,"application/json",cuerpo);
   response->addHeader("Access-Control-Allow-Origin", "*");
   req->send(response);

   interrumpirSocked = false;
}