#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>

const char* ssid="BIR_WIFI";
const char* pass="OeBir2019";

AsyncWebServer server(80);

void handleKivonas(AsyncWebServerRequest* request){
    if(request->hasParam("szam1",true) && request->hasParam(" szam2",true) ){
    String nOne=request->arg("szam1");
    String nTwo=request->arg("szam2");
    int a= nOne.toInt();
    int b= nTwo.toInt();
    Serial.print(a-b);
    request->send(200 "text/plain", "Eredmény: " + a-b);
  }
  else{
    // Serial.print("nem jo");
    request->send(400, "text/plain", "Hibás kérés");
  }
}
void handleOsszeadas(AsyncWebServerRequest* request){
    if(request->hasParam("szam1",true) && request->hasParam("szam2",true) ){
    String nOne=request->arg("szam1");
    String nTwo=request->arg("szam2");
    int a= nOne.toInt();
    int b= nTwo.toInt();
    Serial.print(a+b);
    request->send(200 "text/plain", "Eredmény: " + a+b);
  }
  else{
    // Serial.print("nem jo");
    request->send(400, "text/plain", "Hibás kérés");
  }
}
void handleOsztas(AsyncWebServerRequest* request){
    if(request->hasParam("szam1",true) && request->hasParam("szam2",true) ){
    String nOne=request->arg("szam1");
    String nTwo=request->arg("szam2");
    int a= nOne.toInt();
    int b= nTwo.toInt();
    Serial.print(a/b);
    request->send(200 "text/plain", "Eredmény: " + a/b);
  }
  else{
    // Serial.print("nem jo");
    request->send(400, "text/plain", "Hibás kérés");
  }
}
void setup() {
  Serial.begin(115200);
  if (!SPIFFS.begin()){
    Serial.println("SPIFFS inicializálás sikertelen!");
    return;
  }
  WiFi.begin(ssid, pass);
while (WiFi.status() != WL_CONNECTED) 
{
delay(500);
Serial.print(".");
}
Serial.println("\nCsatlakozva! IP: " +WiFi.localIP().toString());

  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
  server.on("/osszeadas", HTTP_POST, handleOsszeadas);
  server.on("/kivonas", HTTP_POST, handleKivonas);
  server.on("/osztas", HTTP_POST, handleOsztas);
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

}
