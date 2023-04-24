
//=============================================================
// Indica si el ESP se encuentra concectado a una red WI-FI
bool wifiConectado()
{
  return WiFi.localIP().isSet();
}
void toogleAP()
{
  if (!apHabilitado){
    WiFi.softAPdisconnect();
  }
  //   return;

  Serial.println(F("Iniciado Punto de acceso"));
  while (!WiFi.softAP(ssidAP, passwordAP))
  { 
    Serial.print(F("."));
     delay(100);
  }
  Serial.println(F("Punto de acceso iniciado"));
  Serial.println(WiFi.softAPSSID());
  Serial.println(WiFi.softAPIP());
}
//=============================================================
// Inicia una nueva conexion a una red Wi-Fi
// Esto con el nombre y contraeña puestos en config.h
void conectarWiFi()
{
  if(WiFi.SSID() == ssid and WiFi.psk()==password){
    return;
  }

  if (wifiConectado())
  {
    Serial.println("Desconectar wifi");
    // ESP.eraseConfig();
    // asyncSocket.closeAll();
    // asyncSocket.cleanupClients();
    // WiFi.disconnect(false,true);
    // WiFi.localIP().clear();
    // delay(200);
    //TODO: Agregar metodo de reincio
  }
  Serial.println(ssid);
  Serial.println(password);
  WiFi.begin(ssid, password);
  Serial.println("INtentrando conectar");
}

//=============================================================
// inicia el puento de acceso del ESP
// Con el nombre y contraseña puestos en config.h
void configAPWIFI()
{
WiFi.persistent(false);

  WiFi.mode(WiFiMode::WIFI_AP_STA);
  WiFi.softAPConfig(apIp, apIp, IPAddress(255, 255, 255, 0));
  WiFi.setAutoReconnect(true);
  toogleAP();

  //=======================================================
}

