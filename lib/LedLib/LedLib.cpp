#include <Arduino.h>
#include <LedLib.h>

void ledLibClass::begin(uint8_t pin, TipoLed tipoLed)
{
    setTipoLed(tipoLed);
    this->_pinLed = pin;
    pinMode(_pinLed, OUTPUT);
    apagar();
}

void ledLibClass::prender()
{
    digitalWrite(this->_pinLed, this->_onLed);
}
void ledLibClass::apagar()
{
    digitalWrite(this->_pinLed, this->_offLed);
}


void ledLibClass::prender(ulong_t tiempoEn){
    ulong_t tiempoTranscurrido = millis() - _tiempoIni;

    if( tiempoEn == _tiempoPrendido and
        tiempoTranscurrido < tiempoEn){
        return;
    }

    this->_tiempoIni = millis();
    this->_tiempoPrendido = tiempoEn;
    this->_tiempoApagado = 0;
    this->_veces = 1;
    this->_infinito = false;    

}

/// Enciende el LED durante un período de tiempo determinado.
/// @param tiempoEn Tiempo en milisegundos que el LED estará encendido.
/// @param tiempoApa Tiempo en milisegundos que el LED estará apagado después de estar encendido.
/// @param veces Número de veces que se repetirá el ciclo de encendido y apagado.
/// Si se llama a la función antes de que el ciclo actual termine, se omitirá y se esperará hasta que finalice.
/// Si se desea forzar el inicio de un nuevo ciclo, se debe llamar a la función "parar()".
void ledLibClass::prender(ulong_t tiempoEn, ulong_t tiempoApa, uint32_t veces)
{
    ulong_t tiempoTranscurrido = millis() - _tiempoIni;
    ulong_t tiempoCiclo = _tiempoPrendido + _tiempoApagado;
    ulong_t tiempoTotal = tiempoCiclo * veces;

    // Si se llama a la función con los mismos parámetros antes de que el ciclo actual termine, se omite
    if( tiempoEn == this->_tiempoPrendido and
        tiempoApa == this->_tiempoApagado and 
        tiempoTranscurrido < tiempoTotal){
            return;
    }


    this->_tiempoIni = millis();
    this->_tiempoPrendido = tiempoEn;
    this->_tiempoApagado = tiempoApa;
    this->_veces = veces;
    this->_infinito = false;
}


/// @brief Enciende el LED de forma infinita, alternando entre un período de tiempo encendido y otro apagado.
/// @param tiempoEn Tiempo en milisegundos que el LED estará encendido.
/// @param tiempoApa Tiempo en milisegundos que el LED estará apagado después de estar encendido.
/// Si se llaman a la función con los mismos parámetros que el ciclo actual, se omite.
/// Si se desean realizar cambios en los parámetros, se iniciará un nuevo ciclo.
void ledLibClass::prenderInfinito(ulong_t tiempoEn, ulong_t tiempoApa){
    _infinito = true;

    // Si las configuraciones son las mismas, se omite
    if(this->_tiempoPrendido == tiempoEn &&
     this->_tiempoApagado == tiempoApa){
        return;
    }
    this->_tiempoIni = millis();
    this->_tiempoPrendido = tiempoEn;
    this->_tiempoApagado = tiempoApa;
    this->_veces = 1;
}
ledLibClass::ledLibClass()
{
    this->_pinLed = 0;
    this->_tiempoIni = 0;
    this->_infinito = false;
}
void ledLibClass::parar()
{
    this->_tiempoIni = 0;
    this->_tiempoPrendido = 0;
    this->_tiempoApagado = 0;
    this->_veces = 0;
    this->_infinito = false;
}
void ledLibClass::loopInfinito(){

    ulong_t tiempoActual = millis();
    ulong_t tiempoTranscurrido = tiempoActual - _tiempoIni;
    ulong_t tiempoCiclo = _tiempoPrendido + _tiempoApagado;


    if(tiempoTranscurrido % tiempoCiclo <  _tiempoPrendido){

        prender();
    }else{
        apagar();
    }

}
void ledLibClass::loop()
{
    if(_infinito){
        loopInfinito();

        
        return;
    }

    if (_tiempoIni == 0){
        
        return;
    }
    ulong_t tiempoActual = millis();

    ulong_t tiempoTranscurrido = tiempoActual - _tiempoIni;
    ulong_t tiempoCiclo = _tiempoPrendido + _tiempoApagado;
    ulong_t tiempoTotal = tiempoCiclo * _veces;

    

    if (tiempoTranscurrido < tiempoTotal)
    {

        if (tiempoTranscurrido % tiempoCiclo < _tiempoPrendido)
        {

            prender();
        }
        else
        {

            apagar();
        }


    }
    else
    {
        _tiempoIni = 0;
        apagar();
    }
}