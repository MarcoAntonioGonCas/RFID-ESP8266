#include <Arduino.h>
#include <LedLib.h>
void ledLibClass::begin(uint8 pin, TipoLed tipoLed)
{
    this->tipo = tipoLed;
    this->pinLed = pin;
    pinMode(pinLed, OUTPUT);
    apagar();
}
bool ledLibClass::tipoOnLED()
{
    return this->tipo == Anodo ? HIGH : LOW;
}
bool ledLibClass::tipoOffLED()
{
    return this->tipo == Anodo ? LOW : HIGH;
}
void ledLibClass::prender()
{
    digitalWrite(this->pinLed, this->tipoOnLED());
}
void ledLibClass::apagar()
{
    digitalWrite(this->pinLed, this->tipoOffLED());
}
void ledLibClass::prender(ulong_t tiempoEn, ulong_t tiempoApa, uint32_t veces)
{
    this->tiempoIni = millis();
    this->tiempoPrendido = tiempoEn;
    this->tiempoApagado = tiempoApa;
    this->veces = veces;
    this->infinito = false;
}
void ledLibClass::prenderInfinito(ulong_t tiempoEn, ulong_t tiempoApa){
    this->tiempoIni = millis();
    this->tiempoPrendido = tiempoEn;
    this->tiempoApagado = tiempoApa;
    this->veces = 1;
    this->infinito = true;
}
ledLibClass::ledLibClass()
{
    this->pinLed = 0;
    this->tiempoIni = 0;
    this->tipo = Anodo;
    this->infinito = false;
}
void ledLibClass::parar()
{
    this->tiempoIni = 0;
    this->infinito = false;
    apagar();
}
void ledLibClass::loop()
{

    if (tiempoIni == 0)
        return;

    ulong_t tiempoActual = millis();

    ulong_t tiempoTranscurrido = tiempoActual - tiempoIni;
    ulong_t tiempoCiclo = tiempoPrendido + tiempoApagado;
    ulong_t tiempoTotal = tiempoCiclo * veces;

    // ulong_t tiempoSecuencia = tiempoTranscurrido ;

    if ( (tiempoTranscurrido < tiempoTotal)
            || infinito)
    {

        if (tiempoTranscurrido % tiempoCiclo < tiempoPrendido)
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
        tiempoIni = 0;
        apagar();
    }
}