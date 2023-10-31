#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <ESP8266HTTPClient.h>
#include <Arduino_JSON.h>

#define mySerialRx D6
#define mySerialTx D7

SoftwareSerial mySerial(mySerialRx, mySerialTx);
WiFiClient client;
HTTPClient http;

// Configuraciones Varias
unsigned long timerDelay = 0;
unsigned long lastTime = 0;
int httpResponseCode = 0;
String serverName = "NULL";

void setup()
{
  String ssid = "";
  String password = "";
  String serverURL = "";
  Serial.begin(115200);
  mySerial.begin(9600);
  delay(1000);
  ssid = receiveCommand("NETWORK", "UNSET");
  delay(1000);
  password = receiveCommand("PASSWORD", "NETWORK");
  delay(1000);
  serverURL = receiveCommand("SERVER", "PASSWORD");
  serverName = serverURL;
  delay(1000);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
}

void loop()
{
  String recivedData = receiveData();
  // Etapa2 - Envio
  // Send an HTTP POST request every timerDelay seconds
  if (((millis() - lastTime) > timerDelay) && (recivedData != "ERROR"))
  {
    // Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED)
    {
      // Your Domain name with URL path or IP address with path
      if (!http.begin(client, serverName))
      {
        delay(5000);
        return;
      }

      http.addHeader("Content-Type", "application/json");
      httpResponseCode = http.POST(recivedData);

      if (httpResponseCode > 0)
      {
        String response = http.getString();
      }
      // End the HTTP connection but keep the WiFiClient object alive
      http.end();
    }
    lastTime = millis();
  }
}

String receiveCommand(String command, String previusCommand)
{
  String okStr = "OK_";
  String recivedValue = "";
  String recivedData = "UNSET:NULL";
  String recivedCommand = "UNSET";
  int separationIndex = -1;
  while (recivedCommand != command)
  {
    while (!mySerial.available())
    {
    }
    mySerial.print(okStr + previusCommand);
    recivedData = mySerial.readString();
    separationIndex = recivedData.indexOf(":");
    if (separationIndex != -1)
    {
      recivedCommand = recivedData.substring(0, separationIndex);
      recivedValue = recivedData.substring(separationIndex + 1, recivedData.length());
    }
    separationIndex = -1;
  }
  mySerial.print(okStr + command);
  return recivedValue;
}

String receiveData()
{
  String recivedOrigin = "";
  String recivedSensor = "";
  String recivedValue = "";
  String recivedAux = "";
  String recivedData = "";
  String json = "ERROR";
  int separationIndex = -1;
  int secondSeparationIndex = -1;
  while (!mySerial.available())
  {
  }
  recivedData = mySerial.readString();
  separationIndex = recivedData.indexOf(":");
  if (separationIndex != -1)
  {
    recivedOrigin = recivedData.substring(0, separationIndex);
    recivedAux = recivedData.substring(separationIndex + 1, recivedData.length());
    secondSeparationIndex = recivedAux.indexOf(":");
    if (secondSeparationIndex != -1)
    {
      recivedSensor = recivedAux.substring(0, secondSeparationIndex);
      recivedValue = recivedAux.substring(secondSeparationIndex + 1, recivedAux.length());
      json = "{\"origen\":\"" + recivedOrigin + "\",\"sensor\":\"" + recivedSensor + "\",\"valor\":\"" + recivedValue + "\"}";
    }
  }

  return json;
}
