#include <Arduino.h>


class TimeDelayHelper
{
        private:
        /* data */
        ulong tiempoInicial = 0;
        public:
        void reset(){
            // Reiniciamos el tiempo inicial a 0 que es el valor por defecto
            tiempoInicial = 0;
        }
        // Indica si ya han pasado milisegundos tambien reinicia el contador automaticamente 
        bool HanPasado(ulong milisegundos,bool reiniciarAuto){
            if( HanPasado(milisegundos) ){
                reset();
                return true;
            }else{
                return false;
            }
        }
        bool HanPasado(ulong milisegundos){
            
            // Si es la primera vez que llamamos al metodo asignamos el valor en milesgundos actual
            // para realizar la comparacion
            if(this->tiempoInicial <= 0){

                tiempoInicial = millis();

                return false;
            }

            // A los milisegundos actuales le restamos los milisegundos iniciales obtenemos el tiempo que ha pasado
            // Si ya pasaron los milisegundos retornoamos true en caso contrario false
            if(millis() - tiempoInicial >= milisegundos ){
                return true;
            }else{
                return false;
            }
        }
};
