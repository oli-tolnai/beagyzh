//-//Webszerver, fetch(), async/await
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "BIR_WIFI";
const char* password = "OeBir2019";

ESP8266WebServer server(80);
const int ledPin = LED_BUILTIN;
bool ledState = false;

const char MAIN_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
   <meta charset="UTF-8">
   <head>
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <title>LED vezérlés</title>
      <style>
         body {text-align: center; font-family: Arial; margin: 40px;}
         h1 {color: #333;}
         button {background: #202a66; color: white; border: none;
         padding: 10px 20px; font-size: 16px; border-radius: 5px;
         cursor: pointer;}
         .button:hover {background: #000940;}
      </style>
   </head>
   <body>
      <h1>ESP8266 LED Vezérlés</h1>
      <button class="button" onclick="toggleLED('on')">LED BE</button>
      <button class="button" onclick="toggleLED('off')">LED KI</button>
      <p>ADC állapot: <span id="adcStatus">N/A</span></p>
      <hr>
      <input type="number" id="num1" placeholder="Első szám">
      <input type="number" id="num2" placeholder="Második szám">
      <p>Összeg: <span id="sumRes">N/A</span></p>
      <p>Szorzat: <span id="mulRes">N/A</span></p>
      <button class="button" onclick="calculate()">
      PARAMÉTEREK LEKÉRDEZÉSE
      </button>
      <script>
         //általános státuszt lekezelő függvény
         function handleResponseStatus(response){
             console.log("Válasz státusz:", response.status);
             return response.text();
         }
         
         //általános hibát lekezelő függvény
         function handleError(error){
             console.error("Hiba történt:", error);
         }
         
         //LED vezérlő
         function toggleLED(state){
           let url = '/led?state=' + state;
           fetch(url)
             .then(handleResponseStatus)	//szerver státusz kiírása
             .then(getTogleLEDResponse)		//adat feldolgozás
             .catch(handleError);			//hibakezelés
         }
         //LED response data feldolgozó
         function getTogleLEDResponse(data){
         console.log("Szerver válasz:", data);
         }
         
         //aszinkron lekérdezés
         async function calculate(){
         try{
             //érték beolvas
             let num1value = document.getElementById('num1').value;
             let num2value = document.getElementById('num2').value;
         
             if (num1value === "" || num2value === ""){
                 alert("Add meg mindkét számot!");
                 return;
             }
         
             //query építés
             let params = new URLSearchParams({ num1: num1value, num2: num2value});
             let url = `/calculate?${params.toString()}`;
         
             //kérés indítás
             let response = await fetch(url);
         
             console.log("HTTP státusz: ", response.status);
         
             //visszakapott adat jsonné konvertálása
             let data = await response.json();
         
             console.log("Kapott adatt:", data);
         
             //kapott adat megjelenítése
             document.getElementById("sumRes").innerHTML = data.sumRes;
             document.getElementById("mulRes").innerHTML = data.mulRes;
         
         } catch (error){
           console.error("Hiba történt:", error.message);
         }
         }
         
         function getADCState(){
         let url = '/adc';
           fetch(url)
             .then(handleResponseStatus)
             .then(getADCResponse)
             .catch(handleError);
         }
         
         function getADCResponse(data){
         document.getElementById("adcStatus").innerHTML = data;
         console.log("Szerver válasz -> ADC érték:", data);
         }
         
         function startAutoFetch(){
         setInterval(() => {
           getADCState();
         }, 2000);
         }
         
         document.addEventListener("DOMContentLoaded", startAutoFetch);
         
      </script>
   </body>
</html>
)rawliteral";

void handleRoot() {
  server.send_P(200, "text/html", MAIN_page);
}

void handleAdc() {
  int adc = analogRead(A0);
  server.send(200, "text/plain", String(adc));
}

void handleLED() {
  if (server.hasArg("state")) {
    String state = server.arg("state");
    if (state == "on") {
      digitalWrite(ledPin, LOW);
      ledState = true;
    } else if (state == "off") {
      digitalWrite(ledPin, HIGH);
      ledState = false;
    }
    server.send(200, "text/plain", "LED " + state);
  } else {
    server.send(400, "text/plain", "Hiányzó paraméter");
  }
}

void handleCalculate() {
  if (server.hasArg("num1") && server.hasArg("num2")) {
    int num1 = server.arg("num1").toInt();
    int num2 = server.arg("num2").toInt();
    int sumResValue = num1 + num2;
    int mulResValue = num1 * num2;
    char jsonResponse[100];
    sprintf(jsonResponse, "{\"sumRes\": %d, \"mulRes\": %d}", sumResValue, mulResValue);
    //String jsonResponse = {\"param1\":\"" + ()param1 + "\", \"param2\"" + param2 + "\"}";
    server.send(200, "application/json", jsonResponse);
  } else {
    server.send(400, "application/json", "{\"error\":\"Missing parameters\"}");
  }
}

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Csatlakozás Wi-Fi hálózathoz...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Csatlakozva!");
  Serial.print("IP cím: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/led", handleLED);
  server.on("/adc", handleAdc);
  server.on("/calculate", handleCalculate);

  server.begin();
  Serial.println("Webszerver elindult.");
}

void loop() {
  server.handleClient();
}
