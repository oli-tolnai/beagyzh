//-// 12. SPIFF Webszerver, SPIFFS v2.pdf
//-// ESP8266 - Fájlok listázása https://main.elearning.uni-obuda.hu/pluginfile.php/464853/mod_resource/content/1/SPIFFS_v2.pdf


#include <ESP8266WiFi.h>
#include <FS.h>
void setup() {
	Serial.begin(115200);
	// Fájlrendszer csatolása
	SPIFFS.begin();
	//gyökérkönyvátr megnyitása
	//SPIFFS esetén csak a gyökérkönyvtár van!
	Dir dir = SPIFFS.openDir ("/");
	//dir.next() = true amíg van fájl
	while (dir.next()) {
		Serial.println(dir.fileName());
		Serial.println(dir.fileSize());
	}
}
void loop() {
}

//-// ESP8266 - Fájl megnyitása és olvasása
#include "FS.h"

void setup() {
	Serial.begin(115200);
	
	if(!SPIFFS.begin()){
		Serial.println("An Error has occurred while mounting SPIFFS");
		return;
	}
	
	File file = SPIFFS.open("/test_example.txt", "r");
	if(!file){
		Serial.println("Failed to open file for reading");
		return
	}
	
	Serial.println();
	Serial.println("File Content:");
	while(file.availabdle()){
		Serial.write(file.read());
	}
	file.close();
}

void loop() {
}

//-// Aszinkron webszerver példa LED vezérlés külön html,css,js fájlok SPIFFS-sel
// script.js:
async function setLed(state) {
	try {
		const response = await fetch("/led", {
			method: "POST",
			headers: {
				"Content-Type": "application/x-www-form-urlencoded"
			},
			// A query paraméter a POST body-ba kerül
			body: `state=${state}`
		});
		
		if (!response.ok) {
			throw new Error(`Hiba: ${response.status} - ${response.statusText}`);
		}
		
		const data =  await response.text();
		// Kiírjuk a válasz szöveget
		console.log("Szerver válasza:", data);
	} catch (error) {
		// Hibakezelés
		console.error("Hiba történt:", error);
	}
}

// styles.css
body {
	font-family: Arial, sans-serif;
	background-color: #f0f0f0;
	color: #f0f0f0;
	margin: 0;
	padding: 0;
	display: flex;
	flex-direction: column;
	justify-content: center;
	align-items: center;
	text-align: center;
}

//index.html
<!DOCTYPE html>
<html lang="hu">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>LED vezérlés</title>
    <!-- CSS FÁJL HIVATKOZÁS-->
     <link rel="stylesheet" href="styles.css">
    <!-- defer -> DOM betöltése után indul el-->
     <script src="script.js" defer></script>
</head>

<body>
    <header>
        <img src="logo.png" alt="Logo" class="logo">
    </header>
    <h1>LED vezérlés</h1>
    <button onclick="setLed('on')">LED BE</button>
    <button onclick="setLed('off')">LED KI</button>
</body>

</html>

// Aszinkron webszerver kód
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>

const char* ssid = "BIR_WIFI";
const char* password = "OeBir2019";

AsyncWebServer server(80);

// LED BE kapcsolása
void handleLed(AsyncWebServerRequest* request) {
	// Ha a paraméter 'stata' létezik a bodyban
	if (request->hasParam("state", true)) {
		String state = request->arg("state");
		// LED vezérlés
		if (state == "on") {
			digitalWrite(LED_BUILTIN, LOW);
		} else if (state == "off") {
			digitalWrite(LED_BUITLIN, HIGH);
		}
		// Válasz küldése
		request->send(200, "text/plain", "LED állapot: " + state);
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
	
	pinMode(LED_BUILTIN, OUTPUT);
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
	
	//pl1. server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
	//param1: gyökér URL esetén
	//param2: a szerver a SPIFFS fájlrendszerből tölti be az adatokat.
	//param3: SPIFFS fájlrendszerben lévő fájlok gyökérkönyvtárát jelöli
	server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
	server.on("/led", HTTP_POST, handleLed);
	server.onNotFound(notFound);
	server.begin();
}

void loop() {}







