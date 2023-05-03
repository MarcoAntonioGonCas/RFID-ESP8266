//============================================================
// Guarda la confguracion en la memoria del ESP
// En un archivo JSON para leerlo facilmente
void guardarConfigWifijson()
{
    String strJson;
    StaticJsonDocument<2000> json;

    json["ssid"] = ssid;
    json["password"] = password;
    json["proxyHabilitado"] = proxyHabilitado;
    json["proxy"] = proxy.v4();
    json["puerto"] = puerto;

    json["ssidAP"] = ssidAP;
    json["passwordAP"] = passwordAP;
    json["apHabilitado"] = apHabilitado;

    json["serverIp"] = serverIp;
    json["rutaApi"] = rutaApi;
    json["modoRegistro"] = modoRegistro;
    json["autorizacion"] = token;
    json["certificadoHttps"] = certificadoHttps;

    serializeJson(json, strJson);
    Serial.println("Guardando json:::::");
    Serial.println(strJson);
    
    File f = LittleFS.open("/config.json", "w");
    f.print(strJson);
    f.close();
}

//=============================================================
// Lee la configuracion desde la memoria del ESP
// Esta es deserealizada para su facil lectura
void leerConfigWifiJson()
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
    token = json["autorizacion"].as<String>();
    certificadoHttps = json["certificadoHttps"].as<String>();

    Serial.println("Leyendo json:::::");
    serializeJson(json,Serial);
    Serial.println();
    Serial.println(ssidAP);
    Serial.println(passwordAP);
    

    if (proxyAux != 0) proxy = IPAddress(proxyAux);
}
void guardarConfigUserJson(){
    String strJson;
    StaticJsonDocument<100> json;

    json["usuarioLogin"] = usuarioLogin;
    json["contraLogin"] = contraLogin;
    serializeJson(json,strJson);

    File f=LittleFS.open("/loginConfig.json","w");
    f.print(strJson);
}
void leerConfigUserJson(){
    if(!LittleFS.exists("/loginConfig.json")){
        return;
    }
    StaticJsonDocument<100> json;
    File f=LittleFS.open("/loginConfig.json","r");
    deserializeJson(json,f);

    usuarioLogin = json["usuarioLogin"].as<String>();
    contraLogin = json["contraLogin"].as<String>();
}
void borrarConfig(){
    if(LittleFS.exists("/loginConfig.json")){
        LittleFS.remove("/loginConfig.json");
    }
    
    if (LittleFS.exists("/config.json"))
    {
        LittleFS.remove("/config.json");
    }
}
void resetConfig(){
    if(LittleFS.exists("/loginConfig.json")){
        LittleFS.remove("/loginConfig.json");
    }
    
    if (LittleFS.exists("/config.json"))
    {
        LittleFS.remove("/config.json");
    }

    WiFi.disconnect(true);
    delay(100);
    ESP.restart();
}


void leerConfig(){
    leerConfigUserJson();
    leerConfigWifiJson();
}   
void guardarConfigjson(){
    guardarConfigUserJson();
    guardarConfigWifijson();
}

//TODO: Codigo que no se usa
String readString(File& f){
    
    uint8_t* pbuff = new uint8_t[200+1];
    size_t tama = f.size();
    String temp = "";

    if(tama==0){
        return "";
    }
    int leidos = 0;
    do{
        
        leidos = f.read(pbuff,200-1);
        pbuff[leidos] = 0;

        temp += *pbuff;


    }while (leidos != 0);

    return temp;
}

void rellenaString(String& str,uint8_t* buff,int len){
    for(int i=0;i<len;i++){
        str+=buff[i];
    }
}
String readStringEvil(File& f){
    int lec = 200;
    uint8_t* pbuff = new uint8_t[lec];
    size_t tama = f.size();
    String temp = "";

    if(tama==0){
        return "";
    }
    
    int vueltas = floor(tama/ static_cast<double>(lec));
    int restante = tama % lec;

    for(int i = 0; i < vueltas; i++){
        f.read(pbuff,lec);
        rellenaString(temp,pbuff,lec);
    }
    if(restante!=0){
        f.read(pbuff,lec);
        rellenaString(temp,pbuff,restante);
    }
    return temp;
}
//---termina codigo que no se usa