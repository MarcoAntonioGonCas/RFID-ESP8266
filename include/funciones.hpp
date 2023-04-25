//Funciones de ESP
void restartESP(){
    ESP.restart();
}
void clearConfigESP(){
    ESP.eraseConfig();
}
void clearAndRestartESP(){
    clearConfigESP();
    restartESP();
}

uint8_t getIdESP(){  
    return ESP.getChipId();
}

void setNameAP(){
    ssidAP = "RFID_AP" + String(getIdESP());
}
//Helpers
IPAddress toIP(String& str){
    char puntos = 0;
    u8_t partes[4] = {0,0,0,0};

    int parte = 0;
    for(uint i=0; i < str.length(); i++){
        if(str[i]-'0' >= 0 and str[i]-'0'<=9){

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