// -------------------------------------------------------------------
// Petición para obtener la pagina de login
// url: "/"
// Método: GET
// -------------------------------------------------------------------
void handleLoginGet(AsyncWebServerRequest *req)
{
   if (!LittleFS.exists("/login.html"))
   {
      req->send(404, "text/plain", "No encontrado");
      return;
   }

   req->send(LittleFS, "/login.html", "text/html");
}




// -------------------------------------------------------------------
// Petición para comprobar si el usuario existe y contraseña
// url: "/"
// Método: POST
// -------------------------------------------------------------------
void handleLoginPost(AsyncWebServerRequest *req)
{

   int numParametros = req->params();

   if (numParametros == 2)
   {
      String pUsuario = req->getParam(0)->value();
      String pContra = req->getParam(1)->value();
      if (strcmp(pUsuario.c_str(), usuarioLogin.c_str()) == 0 &&
          strcmp(pContra.c_str(), contraLogin.c_str()) == 0)
      {
         req->redirect("/home");
      }else{
         req->redirect("/");
      }
      
   }
   else
   {
      req->redirect("/");
   }
}

// -------------------------------------------------------------------
// Petición para obtener la pagina principal
// url: "/home"
// Método: GET
// -------------------------------------------------------------------
void handlehomeGet(AsyncWebServerRequest *req){
   if (!LittleFS.exists("/home.html"))
   {
      req->redirect("/notFound");
      // req->send(404, "text/plain", "No encontrado");
      return;
   }

   File f = LittleFS.open("/home.html", "r");
   String doc = f.readString();
   doc.replace("%rssi%", String(WiFi.RSSI()));
   req->send(200,"text/html",doc);

}

// -------------------------------------------------------------------
// Petición obtener la configuracion de inicio de sesion
// url: "/confiUser"
// Método: GET
// -------------------------------------------------------------------
void handleConfiUserGet(AsyncWebServerRequest *req){
   //TODO:Cambiar nombre de archivo depues a confiUser.html
   if (!LittleFS.exists("/confiUser.html"))
   {
      req->redirect("/notFound");
      // req->send(404, "text/plain", "No encontrado");
      return;
   }

   File f = LittleFS.open("/confiUser.html", "r");

   String doc = f.readString();
   doc.replace("%usuario%", usuarioLogin);
   doc.replace("%contra%", contraLogin);
   doc.replace("%rssi%", String(WiFi.RSSI()));


   req->send(200,"text/html",doc);

}
// -------------------------------------------------------------------
// Petición guardar la configuracion de inicio de sesion
// url: "/confiUser"
// Método: POST
// -------------------------------------------------------------------
void handleConfiUserPost(AsyncWebServerRequest *req){

   if(req->hasParam("contra",true) and req->hasParam("usuario",true)){
      
      String pContra = req->getParam("contra",true)->value();
      String pUsuario = req->getParam("usuario",true)->value();
      pContra.trim();
      pUsuario.trim();

      if(!pUsuario.isEmpty() and !pContra.isEmpty() ){
         usuarioLogin = pUsuario;
         contraLogin = pContra;
         guardarConfigUserJson();
      }

      req->redirect("/confiUser");
      
   }else{
      req->redirect("/confiUser");
   }

}

// -------------------------------------------------------------------
// Petición de la pagina principal de configuracion
// url: "/confi"
// Método: POST
// -------------------------------------------------------------------
void handleConfiGet(AsyncWebServerRequest *req)
{
   if (!LittleFS.exists("/confi.html"))
   {
      req->send(404, "text/plain", "No encontrado");
      return;
   }

   File f = LittleFS.open("/confi.html", "r");
   String doc = f.readString();
   doc.replace("%rssi%",String(WiFi.RSSI()));
   doc.replace("%nombre%", ssid);
   doc.replace("%contra%", password);
   doc.replace("%servidor%", serverIp);
   doc.replace("%api%", rutaApi);
   doc.replace("%rssi%", String(WiFi.RSSI()));
   doc.replace("%registro%",modoRegistro ? "checked":"");

   req->send(200, "text/html", doc);
}

