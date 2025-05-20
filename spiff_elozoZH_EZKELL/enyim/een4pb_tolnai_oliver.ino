#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>

const char* ssid = "BIR_WIFI";
const char* password = "OeBir2019";

AsyncWebServer server(80);

// LED BE kapcsolása
void handleLed(AsyncWebServerRequest* request) {
  if (request->hasParam("num1", true) && request->hasParam("num2", true)) {
    String state = request->arg("state");
    // LED vezérlés
 
    int num1 = request->arg("num1").toInt();
    int num2 = request->arg("num2").toInt();
    if (state == "sum") {
      int sumResValue = num1 + num2;
      request->send(200, "text/plain", String(sumResValue));
    } else if (state == "sub") {
      int subResValue = num1 - num2;
      request->send(200, "text/plain", String(subResValue));
    } else if (state == "div") {
      int divResValue = num1 / num2;
      request->send(200, "text/plain", String(divResValue));
    }
    // Válasz küldése
    //request->send(200, "text/plain", sumResValue);
  } else {
    // Ha nincs 'state' paraméter, hibát küldünk
    request->send(400, "text/plain", "Hibás kérés");
  }
}

void notFound(AsyncWebServerRequest* request) {
  String message = "A keresett oldal nem található!";
  request->send(404, "text/plain", message);
}

void setup() {
  Serial.begin(115200);
  while (!Serial){
    delay(100);
  }
  
  pinMode(LED_BUILTIN, LOW);
  Serial.println();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nCsatlakozva! IP: " + WiFi.localIP().toString());
  
  //SPIFFS inicializálása
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS inicializálás sikertelen!");
    return;
  }
  
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
  server.on("/led", HTTP_POST, handleLed);
  server.onNotFound(notFound);
  server.begin();
}

void loop() {}
