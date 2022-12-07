#ifndef _LEDLIB_H
#define _LEDLIB_H

#include <Arduino.h>

enum TipoLed:int{
    Catodo,
    Anodo
};
typedef unsigned long ulong_t;
class ledLibClass
{
private:
    bool tipoOnLED();
    bool tipoOffLED();
    void apagar();
    TipoLed tipo;
    ulong_t tiempoIni;
    ulong_t tiempoPrendido;
    ulong_t tiempoApagado;
    uint32_t veces;
    uint8_t pinLed;
    bool infinito = false;
public:
    void inline setPin(uint8_t pinLed)
    {
        this->pinLed = pinLed;
    }
    void inline setTipoLed(TipoLed tipo){
        this->tipo=tipo;
    }
    
    void begin(uint8_t pin,TipoLed tipoLed);
    void parar();
    void prender();
    void prender(ulong_t tiempoEn, ulong_t tiempoApa,uint32_t veces);
    void prenderInfinito(ulong_t tiempoEn, ulong_t tiempoApa);
    void loop();
    ledLibClass();
};

#endif