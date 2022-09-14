#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h> 
#include <ESP8266HTTPClient.h>
#include <Arduino_JSON.h>

#define mySerialRx D6
#define mySerialTx D7

SoftwareSerial mySerial(mySerialRx, mySerialTx); 

//Configuracion del wifi
const char* ssid = "arrakis";
const char* password = "thespicemustflow";
//API REST URL
const char* serverName = "http://192.168.88.53:8000/muestra/";


//Configuraciones Varias
unsigned long sensorDelay = 5000;
//unsigned long timerDelay = 30000;
unsigned long timerDelay = 0;
unsigned long lastTime = 0;
String json = "";
int httpResponseCode = 0;
//String recivedData = "empty";

void setup() {
Serial.begin(115200);
mySerial.begin(9600);
delay(10);
WiFi.begin(ssid, password);
while(WiFi.status() != WL_CONNECTED) {
  delay(500);
}

Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}
 
void loop() {
//Etapa 1 - Sensado
    Serial.println("Esperando un mensaje...");
    while (!mySerial.available()) {  
    }
    String recivedData = mySerial.readString();
    Serial.println("recibi: "+ recivedData);
  
//if (isnan(recivedData) || isnan(t)) {
//Serial.println("Error al obtener los datos");
//return;
//}
// Abrir el Serial monitor para ver los datos
//Etapa2 - Envio
  //Send an HTTP POST request every 30 seconds


  
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
      
      // Your Domain name with URL path or IP address with path
      if (!http.begin(client,serverName)) {
        Serial.println("connection failed");
        delay(5000);
        return;
    }
      
      http.addHeader("Content-Type", "application/json");
      json = "{\"datosensado\":\"Data\",\"valor\":\""+recivedData+"\"}";
      
      Serial.println("Post:");
      Serial.println(json);
      httpResponseCode = http.POST(json);
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
