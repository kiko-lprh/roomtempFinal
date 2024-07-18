#include "DHT.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>

#define DPIN 2  // Sensor data connected to pin 2
#define DTYPE DHT11
#define RELAY_PIN 26

const float temp_treshold = 79.0; // degrees Fahrenheit *
bool relayStatus = false;
//bool StatusLock = false;          // if true, relay doesn't open/close with temperature
float lastTemp;

String URL = "PLACEHOLDER";
String fanURL ="PLACEHOLDER";

const char* ssid = "PLACEHOLDER";
const char* pass = "PLACEHOLDER";

WebServer server(80);

DHT dht(DPIN, DTYPE);  //sensor object

void wifi_connect() {
  Serial.print("Connecting to WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  unsigned long startAttempt = millis();

  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 20000) {
    Serial.print(".");
    delay(100);
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connection failed!");
  } else {
    Serial.println("Connected!");
  }
}


void handleOptions() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.send(204); // No content
}

void handleOpenRelay() {
  Serial.println("Remote");
  openRelay();
  server.send(200, "text/plain", "Relay opened");
}

void handleCloseRelay() {
  Serial.println("Remote");
  closeRelay();
  server.send(200, "text/plain", "Relay closed");
}


void setup() {
  Serial.begin(9600);
  wifi_connect();
  dht.begin();
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  server.on("/openRelay", handleOpenRelay);     // executes handleOpenRelay remotely
  server.on("/closeRelay", handleCloseRelay);   // executes handleCloseRelay remotely
  server.on("/options", handleOptions);         // Handle CORS

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  
  server.handleClient(); // To handle client requests
  
  float cTemp = dht.readTemperature();      //temperature in celcius
  float fTemp = dht.readTemperature(true);  //temperature in farenheith
  float humidity = dht.readHumidity();      // humidity %


  Serial.print("Temperature (Celcius): ");
  Serial.print(cTemp);
  Serial.print(", ");
  Serial.print("Temperature (Fahrenheit): ");
  Serial.print(fTemp);
  Serial.print(", ");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(", ");
  Serial.print("Status: ");
  Serial.print(relayStatus);
  Serial.println();


  // Control relay based on temperature threshold
  if (fTemp > temp_treshold && !relayStatus) {
    openRelay();
  } else if (fTemp <= temp_treshold && relayStatus) {
    closeRelay();
  }


  if (fTemp != lastTemp) {

    String postData = "celcius=" + String(cTemp) + "&fahrenheit=" + String(fTemp) + "&humidity=" + String(humidity);

    HTTPClient http;
    http.begin(URL);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    int httpCode = http.POST(postData);
    String payload = http.getString();
    http.end();
  }

  lastTemp = fTemp;
  delay(1000);
}


void openRelay() {
  
  digitalWrite(RELAY_PIN, HIGH);  // HIGH = on
  relayStatus = true;             // on
  Serial.println("Relay turned on");

  String POSTfanON = "status=F_ON";
  HTTPClient http;

  http.begin(fanURL);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpCode = http.POST(POSTfanON);
  Serial.print("HTTP POST fan ON status: ");
  Serial.println(httpCode);
 
  if (httpCode > 0) {
    String payload = http.getString();
    Serial.println("Response: " + payload);
  } else {
    Serial.printf("Error: %s\n", http.errorToString(httpCode).c_str());
  }
  
  http.end();  // Free resources
}

void closeRelay() {
  digitalWrite(RELAY_PIN, LOW);  // LOW = off
  relayStatus = false;           // off
  Serial.println("Relay turned off");

  String POSTfanOFF = "status=F_OFF";
  HTTPClient http;

  http.begin(fanURL);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpCode = http.POST(POSTfanOFF);
  Serial.print("HTTP POST fan OFF status: ");
  Serial.println(httpCode);
  
  if (httpCode > 0) {
    String payload = http.getString();
    Serial.println("Response: " + payload);
  } else {
    Serial.printf("Error: %s\n", http.errorToString(httpCode).c_str());
  }
  
  http.end();  // Free resources
}
