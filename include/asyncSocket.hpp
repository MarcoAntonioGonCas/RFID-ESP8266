
// Variables auxiliares para controlar cada cierto tiempo se
// limpiaran clietes o se enviara informacion a los clientes
// websockets
ulong_t tiempoInicialSockets = 0;
ulong_t tiempoLimpiaClientes = 0;

//==============================================================
// Metodo para liberar clientes websockets conectados
static void comprobarClientes(long comprobarCada, AsyncWebSocket &socket)
{
   if (tiempoLimpiaClientes == 0)
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
void enviarInfoWS(AsyncWebSocket &socket)
{

   if (socket.count() == 0)
      return;
   JsonObject ob = jsonWS.createNestedObject("WiFi");
   ob["CONECTADO"] = wifiConectado();
   ob["NOMBRE"] = WiFi.SSID();
   ob["RSSI"] = WiFi.RSSI();

   serializeJson(jsonWS, jsonStrWS);
   socket.textAll(jsonStrWS);
   jsonWS.clear();
   jsonStrWS.clear();
   Serial.println(F("Enviando"));
}

//=============================================================
// Envia informacio a clientes websockets cada clerto tiempo
void enviarInfoCada(long espera, AsyncWebSocket &socket)
{
   if (tiempoInicialSockets == 0)
   {
      tiempoInicialSockets = millis();
   }

   if (millis() - tiempoInicialSockets > espera)
   {
      enviarInfoWS(socket);
      tiempoInicialSockets = millis();
   }
}

//=============================================================
// Metodo que procesa la informacion que es enviada de un cliente de
// desde websockets
void ProcessRequest(AsyncWebSocketClient *client, String mensaje)
{
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
      Serial.printf("ws[%s][%u] desconcectadd: %u\n", server->url(), client->id());
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
