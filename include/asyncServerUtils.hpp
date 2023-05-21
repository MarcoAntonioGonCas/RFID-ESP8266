/// @brief Obtiene el valor de la peticion web en el párametro valorNuevaConfiguracion
/// @param req Indica el objeto peticicon
/// @param nombreParametroPeticion Indica como se llama el parametro en la peticion
/// @param valorNuevaConfiguracion Indica el String en donde se almacenara el valor de la peticion
/// @param post Indica si la peticion se trata como peticion POST
void obtenerValorDePeticion(AsyncWebServerRequest *req,const char * nombreParametroPeticion, String &valorNuevaConfiguracion, bool post=true){
   valorNuevaConfiguracion = "";

   if( req->hasParam(nombreParametroPeticion, post)){
      valorNuevaConfiguracion = req->getParam(nombreParametroPeticion, post)->value();
      valorNuevaConfiguracion.trim();
   }


}

/// @brief Obtiene el valor de la peticion web 
/// @param req Indica el objeto peticicon
/// @param nombreParam Indica como se llama el parametro en la peticion
/// @param valorNuevaConfiguracion Indica el String en donde se almacenara el valor de la peticion
/// @param valorConfiguracion Indica el string de la configuracion anterior la cual sera cambiada en caso de que existieran nuevos cambios
/// @param vacio Indica si el parametro puede estar vacio
/// @param post Indica si la peticion se trata como peticion POST
/// @return Devuelve verdadero en caso de que hubieran nuevos cambios si la peticion tiene el mismo valor de la configuracion actual devuele false
bool obtenerValorYComparar(AsyncWebServerRequest *req,const char * nombreParam, String &valorNuevaConfiguracion,String &valorConfiguracion, bool vacio = false, bool post =true){
   valorNuevaConfiguracion = "";
   obtenerValorDePeticion(req,nombreParam,valorNuevaConfiguracion,post);
   //Comparamos que los valores no sean iguales o si esta vacio 
   //Dependiendo del parametro
   if(vacio){
         if( valorNuevaConfiguracion.compareTo(valorConfiguracion) != 0){
            valorConfiguracion = valorNuevaConfiguracion;
            return true;
         }else{
            return false;
         }
   }else{
      if( valorNuevaConfiguracion.compareTo(valorConfiguracion) != 0 and 
         !valorNuevaConfiguracion.isEmpty()){
            valorConfiguracion = valorNuevaConfiguracion;
            return true;
         }else{
            return false;
         }
   }
   

}

/// @brief Envia la pagina de carga
/// @param req Indica el objeto peticion
/// @param tiempo Inica el tiempo que cargara la pagina antes de ser redirigida en milisegundos
/// @param url Indica el url al cual sera redirigida la pagina en cuanto pasen los milisegundos indicados
void enviarHtmlCarga(AsyncWebServerRequest *req,int tiempo,const char* url){
   if(!LittleFS.exists("/carga.html")){
      req->send(200,"text/plain","No sa encontro la pagina");
      return;
   }
   File f = LittleFS.open("/carga.html", "r");
   String doc = f.readString();


   doc.replace("%time%",String(tiempo));
   doc.replace("%link%",url);


   req->send(200,"text/html",doc);
}