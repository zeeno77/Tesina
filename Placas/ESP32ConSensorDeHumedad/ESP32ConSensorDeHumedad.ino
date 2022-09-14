#include "DHT.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

//Constantes del sensor de temperatura 
#define DHTPIN A13
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

//Configuracion del wifi
const char* ssid = "arrakis";
const char* password = "thespicemustflow";
//API REST URL
const char* serverName = "http://192.168.88.53:8000/muestra/";

//Configuraciones Varias
unsigned long sensorDelay = 5000;
unsigned long timerDelay = 30000;
unsigned long lastTime = 0;
String jsonHumedad = "";
String jsonTemperatura = "";
int httpResponseCode = 0;

void setup() {
Serial.begin(115200);
Serial.println("Test DHT22 (DM2302)");
dht.begin();
WiFi.begin(ssid, password);
Serial.println("Conectando a: ");
Serial.print(ssid);
while(WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
}
Serial.println("");
Serial.print("Conectado a la wifi con la IP: ");
Serial.println(WiFi.localIP());
}
 
void loop() {
//Etapa 1 - Sensado
float h = dht.readHumidity();
float t = dht.readTemperature();
if (isnan(h) || isnan(t)) {
Serial.println("Error al obtener los datos");
return;
}
// Abrir el Serial monitor para ver los datos
Serial.print("Humedad: ");
Serial.print(h);
Serial.print(" %\t");
Serial.print("Temperatura: ");
Serial.print(t);
Serial.println(" *C ");
delay(sensorDelay);
//Etapa2 - Envio
  //Send an HTTP POST request every 30 seconds
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
      // Your Domain name with URL path or IP address with path
      http.begin(client, serverName);
      // Specify content-type header
      
      http.addHeader("Content-Type", "application/json");
      jsonHumedad = "{\"datosensado\":\"Humedad\",\"valor\":\""+String(h)+"\"}";
      jsonTemperatura = "{\"datosensado\":\"Temperatura\",\"valor\":\""+String(t)+"\"}";

      Serial.println("Post:");
      Serial.println(jsonHumedad); 
      httpResponseCode = http.POST(jsonHumedad);
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      Serial.println("Post:");
      Serial.println(jsonTemperatura); 
      httpResponseCode = http.POST(jsonTemperatura);
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
        
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }

}
