// Documento json que contendra informacion para solicitudes htpp
StaticJsonDocument<48> json;
String jsonStr;

// Documento json que contendra informacion para ls clientes sockets conectados
StaticJsonDocument<100> jsonWS;
String jsonStrWS;

//=============================================================
// Realiza una peticion POST a nuestra api
// Utiliza la ruta de la Api configurada en archivos de configuracion
void enviarPostApi(String &uuid)
{
   

   if (WiFi.status() != WL_CONNECTED)
   {
      Serial.println("Wifi no conectado");
      return;
   }
   
   json["uuid"] = uuid;

   serializeJson(json, jsonStr);
   http.begin(cli, (serverIp + rutaApi));
   http.addHeader("Authorization", "Bearer " + String(token));
   http.addHeader("Content-Type", "application/json");
   http.addHeader("Connection", "keep-alive");

   int code = http.POST(jsonStr.c_str());

   if (code == 200)
   {
      Serial.println(F("Peticion realizada con exito"));
   }
   else
   {
      Serial.println(F("Erro al realizar la peticion"));
      Serial.println(http.errorToString(code));
   }

   http.end();
   cli.stop();
   cli.flush();
   json.clear();
   jsonStr.clear();
}
