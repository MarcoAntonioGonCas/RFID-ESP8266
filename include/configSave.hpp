
//============================================================
// Guarda la confguracion en la memoria del ESP
// En un archivo JSON para leerlo facilmente
void guardarConfigWifijson()
{
    StaticJsonDocument<2000> json;

    json["ssid"] = ssid;
    json["password"] = password;
    json["proxyH"] = proxyHabilitado;
    json["proxy"] = proxy.v4();
    json["puerto"] = puerto;

    json["ssidAP"] = ssidAP;
    json["passwordAP"] = passwordAP;
    json["apHabilitado"] = apHabilitado;

    json["serverIp"] = serverIp;
    json["rutaApi"] = rutaApi;
    json["rutaApiRegis"] = rutaApiRegistro;
    json["modoRegistro"] = modoRegistro;
    json["autorizacion"] = token;
    json["codigoInter"] = codigoIntercambio;
    json["salon"] = salon;

    File f = LittleFS.open("/config.json", "w");

    Serial.println("Guardando json:::::");
    serializeJsonPretty(json,Serial); 
    serializeJson(json, f);
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
    StaticJsonDocument<2000> json;
    deserializeJson(json, configFile);
    configFile.close();

    ssid = json["ssid"].as<String>();
    password = json["password"].as<String>();
    proxyHabilitado = json["proxyH"].as<bool>();
    uint32_t proxyAux = json["proxy"].as<uint32_t>();
    puerto = json["puerto"].as<int>();

    ssidAP = json["ssidAP"].as<String>();
    passwordAP = json["passwordAP"].as<String>();
    apHabilitado = json["apHabilitado"].as<bool>();

    serverIp = json["serverIp"].as<String>();
    rutaApi = json["rutaApi"].as<String>();
    rutaApiRegistro = json["rutaApiRegis"].as<String>();
    modoRegistro = json["modoRegistro"].as<bool>();
    token = json["autorizacion"].as<String>();
    codigoIntercambio = json["codigoInter"].as<String>();
    salon = json["salon"].as<String>();


    Serial.println("Leyendo json:::::");
    serializeJsonPretty(json,Serial);    

    if (proxyAux != 0) proxy = IPAddress(proxyAux);
}




void guardarConfigUserJson(){

    StaticJsonDocument<100> json;

    json["usuarioLogin"] = usuarioLogin;
    json["contraLogin"] = contraLogin;

    File f=LittleFS.open("/loginConfig.json","w");
    serializeJson(json,f);
    f.close();
    
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

void guardarCertificadoJson(){
    StaticJsonDocument<2000> json;
    json["cert"] = certificadoHttps;

    File f = LittleFS.open("/certi.json","w");
    serializeJson(json,f);
    f.close();
}
void leerCertificadoJson(){
    if(!LittleFS.exists("/certi.json"))
    {
        return;
    }

    StaticJsonDocument<2000> json;

    File f = LittleFS.open("/certi.json","r");

    deserializeJson(json,f);
    f.close();

    certificadoHttps = json["cert"].as<String>();

}
void borrarConfig(){
    if(LittleFS.exists("/loginConfig.json")){
        LittleFS.remove("/loginConfig.json");
    }
    
    if (LittleFS.exists("/config.json"))
    {
        LittleFS.remove("/config.json");
    }
    if(LittleFS.exists("/certi.json")){
        LittleFS.remove("/certi.json");
    }
}
void resetConfig(){
    borrarConfig();

    WiFi.disconnect(true);
    delay(100);
    ESP.restart();
}


void leerConfig(){
    leerConfigUserJson();
    leerConfigWifiJson();
    leerCertificadoJson();
}   
void guardarConfigjson(){
    guardarConfigUserJson();
    guardarConfigWifijson();
    guardarCertificadoJson();
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