// Documento json que contendra informacion para ls clientes sockets conectados
StaticJsonDocument<300> jsonWS;
String jsonStrWS;

// Variables auxiliares para controlar cada cierto tiempo se
// limpiaran clietes o se enviara informacion a los clientes
// websockets
ulong_t tiempoInicialSockets = 0;
ulong_t tiempoLimpiaClientes = 0;
//==============================================================
// Metodo para liberar clientes websockets conectados
static void comprobarClientesWs(ulong_t comprobarCada, AsyncWebSocket &socket)
{
   if (tiempoLimpiaClientes <= 0)
   {
      tiempoLimpiaClientes = millis();
   }

   if (millis() - tiempoLimpiaClientes > comprobarCada)
   {

      socket.cleanupClients();
      tiempoLimpiaClientes = millis();
   }
}

//=============================================================
// Envia informacio sobre Wi-Fi a clientes websockets conectados
void enviarInfoWs(AsyncWebSocket &socket)
{

   if (socket.count() == 0)
      return;
   JsonObject ob = jsonWS.createNestedObject("WiFi");
   ob["CONECTADO"] = wifiConectado();
   ob["NOMBRE"] = WiFi.SSID();
   ob["RSSI"] = WiFi.RSSI();

   JsonObject infoESP = jsonWS.createNestedObject("ESP");
   FSInfo info;
   LittleFS.info(info);

   infoESP["RAM"] = ESP.getFreeHeap();
   infoESP["RAMT"] = stackInicial;
   infoESP["RAMUSE"] = stackInicial - ESP.getFreeHeap();
   infoESP["SPIFFS"] = info.totalBytes - info.usedBytes;
   infoESP["TIEMPO"] = millis();
   infoESP["FREC"] = ESP.getCpuFreqMHz();

   JsonObject infoAP = jsonWS.createNestedObject("AP");

   infoAP["HABI"] = apHabilitado;
   infoAP["CLIEN"] = !apHabilitado?0:WiFi.softAPgetStationNum();

   serializeJson(jsonWS, jsonStrWS);
   // serializeJsonPretty(jsonWS,Serial);

   socket.textAll(jsonStrWS);

   jsonWS.clear();
   jsonStrWS.clear();
   Serial.println(F("Enviando"));
}

//=============================================================
// Envia informacio a clientes websockets cada clerto tiempo
void enviarInfoWs(ulong_t espera, AsyncWebSocket &socket)
{
   if (tiempoInicialSockets <= 0)
   {
      tiempoInicialSockets = millis();
   }

   if (millis() - tiempoInicialSockets > espera)
   {
      enviarInfoWs(socket);
      tiempoInicialSockets = millis();
   }
}

//=============================================================
// Metodo que procesa la informacion que es enviada de un cliente de
// desde websockets
void ProcessRequest(AsyncWebSocketClient *client, String mensaje)
{
   
   Serial.println("Mensaje recibido de cliente websocket: " + mensaje);

   if (mensaje == "ESTADO")
   {
      enviarEstado = true;
   }
}
void enviaEatadoServidorSocket(){
      JsonObject ob = jsonWS.createNestedObject("ESTADOSERVER");
      int res = probarConexionServidor();
      ob["ESTADO"] = res > 0;
      ob["CODIGO"] = res;
      serializeJson(jsonWS,jsonStrWS);
      asyncSocket.textAll(jsonStrWS);
      jsonStr.clear();
      jsonStrWS.clear();
}
//=============================================================
// metodo que contiene los eventos de los clientes websockets
void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
   if (type == WS_EVT_CONNECT)
   {
      Serial.printf("ws[%s][%u] conectado\n", server->url(), client->id());
      Serial.printf("Clientes conectados [%u]\n", server->getClients().length());
      client->ping();
   }
   else if (type == WS_EVT_DISCONNECT)
   {
      
      Serial.printf("ws[%s][%u] desconcectadd: %u \n", server->url(), client->id(), client->id());
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

      //TODO: Realizar metodo para procesar mensaje;
      Serial.println("Estoy recibiendo datos");
      AwsFrameInfo *info = (AwsFrameInfo *)arg;

      Serial.println(String(info->final));
      Serial.println(String(info->index));
      Serial.println(String(info->len));
      Serial.println(String(len));
      Serial.println(String(info->message_opcode));

      String msg = "";


      if(info->opcode  == WS_TEXT){
         Serial.println("Estoy recibiendo texto");
      }else if(info->opcode  == WS_BINARY){
         Serial.println("Estoy recibiendo binarios");
      }

      if(info->index == 0 and info->opcode  == AwsFrameType::WS_TEXT){
         
         for(size_t i = 0; i < len;i++){
            msg += (char)data[i]; 
         }

      }
      ProcessRequest(client,msg);
      return;
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


void loopSocket(){
 if(interrumpirSocked) return;
  enviarInfoWs(2000, asyncSocket);
  comprobarClientesWs(1000, asyncSocket);
}