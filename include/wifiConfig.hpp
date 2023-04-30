
//=============================================================
// Indica si el ESP se encuentra concectado a una red WI-FI

static bool mostrarInfoAp = false;
static bool mostrarInfoWifi = true;

bool wifiConectado()
{
  return WiFi.localIP().isSet();
}
//Cambia la el nombre del punto de aceso WIFI
void conectarAP()
{
  mostrarInfoAp = true;
  
  if (!apHabilitado){
    WiFi.softAPdisconnect();
    Serial.println("Punto de acceso apagado");
    return;
  }
  
  Serial.println(F("\nIniciado Punto de acceso"));
  Serial.println(ssidAP);
  Serial.println(passwordAP);

  int intentos  = 0; 
  
  while (!WiFi.softAP(ssidAP, passwordAP) and intentos < 20)
  { 
    Serial.print(".");
    delay(200);
    intentos++;
  }
    Serial.println(F("\nPunto de acceso iniciado"));
    Serial.println(WiFi.softAPSSID());
    Serial.println(WiFi.softAPIP());
}
//=============================================================
// Inicia una nueva conexion a una red Wi-Fi
// Esto con el nombre y contraeña puestos en config.h
//=============================================================
void conectarWiFi()
{
  Serial.println("Conectando a la red WIFI....");

  if (wifiConectado())
  {
    restart = true;
    return;
  }

  WiFi.begin(ssid, password);

  Serial.println(ssid);
}





void loopAP(){
  
}

//=============================================================
// Metodo que se ejecutara en la funcion loop e indicara
// si existe una conexion wifi atravez del led indicador
//=============================================================
void loopWiFi()
{
  if (!wifiConectado())
  {
    ledWIFI.prenderInfinito(1000, 500);
    mostrarInfoWifi = true;
  }
  else
  {
    ledWIFI.parar();
    ledWIFI.prender();
    if (mostrarInfoWifi)
    {
      Serial.printf("Conectado a WIFI %s con ip: %s", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
      mostrarInfoWifi = false;
    }
  }
}


//=============================================================
// Configuramos el WIFI como modo Punto de acceso y estacion
//=============================================================
void configAPWIFI()
{
  
  WiFi.mode(WiFiMode::WIFI_AP_STA);
  WiFi.softAPConfig(apIp, apIp, IPAddress(255, 255, 255, 0));
  WiFi.setAutoReconnect(true);
}