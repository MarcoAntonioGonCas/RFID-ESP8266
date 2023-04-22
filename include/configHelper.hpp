//============================================================
// Guarda la confguracion en la memoria del ESP
// En un archivo JSON para leerlo facilmente
void guardarConfigjson()
{
    String strJson;
    StaticJsonDocument<1000> json;

    json["ssid"] = ssid;
    json["password"] = password;
    json["proxyHabilitado"] = proxyHabilitado;
    // json["proxy"] = proxy == nullptr ? 0 : proxy->v4();
    json["puerto"] = puerto;

    json["ssidAP"] = ssidAP;
    json["passwordAP"] = passwordAP;
    json["apHabilitado"] = apHabilitado;

    json["serverIp"] = serverIp;
    json["rutaApi"] = rutaApi;
    json["modoRegistro"] = modoRegistro;

    serializeJson(json, strJson);
    Serial.println(strJson);
    Serial.println("nknsa");
    
    File f = LittleFS.open("/config.json", "w");
    f.print(strJson);
    f.close();
}

//=============================================================
// Lee la configuracion desde la memoria del ESP
// Esta es deserealizada para su facil lectura
void leerConfig()
{
    if (!LittleFS.exists("/config.json"))
    {
        return;
    }
    File configFile = LittleFS.open("/config.json", "r");
    StaticJsonDocument<1000> json;
    deserializeJson(json, configFile);
    configFile.close();

    ssid = json["ssid"].as<String>();
    password = json["password"].as<String>();
    proxyHabilitado = json["proxyHabilitado"].as<bool>();
    uint32_t proxyAux = json["proxy"].as<uint32_t>();
    puerto = json["puerto"].as<int>();

    ssidAP = json["ssidAP"].as<String>();
    passwordAP = json["passwordAP"].as<String>();
    apHabilitado = json["apHabilitado"].as<bool>();

    serverIp = json["serverIp"].as<String>();
    rutaApi = json["rutaApi"].as<String>();
    modoRegistro = json["modoRegistro"].as<bool>();


    if (proxyAux != 0) proxy = new IPAddress(proxyAux);
}