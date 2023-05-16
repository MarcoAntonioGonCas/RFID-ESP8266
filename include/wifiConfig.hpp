
//=============================================================
// Indica si el ESP se encuentra concectado a una red WI-FI
// Mostrar mensaje de conexion ESP
static bool mostrarInfoWifi = true;

bool wifiConectado()
{
  return WiFi.localIP().isSet();
}


/// @brief Habilita o desabilita el AP 
void iniciarAP()
{
  WiFi.softAPdisconnect();
  WiFi.enableAP(apHabilitado);
  if (!apHabilitado){
    Serial.println(F("Punto de acceso apagado"));
    return;
  }
  
  Serial.println(F("\nIniciando Punto de acceso"));
  Serial.println(ssidAP);
  Serial.println(passwordAP);

  int intentos  = 0; 
  while (!WiFi.softAP(ssidAP, passwordAP) and intentos < 20)
  { 
    Serial.print(F("."));
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


//REGION LOOPS
// LOOPS


void loopAP(){
  
}


//=============================================================
// Obtiene la hora 
//=============================================================
void configurarHora() {
  configTime(6 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Serial.println("Sincronizando hora: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
      delay(500);
      Serial.print(".");
      now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Tiempo actua: ");
  Serial.println(asctime(&timeinfo));
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
      Serial.printf("Conectado a WIFI %s con ip: %s \n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
      mostrarInfoWifi = false;
      configurarHora();
    }
  }
}


//=============================================================
// Configuramos el WIFI como modo Punto de acceso y estacion
//=============================================================
void configAPWIFI()
{
  
  WiFi.mode(WiFiMode::WIFI_AP_STA);
  WiFi.softAPConfig(apIp, IPAddress(192,168,0,254), IPAddress(255, 255, 255, 0));
  WiFi.setAutoReconnect(true);
}