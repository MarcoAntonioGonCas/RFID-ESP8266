// const char *ssid = "TP-Link_8BB6";
// const char *password = "71904382";
// String ServerIp  = "http://192.168.0.104:44355";
// String rutaApi = "/api/ident/";
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