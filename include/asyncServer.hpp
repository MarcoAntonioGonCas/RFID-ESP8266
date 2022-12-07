#include <ESPAsyncWebServer.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>

const char *usario = "rfid";
const char *contra = "1234";

void filesStatic(AsyncWebServer &async)
{
    async.serveStatic("/", LittleFS, "/www/");
}
void handleLoginGet(AsyncWebServerRequest *req)
{
    if (!LittleFS.exists("/login.html"))
    {
        req->send(404, "text/plain", "No encontrado");
        return;
    }

    req->send(LittleFS, "/login.html", "text/html");
}
void handleLoginPost(AsyncWebServerRequest *req)
{

    int numParametros = req->params();

    if (numParametros == 2)
    {
        String pUsuario = req->getParam(0)->value();
        String pContra = req->getParam(1)->value();
        if (strcmp(pUsuario.c_str(), usario) == 0 &&
            strcmp(pContra.c_str(), contra) == 0)
        {
            req->redirect("/home");
        }
    }
    else
    {
        req->redirect("/");
    }
}
void handlePrincipalGet(AsyncWebServerRequest *req)
{
    if (!LittleFS.exists("/principal.html"))
    {
        req->send(404, "text/plain", "No encontrado");
        return;
    }

    File f = LittleFS.open("/principal.html", "r");
    String doc = f.readString();
    // #nombre#
    // #contra#
    // #servidor#
    // #api#
    doc.replace("#nombre#", ssid);
    doc.replace("#contra#", password);
    doc.replace("#servidor#", ServerIp);
    doc.replace("#api#", rutaApi);

    req->send(200, "text/html", doc);
}
void handlePrincipalPost(AsyncWebServerRequest *req)
{
    //nombre
    // contra
    // servidor
    // api


    
}
void addRouters(AsyncWebServer &asyncServer)
{
    filesStatic(asyncServer);
    asyncServer.on("/", HTTP_GET, handleLoginGet);
    asyncServer.on("/", HTTP_POST, handleLoginPost);
    asyncServer.on("/home", HTTP_GET, handlePrincipalGet);
    asyncServer.on("/home", HTTP_POST, handlePrincipalPost);
    asyncServer.onNotFound([](AsyncWebServerRequest *req)
                           { 
                            req->send(LittleFS,"/noEncontrado.html","text/html");
                           });
}