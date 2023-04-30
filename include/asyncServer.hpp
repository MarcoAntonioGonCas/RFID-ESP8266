void obtenerValorDePeticion(AsyncWebServerRequest *req,const char * nombreParametroPeticion, String &valorNuevaConfiguracion, bool post=true){
   valorNuevaConfiguracion = "";

   if( req->hasParam(nombreParametroPeticion, post)){
      valorNuevaConfiguracion = req->getParam(nombreParametroPeticion, post)->value();
      valorNuevaConfiguracion.trim();
   }

}
bool obtenerValorYComparar(AsyncWebServerRequest *req,const char * nombreParam, String &valorNuevaConfiguracion,String &valorConfiguracion, bool vacio = false, bool post =true){
   valorNuevaConfiguracion = "";
   obtenerValorDePeticion(req,nombreParam,valorNuevaConfiguracion,post);
   //Comparamos que los valores no sean iguales o si esta vacio 
   //Dependiendo del parametro

   if(vacio){
         if( valorNuevaConfiguracion.compareTo(valorConfiguracion) != 0){
            valorConfiguracion = valorNuevaConfiguracion;
            return true;
         }else{
            return false;
         }
   }else{
      if( valorNuevaConfiguracion.compareTo(valorConfiguracion) != 0 and 
         !valorNuevaConfiguracion.isEmpty()){
            valorConfiguracion = valorNuevaConfiguracion;
            return true;
         }else{
            return false;
         }
   }
   
   
}
void enviarHtmlCarga(AsyncWebServerRequest *req,int tiempo,const char* url){
   if(!LittleFS.exists("/carga.html")){
      req->send(200,"text/plain","No sa encontro la pagina");
      return;
   }
   File f = LittleFS.open("/carga.html", "r");
   String doc = f.readString();


   doc.replace("%time%",String(tiempo));
   doc.replace("%link%",url);


   req->send(200,"text/html",doc);
}
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
   doc.replace("%servidor%", serverIp);
   doc.replace("%api%", rutaApi);
   doc.replace("%autorizacion%",token);
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
   bool nuevaConfiguracion = false;

   //Varibles auxiliares
   String pServidor = "";
   String pRuta = "";
   String pAutorizacion = "";
   bool pModoRegistro = false;


   nuevaConfiguracion = 
   obtenerValorYComparar(req,"servidor",pServidor,serverIp) ||
   obtenerValorYComparar(req,"api",pRuta,rutaApi) ||
   obtenerValorYComparar(req,"autorizacion",pAutorizacion,token);


   pModoRegistro= req->hasParam("registro",true);

   if(pModoRegistro != modoRegistro){
      modoRegistro = pModoRegistro;
      nuevaConfiguracion = true;
   }

   if(nuevaConfiguracion){
      guardarConfigjson();
   }

   req->redirect("/confi");
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
   return proxy.isSet() ? proxy.toString() :"" ;
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
   doc.replace("%proxy%", proxy.isSet() ? proxy.toString():"");
   doc.replace("%puerto%", String(puerto));
   doc.replace("%nombreAP%", ssidAP);
   doc.replace("%contraAP%", passwordAP);
   doc.replace("%APHabilitado%", apHabilitado ? "checked":"");
   doc.replace("%rssi%", String(WiFi.RSSI()));


   //*************************************************************************
   
   AsyncWebServerResponse *response = req->beginResponse(200,"text/html",doc);
   response->addHeader("Access-Control-Allow-Origin", "*");
   req->send(response);
}

// -------------------------------------------------------------------
// Metodos para cada apartado de configiguracion desde post
// -------------------------------------------------------------------
bool postProxyAux(AsyncWebServerRequest *req){

   //#proxy
   //proxyHabi - opcional
   //proxy - requerido
   //puerto - requerido
   bool cambio = false;
   proxyHabilitado = req->hasParam("proxyHabi",true);

   if(req->hasParam("proxy",true)){
      String pProxy = req->getParam("proxy",true)->value();
      proxy = toIP(pProxy);
      cambio = true;
   }
 
   String pPuerto = "";
   obtenerValorDePeticion(req,"puerto",pPuerto);


   if(isNum(pPuerto)){
      puerto = pPuerto.toInt();
      cambio = true;
   } 

   return cambio;
}
// -------------------------------------------------------------------
// Metodo que verifica la solcitud post el cual devuelve true si la
// configuracion a cambiado solo nombre y contraseña e punto de acceso
// ------------------------------------------------------------------
bool postWiFiAux(AsyncWebServerRequest *req){
   //#wifi
   //nombre - requerido
   //contra - opcional
   bool cambio  = false;
   String pNombre = "";
   obtenerValorDePeticion(req,"nombre",pNombre,true);
   String pContra = "";
   obtenerValorDePeticion(req,"contra",pContra,true);

   Serial.println("POST WIFI");
   Serial.println(pNombre);
   Serial.println(pContra);

   // Verifica que existan nuevos cambios para aplicarlos y reiniciar el dipositivo
   if(  pNombre.compareTo(ssid) != 0 and pNombre.compareTo("") != 0){
      
      ssid = pNombre;
      cambio = true;
   }

   if(pContra.compareTo(password) != 0){
      if(pContra.isEmpty() || pContra.length() >= 8){
         password = pContra;
         cambio = true;
      }
   }

   return cambio;
}


// -------------------------------------------------------------------
// Metodo que verifica la solcitud post el cual devuelve true si la
// configuracion a cambiado solo nombre y contraseña e punto de acceso
// -------------------------------------------------------------------
bool postApAux(AsyncWebServerRequest *req){
   //#punto de acceso
   //APHabilitado - opcional
   //nombreAP - requerido
   //contraAP - opcional
   bool cambio = false;
   bool pApHabilitado = true;

   pApHabilitado = req->hasParam("APHabilitado",true);

   String pNombreAP = "";
   obtenerValorDePeticion(req,"nombreAP",pNombreAP,true);

   String pContraAP = "";
   obtenerValorDePeticion(req,"contraAP",pContraAP,true);

   Serial.println("POST AP");
   Serial.println(pNombreAP);
   Serial.println(pContraAP);

   //Si el nombre de punto de acceso es diferente del actual
   //O la contraseña es diferente
   //Se actualizara el nombre 
   if(  pNombreAP.compareTo(ssidAP) != 0 and 
        pNombreAP.compareTo("")!=0 ){
      ssidAP = pNombreAP; 
      cambio = true;
   }

   if(pContraAP.compareTo(passwordAP)!=0){
      passwordAP = pContraAP;
      cambio = true;
   }
   if(pApHabilitado != apHabilitado){
      apHabilitado = pApHabilitado;
      cambio = true;
   }
   return cambio;
}
// -------------------------------------------------------------------
// Petición para guardar cambios de configuracion del WIFI
// url: "/confiWifi"
// Método: POST
// -------------------------------------------------------------------
void handleWifiConfigPost(AsyncWebServerRequest *req)
{
   bool change = false;
   bool cambioProxy = false;

   cambioProxy = postProxyAux(req);
   change = postWiFiAux(req) || postApAux(req);

   Serial.println("POST API AP GUardao comopleto en memoria");

   
   //Configurar
   if(change){
      guardarConfigjson();
      enviarHtmlCarga(req,5000,"/confiWifi");
      restart = true;
   }else{

      if(cambioProxy){
         guardarConfigjson();
      }

      AsyncWebServerResponse *response = req->beginResponse(302);
      response->addHeader("Access-Control-Allow-Origin", "*");
      response->addHeader("Location","/confiWifi");
      req->send(response);
   }
}


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