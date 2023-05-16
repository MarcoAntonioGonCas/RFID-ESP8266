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
// Procesador de la plantilla, del arcivo /confi.html
// -------------------------------------------------------------------
String processorConfigGet(const String& var)
{
  if(var == "rssi"){
   return String(WiFi.RSSI());
  }else if(var == "servidor"){
   return serverIp;
  }else if(var == "api"){
   return rutaApi;
  }else if(var == "api-registro"){
   return rutaApiRegistro;
  }else if(var == "codigoInter"){
   return codigoIntercambio;
   }else if(var == "autorizacion"){
   return token;
  }else if(var == "certificado"){
   return certificadoHttps;
  }else if(var == "registro"){
   return modoRegistro ? "checked":"";
  }
  return String();
}
// -------------------------------------------------------------------
// Petición de la pagina principal de configuracion
// url: "/confi"
// Método: GET
// -------------------------------------------------------------------
void handleConfiGet(AsyncWebServerRequest *req)
{
   if (!LittleFS.exists("/confi.html"))
   {
      req->send(404, "text/plain", "No encontrado");
      return;
   }

   File f = LittleFS.open("/confi.html", "r");
   // String doc = f.readString();


   // doc.replace("%rssi%",String(WiFi.RSSI()));
   // doc.replace("%servidor%", serverIp);
   // doc.replace("%api%", rutaApi);
   // doc.replace("api-registro",rutaApiRegistro);
   // doc.replace("%autorizacion%",token);
   // doc.replace("%certificado%",certificadoHttps);
   // doc.replace("%registro%",modoRegistro ? "checked":"");

   req->send(LittleFS,"/confi.html", "text/html",false, processorConfigGet);
}

// -------------------------------------------------------------------
// Petición para guardar los cambios de la pagina principal
// url: "/confi"
// Método: POST
// -------------------------------------------------------------------
void handleConfiPost(AsyncWebServerRequest *req)
{

   //Varibles auxiliares
   String pServidor = "";
   String pRuta = "";
   String pRutaRegistro = "";
   String pAutorizacion = "";
   String pCertificado = "";
   String pCodigoInter = "";

   bool nuevaConfiguracion = false;
   bool pModoRegistro = false;
   bool nuevaAutorizacion = false;

   nuevaConfiguracion = obtenerValorYComparar(req,"servidor",pServidor,serverIp);
   nuevaConfiguracion = obtenerValorYComparar(req,"api",pRuta,rutaApi) ? true : nuevaConfiguracion;
   nuevaConfiguracion = obtenerValorYComparar(req,"apiRegistro",pRutaRegistro,rutaApiRegistro) ? true : nuevaConfiguracion;
   nuevaConfiguracion = obtenerValorYComparar(req,"codigoInter",pCodigoInter,codigoIntercambio) ? true : nuevaConfiguracion;
   nuevaConfiguracion = obtenerValorYComparar(req,"certificado",pCertificado,certificadoHttps,true) ? true : nuevaConfiguracion;

   nuevaAutorizacion = obtenerValorYComparar(req,"autorizacion",pAutorizacion,token);
   pModoRegistro = req->hasParam("registro",true);

   if(pModoRegistro != modoRegistro){
      modoRegistro = pModoRegistro;
      nuevaConfiguracion = true;
   }

   if(nuevaAutorizacion){
      nuevaConfiguracion = true;
      wifiClientConfigurado = false;
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


  return String();
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
   // String doc = f.readString();


   // doc.replace("%nombre%", ssid);
   // doc.replace("%contra%", password);
   // doc.replace("%HabiProxy%", proxyHabilitado ? "checked":"");
   // doc.replace("%proxy%", proxy.isSet() ? proxy.toString():"");
   // doc.replace("%puerto%", String(puerto));
   // doc.replace("%nombreAP%", ssidAP);
   // doc.replace("%contraAP%", passwordAP);
   // doc.replace("%APHabilitado%", apHabilitado ? "checked":"");
   // doc.replace("%rssi%", String(WiFi.RSSI()));

   //*************************************************************************   
   // AsyncWebServerResponse *response = req->beginResponse(200,"text/html",doc);
  
   AsyncWebServerResponse *response = req->beginResponse(LittleFS,"/confiWifi.html","text/html",false, processorWifiConfigGet);
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
   bool cambio  = false;
   String pNombre = "";
   String pContra = "";

   obtenerValorDePeticion(req,"nombre",pNombre,true);
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
      password = pContra;
      cambio = true;
   }

   return cambio;
}


// -------------------------------------------------------------------
// Metodo que verifica la solcitud post el cual devuelve true si la
// configuracion a cambiado solo nombre y contraseña e punto de acceso
// -------------------------------------------------------------------
bool postApAux(AsyncWebServerRequest *req){
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

   if(pContraAP.compareTo(passwordAP) != 0){
      Serial.println("Longitud de la nueva contraseña");
      Serial.println(pContraAP.length());
      if(pContraAP.isEmpty() || pContraAP.length() >= 8){
         passwordAP = pContraAP;
         cambio = true;
      }
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
   change = postWiFiAux(req) | postApAux(req);

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

   asyncServer.on("logout",HTTP_GET,[](AsyncWebServerRequest*req){

   });


   asyncServer.on("/confi", HTTP_GET, handleConfiGet);
   asyncServer.on("/confi", HTTP_POST, handleConfiPost);

   asyncServer.on("/confiWifi",HTTP_GET,handleWifiConfigGet);
   asyncServer.on("/confiWifi",HTTP_POST,handleWifiConfigPost);


   //confiUser
   asyncServer.on("/confiUser",HTTP_GET,handleConfiUserGet);
   asyncServer.on("/confiUser",HTTP_POST,handleConfiUserPost);

   //confiReset
   asyncServer.on("/confiRestore",HTTP_GET,handleConfiRestoretGet);
   asyncServer.on("/restoreAll",HTTP_GET,handleRestoreAllGet);
   asyncServer.on("/restart",HTTP_GET,handleRestartGet);

   asyncServer.on("/api/redes", HTTP_GET, handleApiGetNetworks);
   asyncServer.on("/api/estadoServer", HTTP_GET, handleApiGetStatusServer);

   asyncServer.on("/listDir",HTTP_GET,handleLisDirGet);

   asyncServer.onNotFound([](AsyncWebServerRequest *req)
                         { req->send(200, "text/html", "No encontrado"); });
}