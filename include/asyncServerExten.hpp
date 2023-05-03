// -------------------------------------------------------------------
// Obtiene la pagina restablecer
// url: "/confiReset"
// Método: GET
// -------------------------------------------------------------------
void handleConfiResetGet(AsyncWebServerRequest *req)
{
   if (!LittleFS.exists("/confiReset.html"))
   {
      req->send(404, "text/plain", "No encontrado");
      return;
   }

   req->send(LittleFS, "/confiReset.html", "text/html");
}

// -------------------------------------------------------------------
// Obtiene la pagina restablecer
// url: "/resetAll"
// Método: GET
// -------------------------------------------------------------------
void handleResetAllGet(AsyncWebServerRequest *req)
{
   borrarConfig();
   restart = true;
   enviarHtmlCarga(req,6000,"/");
}

