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

void setup()
{
  String ssid = "";
  String password = "";
  Serial.begin(115200);
  mySerial.begin(9600);
  delay(100);
  Serial.println("Iniciando Configuracion");
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

String receiveCommand(String command, String previusCommand)
{
  String recivedValue = "";
  String recivedData = "UNSET:NULL";
  String recivedCommand = "UNSET";
  Serial.println("Esperando " + command);
  int separationIndex = -1;
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