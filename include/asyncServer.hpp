const char *usario = "rfid";
const char *contra = "1234";
// const uint8_t fingerprint[20] = { 0xC0,0x3F,0x3A,0x29,0xC1,0xDC,0x8C,0x4B,0x82,0x1D,0xE3,0x55,0xDB,0x43,0xE0,0xFF,0xFD,0x73,0xC3,0xE7 };
HTTPClient http;
WiFiClient cli;
StaticJsonDocument<48> json;
String jsonStr;

void enviarPostApi(String& uuid)
{
   if (WiFi.status() != WL_CONNECTED)
   {
      Serial.println("Wifi no conectado");
      return;
   }
   json["uuid"] = uuid;
   serializeJson(json, jsonStr);
   http.begin(cli, (serverIp + rutaApi));
   http.addHeader("Authorization", "Bearer api123");
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
void ProcessRequest(AsyncWebSocketClient *client, String mensaje)
{

}
void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
   if (type == WS_EVT_CONNECT)
   {
      // Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
      client->printf("Hello Client %u :)", client->id());
      client->ping();
   }
   else if (type == WS_EVT_DISCONNECT)
   {
      // Serial.printf("ws[%s][%u] disconnect: %u\n", server->url(), client->id());
   }
   else if (type == WS_EVT_ERROR)
   {
      // Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
   }
   else if (type == WS_EVT_PONG)
   {
      // Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len)?(char*)data:"");
   }
   else if (type == WS_EVT_DATA)
   {
      AwsFrameInfo *info = (AwsFrameInfo *)arg;
      String msg = "";
      if (info->final && info->index == 0 && info->len == len)
      {
         if (info->opcode == WS_TEXT)
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

void filesStatic(AsyncWebServer &async)
{
   
   async.serveStatic("/", LittleFS, "/www/");
}
void handleLoginGet(AsyncWebServerRequest *req)
{
   if (!LittleFS.exists("/login.html"))
   {
      req->send(404, "text/plain", "No encontrado");
      return;
   }

   req->send(LittleFS, "/login.html", "text/html");
}
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
void handlePrincipalGet(AsyncWebServerRequest *req)
{
   if (!LittleFS.exists("/principal.html"))
   {
      req->send(404, "text/plain", "No encontrado");
      return;
   }

   File f = LittleFS.open("/principal.html", "r");
   String doc = f.readString();
   // #nombre#
   // #contra#
   // #servidor#
   // #api#
   doc.replace("#nombre#", ssid);
   doc.replace("#contra#", password);
   doc.replace("#servidor#", serverIp);
   doc.replace("#api#", rutaApi);

   req->send(200, "text/html", doc);
}
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