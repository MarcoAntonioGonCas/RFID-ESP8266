//============================================================
//Guarda la confguracion en la memoria del ESP
//En un archivo JSON para leerlo facilmente
void guardarConfigjson(){
    String strJson;
    StaticJsonDocument<1000> json;
    json["ssid"] =  ssid;
    json["password"] = password;
    json["ServerIp"] = serverIp;
    json["rutaApi"] = rutaApi;
    serializeJson(json,strJson);

    File f = LittleFS.open("/config.json","w");

    f.print(strJson);
    f.close();
}


//=============================================================
//Lee la configuracion desde la memoria del ESP
//Esta es deserealizada para su facil lectura
void cargarConfi(){
    if(!LittleFS.exists("/config.json")){
        return;
    }
    File configFile = LittleFS.open("/config.json","r");
    StaticJsonDocument<1000> json;
    deserializeJson(json,configFile);
    configFile.close();

    ssid = json["ssid"].as<String>();
    password = json["password"].as<String>();
    serverIp = json["ServerIp"].as<String>();
    rutaApi = json["rutaApi"].as<String>();
}