// -------------------------------------------------------------------
// Petición para guardar los cambios de la pagina principal
// url: "/confi"
// Método: POST
// -------------------------------------------------------------------
void handleConfiPost(AsyncWebServerRequest *req)
{
   if (
      req->hasParam("nombre",true) and
      req->hasParam("servidor",true) and
      req->hasParam("api",true)
   )
   {
      String pNombre = req->getParam("nombre",true)->value();
      String pContra = "";
      String pServidor = req->getParam("servidor",true)->value();
      String pApi = req->getParam("api",true)->value();

      if(req->hasParam("contra",true)){
         pContra = req->getParam("contra",true)->value();
      }   
      ssid = pNombre;
      password = pContra;
      serverIp = pServidor;
      rutaApi = pApi;
      modoRegistro = req->hasParam("registro",true);

      // guardarConfigjson();
      conectarWiFi();
      req->redirect("/home");
   }
   else
   {
      req->redirect("/home");
   }
}

// -------------------------------------------------------------------
// Procesador de la plantilla, del arcivo /conWifi.html
// -------------------------------------------------------------------
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



// -------------------------------------------------------------------
// Petición para obtener la pagina web para la configuracion de redes
// url: "/confiWifi"
// Método: GET
// -------------------------------------------------------------------
void handleWifiConfigGet(AsyncWebServerRequest *req){
   if (!LittleFS.exists("/confiWifi.html"))
   {
      req->send(404, "text/plain", "No encontrado");
      return;
   }

   File f = LittleFS.open("/confiWifi.html", "r");
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
// -------------------------------------------------------------------
// Petición para guardar cambios de configuracion del WIFI
// url: "/confiWifi"
// Método: POST
// -------------------------------------------------------------------
void handleWifiConfigPost(AsyncWebServerRequest *req)
{
   int numParametros = req->params();

   if (numParametros == 8)
   {
      
      String pNombre = req->getParam(0)->value();
      String pContra = req->getParam(1)->value();
      String pHabiProxy = req->getParam(2)->value();
      String pProxy = req->getParam(3)->value();
      String pPuerto = req->getParam(4)->value();
      String pNombreAP = req->getParam(5)->value();
      String pContraAP = req->getParam(6)->value();
      String pPuntoAPHabilitado = req->getParam(7)->value();

      Serial.println("Mostrando info");
      Serial.println(pNombre);
      Serial.println(pContra);
      Serial.println(pHabiProxy);
      Serial.println(pProxy);
      Serial.println(pPuerto);
      Serial.println(pNombreAP);
      Serial.println(pContraAP);
      Serial.println(pPuntoAPHabilitado);

      req->redirect("/confiWifi");
   }
   else
   {
      req->redirect("/confiWifi");
   }
}


//=============================================================
// Envia un json con las conexiones wifi actuales
const String NombreSeguridad [] = {"ABIERTA","WEP","WPA_PSK","WPA2_PSK","WPA_WPA2_PSK","AUTH_MAX"};

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

//-------------------------------------------------------------------
// Indica en que parte de la memoria se encuentran los archivos
// estaticos para el servidor
void filesStatic(AsyncWebServer &async)
{
   async.serveStatic("/www", LittleFS, "/www/");
}

//-------------------------------------------------------------------
// Agrega las rutas a nuestro servidor web embebido
//-------------------------------------------------------------------
void addRouters(AsyncWebServer &asyncServer)
{
   filesStatic(asyncServer);
   
   asyncServer.on("/", HTTP_GET, handleLoginGet);
   asyncServer.on("/", HTTP_POST, handleLoginPost);
   
   asyncServer.on("/home", HTTP_GET, handlehomeGet);


   asyncServer.on("/confi", HTTP_GET, handleConfiGet);
   asyncServer.on("/confi", HTTP_POST, handleConfiPost);

   asyncServer.on("/confiWifi",HTTP_GET,handleWifiConfigGet);
   asyncServer.on("/confiWifi",HTTP_POST,handleWifiConfigPost);


   //confiUser
   asyncServer.on("/confiUser",HTTP_GET,handleConfiUserGet);
   asyncServer.on("/confiUser",HTTP_POST,handleConfiUserPost);


   asyncServer.on("/api/redes", HTTP_GET, handleApiGetNetworks);

   asyncServer.onNotFound([](AsyncWebServerRequest *req)
                          { req->send(LittleFS, "/noEncontrado.html", "text/html"); });
}