void conectarWiFi(){
  if(WiFi.status() == wl_status_t::WL_CONNECTED){
    WiFi.disconnect();
    WiFi.localIP().clear();
  }
  WiFi.begin(ssid,password);
}
void iniciarSTAWiFI(){
  WiFi.mode(WiFiMode::WIFI_AP_STA);
  WiFi.softAPConfig(apIp,apIp,IPAddress(255,255,255,0));
  WiFi.setAutoReconnect(true);
  //=======================================================
  Serial.println(F("Iniciado Punto de acceso"));
  while(!WiFi.softAP("RFID_AP","RFID2022MA")){
      Serial.print(F("."));
      delay(100);
  }

  Serial.println(F("Punto de acceso iniciado"));
  Serial.println(WiFi.softAPSSID());
  Serial.println(WiFi.softAPIP());
}

bool wifiConectado(){
  return WiFi.localIP().isSet();
}