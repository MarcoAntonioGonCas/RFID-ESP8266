#ifndef _LEDLIB_H
#define _LEDLIB_H

#include <Arduino.h>

typedef unsigned long ulong_t;
enum TipoLed : int
{
    Catodo,
    Anodo
};
class ledLibClass
{
private:
    
    ulong_t _tiempoIni;
    ulong_t _tiempoPrendido;
    ulong_t _tiempoApagado;
    uint32_t _veces;
    uint8_t _pinLed;


    bool _infinito = false;
    bool _onLed = HIGH;
    bool _offLed = LOW;


    void apagar();
    void loopInfinito();
    

public:
    void inline setPin(uint8_t pinLed)
    {
        this->_pinLed = pinLed;
    }
    void setTipoLed(TipoLed tipo)
    {
        if(tipo == TipoLed::Anodo){

            this->_onLed  = HIGH;
            this->_offLed = LOW;
        }else if(tipo == TipoLed::Catodo){

            this->_onLed  = LOW;
            this->_offLed = HIGH;
        }
        Serial.println("Tipo encendido apagado");
        Serial.println(_onLed);
        Serial.println(_offLed);
    }

    void begin(uint8_t pin, TipoLed tipoLed);
    void parar();
    void prender();
    void prender(ulong_t tiempoEn);
    void prender(ulong_t tiempoEn, ulong_t tiempoApa, uint32_t veces);
    void prenderInfinito(ulong_t tiempoEn, ulong_t tiempoApa);
    void loop();
    ledLibClass();
};

#endif