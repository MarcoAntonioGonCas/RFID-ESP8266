
//=============================================================
// Indica en que parte de la memoria se encuentran los archivos
// estaticos para el servidor
void filesStatic(AsyncWebServer &async)
{
   async.serveStatic("/www", LittleFS, "/www/");
}
//=============================================================
// Ruta Login de nuestro servidor
// Metodo de acceso GET
void handleLoginGet(AsyncWebServerRequest *req)
{
   if (!LittleFS.exists("/login.html"))
   {
      req->send(404, "text/plain", "No encontrado");
      return;
   }

   req->send(LittleFS, "/login.html", "text/html");
}
//=============================================================
// Ruta Login de nuestro servidor
// Metodo de acceso POST
void handleLoginPost(AsyncWebServerRequest *req)
{

   int numParametros = req->params();

   if (numParametros == 2)
   {
      String pUsuario = req->getParam(0)->value();
      String pContra = req->getParam(1)->value();
      if (strcmp(pUsuario.c_str(), usario) == 0 &&
          strcmp(pContra.c_str(), contra) == 0)
      {
         req->redirect("/home");
      }
      Serial.println((strcmp(pUsuario.c_str(), usario) == 0 &&
                      strcmp(pContra.c_str(), contra) == 0));
   }
   else
   {
      req->redirect("/");
   }
}
//=============================================================
// Ruta principal de nuestro servidor
// Metodo de acceso GET
void handlePrincipalGet(AsyncWebServerRequest *req)
{
   if (!LittleFS.exists("/principal.html"))
   {
      req->send(404, "text/plain", "No encontrado");
      return;
   }

   File f = LittleFS.open("/principal.html", "r");
   String doc = f.readString();
   doc.replace("#rssi#",String(WiFi.RSSI()));
   doc.replace("#nombre#", ssid);
   doc.replace("#contra#", password);
   doc.replace("#servidor#", serverIp);
   doc.replace("#api#", rutaApi);

   req->send(200, "text/html", doc);
}

//=============================================================
// Ruta principal de nuestro servidor
// Metodo de acceso POST
void handlePrincipalPost(AsyncWebServerRequest *req)
{
   int numParametros = req->params();
   // nombre
   //  contra
   //  servidor
   //  api
   if (numParametros == 4)
   {
      String pNombre = req->getParam(0)->value();
      String pContra = req->getParam(1)->value();
      String pServidor = req->getParam(2)->value();
      String pApi = req->getParam(3)->value();

      ssid = pNombre;
      password = pContra;
      serverIp = pServidor;
      rutaApi = pApi;

      guardarConfigjson();
      conectarWiFi();
      req->redirect("/home");
   }
   else
   {
      req->redirect("/home");
   }
}
String processorWifiConfigGet(const String& var)
{
  if(var == "nombre"){
   return ssid;
  }else if(var == "contra"){
   return password;
  }else if(var == "HabiProxy"){
   return proxyHabilitado ? "checked":"";
  }else if(var == "proxy"){
   return proxy->toString();
  }else if(var == "puerto"){
   return String(puerto);
  }else if(var == "nombreAP"){
   return ssidAP;
  }else if(var == "contraAP"){
   return passwordAP;
  }else if(var == "APHabilitado"){
   return apHabilitado ? "checked":"";
  }else if(var == "rssi"){
   return String(WiFi.RSSI());
  }


  return var;
}
void handleWifiConfigGet(AsyncWebServerRequest *req){
   if (!LittleFS.exists("/conWifi.html"))
   {
      req->send(404, "text/plain", "No encontrado");
      return;
   }

   File f = LittleFS.open("/conWifi.html", "r");
   String doc = f.readString();
   doc.replace("%nombre%", ssid);
   doc.replace("%contra%", password);
   doc.replace("%HabiProxy%", proxyHabilitado ? "checked":"");
   doc.replace("%proxy%", proxy !=nullptr? proxy->toString():"");
   doc.replace("%puerto%", String(puerto));
   doc.replace("%nombreAP%", ssidAP);
   doc.replace("%contraAP%", passwordAP);
   doc.replace("%APHabilitado%", apHabilitado ? "checked":"");
   doc.replace("%rssi%", String(WiFi.RSSI()));


   req->send(200,"text/html",doc);
}
void handleWifiConfigPost(AsyncWebServerRequest *req)
{
   int numParametros = req->params();

   //nombreRed
   //contra
   //habilitarProxy
   //proxy
   //purto
   //nombreAP
   //contraAP
   //puntoAPHabilitado


   if (numParametros == 8)
   {
      String pNombre = req->getParam(0)->value();
      String pContra = req->getParam(1)->value();
      String pHabiProxy = req->getParam(3)->value();
      String pProxy = req->getParam(4)->value();





      req->redirect("/confiWifi");
   }
   else
   {
      req->redirect("/confiWifi");
   }
}
//=============================================================
// Agrega las rutas a nuestro servidor embebido

//=============================================================
// Envia un json con las conexiones wifi actuales
const String NombreSeguridad [] = {"ABIERTA","WEP","WPA_PSK","WPA2_PSK","WPA_WPA2_PSK","AUTH_MAX"};


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

      jsonWifi["count"]= redesDispo;
      for (int i = 0; i < redesDispo; ++i)
      {
         JsonObject jsonRed = jsonRedes.createNestedObject();
         jsonRed["ssid"] = WiFi.SSID(i);
         jsonRed["rssi"] = WiFi.RSSI(i);
         jsonRed["enc"] = NombreSeguridad[WiFi.encryptionType(i)];
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

void addRouters(AsyncWebServer &asyncServer)
{
   filesStatic(asyncServer);
   
   asyncServer.on("/", HTTP_GET, handleLoginGet);
   asyncServer.on("/", HTTP_POST, handleLoginPost);

   asyncServer.on("/home", HTTP_GET, handlePrincipalGet);
   asyncServer.on("/home", HTTP_POST, handlePrincipalPost);

   asyncServer.on("/confiWifi",HTTP_GET,handleWifiConfigGet);
   asyncServer.on("/confiWifi",HTTP_POST,handleWifiConfigPost);

   asyncServer.on("/api/redes", HTTP_GET, handleApiGetNetworks);
   asyncServer.onNotFound([](AsyncWebServerRequest *req)
                          { req->send(LittleFS, "/noEncontrado.html", "text/html"); });
}