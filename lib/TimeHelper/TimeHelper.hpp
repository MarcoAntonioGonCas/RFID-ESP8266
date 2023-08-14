#include <Arduino.h>


class TimeDelayHelper
{
        private:
        /* data */
        ulong tiempoInicial = 0;
        public:
        void reset(){
            tiempoInicial = millis();
        }
        bool HanPasado(ulong milisegundos,bool reiniciarAuto){
            if(HanPasado(milisegundos)){
                reset();
                return true;
            }else{
                return false;
            }
        }
        bool HanPasado(ulong milisegundos){
            if(this->tiempoInicial <= 0){
                tiempoInicial = millis();
                return false;
            }

            if(millis() - tiempoInicial >= milisegundos ){
                return true;
            }else{
                return false;
            }
        }
};
