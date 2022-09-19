#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <ESP8266HTTPClient.h>
#include <Arduino_JSON.h>

#define mySerialRx D6
#define mySerialTx D7

SoftwareSerial mySerial(mySerialRx, mySerialTx);

// Configuraciones Varias
unsigned long sensorDelay = 5000;
unsigned long timerDelay = 0;
unsigned long lastTime = 0;
String json = "";
int httpResponseCode = 0;
String serverName = "NULL";
String recivedData = "UNSET:NULL";
String recivedCommand = "UNSET";
String recivedValue = "";
int separationIndex = -1;

String receiveCommand(String command, String previusCommand)
{
  Serial.println("Esperando " + command);
  recivedData = "UNSET:NULL";
  separationIndex = -1 ;
  Serial.println("recibi: " + recivedData);
  mySerial.print(previusCommand);
  Serial.println("Enviando " + previusCommand);
  while (recivedCommand != command)
  {
    while (!mySerial.available())
    {
    }
    mySerial.print(previusCommand);
    Serial.println("Enviando " + previusCommand);
    recivedData = mySerial.readString();
    separationIndex = recivedData.indexOf(":");
    if (separationIndex != -1)
    {
      recivedCommand = recivedData.substring(0, separationIndex);
      recivedValue = recivedData.substring(separationIndex + 1, recivedData.length());
      Serial.println("recivedValue: " + recivedValue);
    }
    Serial.println("recivedCommand: " + recivedCommand);
    Serial.println("recibi: " + recivedData);
    separationIndex = -1;
  }
  return recivedValue;
}

void setup()
{
  String ssid = "";
  String password = "";
  Serial.begin(115200);
  mySerial.begin(9600);
  delay(100);
  // Get Wifi parameters via serial
  /*
  Serial.println("Esperando Nombre de Red");
  delay(100);
  while ( recivedCommand != "NETWORK" ) {
    while (!mySerial.available()) {
    }
    recivedData = mySerial.readString();
    separationIndex = recivedData.indexOf(":");
    if (separationIndex != -1) {
      recivedCommand = recivedData.substring(0, separationIndex);
      recivedValue = recivedData.substring(separationIndex + 1, recivedData.length());
      Serial.println("recivedValue: " + recivedValue);
      ssid = recivedValue;
    }
    Serial.println("recivedCommand: " + recivedCommand);
    Serial.println("recibi: " + recivedData);
    separationIndex = -1;
  }
  recivedData = "UNSET:NULL";
  Serial.println("recibi: " + recivedData);
  mySerial.print("OK_NETWORK");
  Serial.println("Enviando OK_NETWORK");

  Serial.println("Esperando Password de Red");
  while ( recivedCommand != "PASSWORD" ) {
    while (!mySerial.available()) {
    }
    mySerial.print("OK_NETWORK");
    Serial.println("Enviando OK_NETWORK");
    recivedData = mySerial.readString();
    separationIndex = recivedData.indexOf(":");
    if (separationIndex != -1) {
      recivedCommand = recivedData.substring(0, separationIndex);
      recivedValue = recivedData.substring(separationIndex + 1, recivedData.length());
      Serial.println("recivedValue: " + recivedValue);
      password = recivedValue;
    }
    Serial.println("recivedCommand: " + recivedCommand);
    Serial.println("recibi: " + recivedData);
    separationIndex = -1;
  }
  mySerial.print("OK_PASSWORD");
 */

  ssid = receiveCommand("NETWORK", "UNSET");
  password = receiveCommand("PASSWORD", "NETWORK");
  serverName =  receiveCommand("SERVER", "PASSWORD");
  //
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  // Etapa 1 - Sensado
  Serial.println("Esperando un mensaje...");
  while (!mySerial.available())
  {
  }
  String recivedData = mySerial.readString();
  Serial.println("recibi: " + recivedData);

  // if (isnan(recivedData) || isnan(t)) {
  // Serial.println("Error al obtener los datos");
  // return;
  // }
  //  Abrir el Serial monitor para ver los datos
  // Etapa2 - Envio
  // Send an HTTP POST request every 30 seconds

  if ((millis() - lastTime) > timerDelay)
  {
    // Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED)
    {
      WiFiClient client;
      HTTPClient http;

      // Your Domain name with URL path or IP address with path
      if (!http.begin(client, serverName))
      {
        Serial.println("connection failed");
        delay(5000);
        return;
      }

      http.addHeader("Content-Type", "application/json");
      json = "{\"datosensado\":\"Data\",\"valor\":\"" + recivedData + "\"}";

      Serial.println("Post:");
      Serial.println(json);
      httpResponseCode = http.POST(json);
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      // Free resources
      http.end();
    }
    else
    {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}
