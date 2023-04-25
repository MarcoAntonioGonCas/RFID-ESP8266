
//=============================================================
// Indica si el ESP se encuentra concectado a una red WI-FI
bool wifiConectado()
{
  return WiFi.localIP().isSet();
}
//Cambia la el nombre del punto de aceso WIFI
void toogleAP()
{
  if (!apHabilitado){
    WiFi.softAPdisconnect();
  }
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
  Serial.println("Entrando a la una nueva conexion wifi");
  //Compara si existe una 
  if( (ssid.compareTo(WiFi.SSID()) == 0 and password.compareTo(WiFi.psk()) == 0 ) and
      (ssid.compareTo("") !=0 )){
        Serial.println("Saliendo de la conexion de red wifi");
    return;
  }
  Serial.println("Comprobando wificonectado");
  wifiConectado();
  Serial.println("Saliendo ");

  if (wifiConectado())
  {
    // WiFi.softAPdisconnect(true);
    WiFi.disconnect(true);
    Serial.println("Desconectando la conexion actual wifi");
    //TODO: Agregar metodo de reincio
    delay(100);
    restartESP();
  }
  Serial.println(ssid);
  Serial.println(password);
  WiFi.begin(ssid, password);
  Serial.println("Conectando");
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
  //=======================================================
}

