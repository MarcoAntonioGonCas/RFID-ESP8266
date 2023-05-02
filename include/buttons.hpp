// Variables auxiliares que indican cuanto tiempo esta siendo presiondo en boton
bool press = false;
ulong tiempoInicialPresionado = 0;
ulong tiempoEspera = 4000;
ulong tiempoPresionado = 0;


/// @brief Desabilita o habilita el punto de acceso
static void onAPToogle(){
    apHabilitado = !apHabilitado;
   
    ledRFID.prender(100,100,2);
    guardarConfigWifijson();
    conectarAP();
}

/// @brief Borra las configuraciones realizadas y regresa a las puestas de fabrica
static void onResetConfi(){
    delay(400);
    resetConfig();
}


/// @brief Metodo que siempre se estara ejecutando comprobara si se presiono el botonReset y cuanto tiempo fue presionado
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

        //Regresamos el tiempo inicial a 0 para posteriormente realizar mas comprobaciones
        tiempoInicialPresionado = 0;
        
    }
    
}
