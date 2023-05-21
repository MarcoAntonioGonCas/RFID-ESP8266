//Funciones de ESP
bool restart = false;

static long timeRestartIint = 0;


void reiniciarESP(){
    asyncServer.end();
        
    WiFi.stopSmartConfig();
    WiFi.disconnect(true);
    // WiFi.softAPdisconnect(true);
    delay(200);
    ESP.restart();
}
void limpiarConfigESP(){
    ESP.eraseConfig();
}
void limpiarReiniciarESP(){
    limpiarConfigESP();
    reiniciarESP();
}



void loopRestart(){
    if(restart){
        if(timeRestartIint == 0) timeRestartIint = millis();

        if(millis() - timeRestartIint > 1000){
            reiniciarESP();
        }
    }
}




uint8_t getIdESP(){  
    return ESP.getChipId();
}

void setNameAP(){
    ssidAP = "RFID_AP" + String(getIdESP());
}


bool wifiConectado()
{
  return WiFi.localIP().isSet();
}

//-----------------------------------------------------------------
//Helper
//-----------------------------------------------------------------


/// @brief Convierto un string a un obejeto IPAddress
/// @param str Cadena con formato IPV4
/// @return Retorna un objeto IPAdrres en caso de la cadena no sea valida retorna un objeto vacio
IPAddress toIP(String& str){
    char puntos = 0;
    u8_t partes[4] = {0,0,0,0};

    int parte = 0;
    for(uint i=0; i < str.length(); i++){
        if(str[i]- '0' >= 0 and str[i]-'0'<=9){

            partes[parte] *= 10;
            partes[parte] += str[i]-'0';

        }else if(str[i]=='.'){
            if(parte<3){
                parte++;
            }
            puntos++;
        }
    }
    if(puntos==3){
        return IPAddress(partes[0],partes[1],partes[2],partes[3]);
    }else{
        return IPAddress();
    }
}



/// @brief Indica si la cadena es un numero
/// @param str Cadena la cual sera evaluada
/// @return Retorna un booleano el cual indica si se puede convertir a un numero
bool isNum(String& str){
    if(str.length()==0){
        return false;
    }
    str.trim();

    if(str.compareTo("+.") == 0 or str.compareTo(".+") == 0){
        return false;
    }
    if ( str.length() == 1 and 
            (
                str[0] == '.' or
                str[0] == '+' or
                str[0] == '-' 
            )
        )
    {
        return false;
    }
    

    bool isNum = true;
    bool punto = false;

    for(size_t i = 0 ; i < str.length(); i++ ){

        if(i == 0 and (str[i] == '-' or str[i] == '+')){
            continue;
        }else if(str[i] == '.' and punto == false){
            punto = true;
            continue;
        }else if(!isdigit(str[i])){
            isNum = false;
            break;
        }

    }

    return isNum;
}


/// @brief Convierte la enumeracion wl_enc_type a string
/// @param enc wl_enc_type
/// @return Valor de enumeracion en string
String encryptionTypeToString(uint8_t enc){
    switch(enc) {
      case ENC_TYPE_NONE:
        return "Abierta";
        break;
      case ENC_TYPE_WEP:
        return "WEP";
        break;
      case ENC_TYPE_TKIP:
      case ENC_TYPE_CCMP:
        return "WPA";
        break;
      case ENC_TYPE_AUTO:
        return "Automático";
        break;
      default:
        return "Desconocido";
    }
}
