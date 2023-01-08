//Cliente htpp para realizar solicituedes http al servidor
HTTPClient http;
WiFiClient cli;

//Documento json que contendra informacion para solicitudes htpp
StaticJsonDocument<48> json;
String jsonStr;

//Documento json que contendra informacion para ls clientes sockets conectados
StaticJsonDocument<100> jsonWS;
String jsonStrWS;


//Variables auxiliares para controlar cada cierto tiempo se
//limpiaran clietes o se enviara informacion a los clientes
//websockets
ulong_t tiempoInicialSockets = 0;
ulong_t tiempoLimpiaClientes = 0;


//==============================================================
//Metodo para liberar clientes websockets conectados  
static void comprobarClientes(long cada, AsyncWebSocket &socket)
{
   if (tiempoLimpiaClientes == 0)
   {
      tiempoLimpiaClientes = millis();
   }

   if (millis() - tiempoLimpiaClientes > cada)
   {

      socket.cleanupClients();
      tiempoLimpiaClientes = millis();
   }
}


//=============================================================
//Envia informacio sobre Wi-Fi a clientes websockets conectados
static void enviarInfoWS(AsyncWebSocket &socket)
{

   JsonObject ob = jsonWS.createNestedObject("WiFi");
   ob["CONECTADO"] = wifiConectado();
   ob["NOMBRE"] = WiFi.SSID();
   ob["RSSI"] = WiFi.RSSI();

   serializeJson(jsonWS, jsonStrWS);
   Serial.println(jsonStrWS);
   socket.textAll(jsonStrWS);

   jsonWS.clear();
   jsonStrWS.clear();
}


//=============================================================
//Envia informacio a clientes websockets cada clerto tiempo
void enviarInfoCada(long espera, AsyncWebSocket &socket)
{
   if(tiempoInicialSockets == 0){
      tiempoInicialSockets = millis();
   }

   if (millis() - tiempoInicialSockets > espera)
   {
      enviarInfoWS(socket);
      tiempoInicialSockets = millis();
   }
}

//=============================================================
//Realiza una peticion POST a nuestra api
//Utiliza la ruta de la Api configurada en archivos de configuracion
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
   http.addHeader("Authorization", strcat("Bearer ",token));
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

//=============================================================
//Metodo que procesa la informacion que es enviada de un cliente de
//desde websockets
void ProcessRequest(AsyncWebSocketClient *client, String mensaje)
{
}

//=============================================================
//metodo que contiene los eventos de los clientes websockets
void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
   if (type == WS_EVT_CONNECT)
   {
      Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
      // client->printf("Hello Client %u :)", client->id());
      Serial.printf("Clientes conectados [%u]\n",server->getClients().length());
      client->ping();
      
   }
   else if (type == WS_EVT_DISCONNECT)
   {

      //Serial.printf("ws[%s][%u] disconnect: %u\n", server->url(), client->id());
   }
   else if (type == WS_EVT_ERROR)
   {
      Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t *)arg), (char *)data);
   }
   else if (type == WS_EVT_PONG)
   {
      Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len) ? (char *)data : "");
   }
   else if (type == WS_EVT_DATA)
   {
      AwsFrameInfo *info = (AwsFrameInfo *)arg;

      String msg = "";
      if (info->final && info->index == 0 && info->len == len)
      {

         if (info->opcode == AwsFrameType::WS_TEXT)
         {
            for (size_t i = 0; i < info->len; i++)
            {
               msg += (char)data[i];
            }
         }
         else
         {
            char buff[3];
            for (size_t i = 0; i < info->len; i++)
            {
               sprintf(buff, "%02x ", (uint8_t)data[i]);
               msg += buff;
            }
         }
         if (info->opcode == WS_TEXT)
         {
         }
      }
      else
      {
         // message is comprised of multiple frames or the frame is split into multiple packets
         if (info->opcode == WS_TEXT)
         {
            for (size_t i = 0; i < len; i++)
            {
               msg += (char)data[i];
            }
         }
         else
         {
            char buff[3];
            for (size_t i = 0; i < len; i++)
            {
               sprintf(buff, "%02x ", (uint8_t)data[i]);
               msg += buff;
            }
         }
         Serial.printf("%s\n", msg.c_str());
         if ((info->index + len) == info->len)
         {
            if (info->final)
            {
               if (info->message_opcode == WS_TEXT)
                  ProcessRequest(client, msg);
            }
         }
      }
   }
}


//=============================================================
//Indica en que parte de la memoria se encuentran los archivos
//estaticos para el servidor
void filesStatic(AsyncWebServer &async)
{
   async.serveStatic("/", LittleFS, "/www/");
}
//=============================================================
//Ruta Login de nuestro servidor
//Metodo de acceso GET
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
//Ruta Login de nuestro servidor
//Metodo de acceso POST
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
   }
   else
   {
      req->redirect("/");
   }
}
//=============================================================
//Ruta principal de nuestro servidor
//Metodo de acceso GET
void handlePrincipalGet(AsyncWebServerRequest *req)
{
   if (!LittleFS.exists("/principal.html"))
   {
      req->send(404, "text/plain", "No encontrado");
      return;
   }

   File f = LittleFS.open("/principal.html", "r");
   String doc = f.readString();
   doc.replace("#nombre#", ssid);
   doc.replace("#contra#", password);
   doc.replace("#servidor#", serverIp);
   doc.replace("#api#", rutaApi);

   req->send(200, "text/html", doc);
}

//=============================================================
//Ruta principal de nuestro servidor
//Metodo de acceso POST
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
//=============================================================
//Agrega las rutas a nuestro servidor embebido

void addRouters(AsyncWebServer &asyncServer)
{
   filesStatic(asyncServer);
   asyncServer.on("/", HTTP_GET, handleLoginGet);
   asyncServer.on("/", HTTP_POST, handleLoginPost);
   asyncServer.on("/home", HTTP_GET, handlePrincipalGet);
   asyncServer.on("/home", HTTP_POST, handlePrincipalPost);
   asyncServer.onNotFound([](AsyncWebServerRequest *req)
                          { req->send(LittleFS, "/noEncontrado.html", "text/html"); });
}