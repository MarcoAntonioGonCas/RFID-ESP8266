bool press = false;
ulong tiempoInicialPresionado = 0;
ulong tiempoEspera = 4000;
ulong tiempoPresionado = 0;

static void onAPToogle(){
    apHabilitado = !apHabilitado;
    ledRFID.prender(100,100,2);
    guardarConfigWifijson();
    conectarAP();
}
static void onResetConfi(){
    delay(400);
    resetConfig();
}
void loopButtonAPReset(){
    //Si se presiono el boton se indica el tiempo en el que se inicio
    if (digitalRead(pinbtnAPReset) == LOW)
    {
        if(tiempoInicialPresionado == 0){
            tiempoInicialPresionado = millis();
        }

        
        //Si se sigue presionando el boton de sale de la funcion
        return;
    }
    
    //Al dejar de presionar el boton
    //Comprobamos que exista un tiempo de inicio 

    if(tiempoInicialPresionado != 0){

        //Comprobamos el tiempo que estuvo presionado el el boton 
        tiempoPresionado = millis() - tiempoInicialPresionado;

        if(tiempoPresionado >= tiempoEspera ){
            Serial.println("Restablecionendo configuraciones...");
            onResetConfi();
        }else if(tiempoPresionado >= 300){
            Serial.println(apHabilitado?"Apagando AP":"Prendiendo AP");
            onAPToogle();
        }

        tiempoInicialPresionado = 0;
        
    }
    
}
