// -------------------------------------------------------------------
// Obtiene la pagina restablecer
// url: "/confiRestore"
// Método: GET
// -------------------------------------------------------------------
void handleConfiRestoretGet(AsyncWebServerRequest *req)
{
   if (!LittleFS.exists("/confiRestore.html"))
   {
      req->send(404, "text/plain", "No encontrado");
      return;
   }

   req->send(LittleFS, "/confiRestore.html", "text/html");
}


// -------------------------------------------------------------------
// Obtiene la pagina restablecer
// url: "/restoreAll"
// Método: GET
// -------------------------------------------------------------------
void handleRestoreAllGet(AsyncWebServerRequest *req)
{
   borrarConfig();
   restart = true;
   enviarHtmlCarga(req,6000,"/");
}

// -------------------------------------------------------------------
// Obtiene la pagina reiniciar
// url: "/restart"
// Método: GET
// -------------------------------------------------------------------
void handleRestartGet(AsyncWebServerRequest *req){

   restart = true;
   enviarHtmlCarga(req,6000,"/");
}



// -------------------------------------------------------------------
// Lista los archivos que estan en el ESP por el pueto serial
// -------------------------------------------------------------------
void listDirSerial(String dirName,uint16 nivel = 0){
   Dir dir = LittleFS.openDir(dirName);


   while (dir.next())
   {
      String nombre = dir.fileName();

      for(uint16 i = 0; i < nivel;i++){
         Serial.print(F("\t"));
      }
      if(dir.isDirectory()){

         Serial.println("Directorio: "+ nombre);

         listDirSerial(dirName + "/" + nombre, nivel + 1);
      }else{
         Serial.println("Archivo " + nombre);
      }
   }
  
}


// -------------------------------------------------------------------
// Lista los archivos que estan en el ESP en un JSON
// -------------------------------------------------------------------
void listFiles(DynamicJsonDocument& jsonDoc, const String& dirname) {
  Dir dir = LittleFS.openDir(dirname);

  while (dir.next()) {
    String fileName = dir.fileName();

    if (dir.isDirectory()) {
      DynamicJsonDocument subDirDoc(1024);
      JsonObject subDir = subDirDoc.to<JsonObject>();
      subDir["name"] = fileName;
      subDir["type"] = "directory";
      JsonArray files = subDir.createNestedArray("files");
      listFiles(subDirDoc, dirname + "/" + fileName);
      jsonDoc.add(subDir);
    } else {
      JsonObject file = jsonDoc.createNestedObject();
      file["name"] = fileName;
      file["type"] = "file";
    }
  }
}

// -------------------------------------------------------------------
// Lista los archivos que contiene el esp32
// url: "/listDir"
// Método: GET
// -------------------------------------------------------------------
void handleLisDirGet(AsyncWebServerRequest *req){
   DynamicJsonDocument jsonDirec (2000);
   String cadenaDirec;

   listFiles(jsonDirec,"/");
   listDirSerial("/");
   serializeJson(jsonDirec,cadenaDirec);
   req->send(200,"application/json",cadenaDirec);
}

