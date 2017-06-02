// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "DHT.h"
#include <Homie.h>
//int len = snprintf(null, 0, "%f", amount);

#define DHTPIN D1     // what digital pin we're connected to
#define SIZE 10
#define SIZEJSON 30
#define DHTTYPE DHT11   // DHT 11 libraries

#define LedVerde D6

//Temperatue and humidity libraries
char* temperature = (char *) malloc(SIZE);
char* humidity = (char *) malloc(SIZE);
char* end = (char *) malloc(SIZEJSON);
bool openDoor = false;

//StaticJsonBuffer<200> jsonBuffer;
//JsonObject& root = jsonBuffer.createObject();
//String output;

DHT dht(DHTPIN, DHTTYPE);
HomieNode dhtNode("DHTsensor", "DHTsensor");

bool sendHandler() {
  Homie.setNodeProperty(dhtNode, "temp", end);
  //Homie.setNodeProperty(dhtNode, "humi", humidity);
  return true;
}

void setup() {
  Serial.begin(115200);
  Serial.println("DHTxx test!");
  Homie.setFirmware("databasesus", "1.0.0");
  Homie.registerNode(dhtNode);
  Homie.setLoopFunction(sendHandler);
  Homie.setup();

  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  dtostrf(h, 2, 0, humidity);
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  dtostrf(t, 2, 0, temperature);
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  strcpy(end, temperature);
  strcat(end, " ");
  strcat(end, humidity);

  //root["T"] = temperature;
  //root["H"] = humidity;

  //root.printTo(output);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");

  ///////////////////////
  Homie.loop();

}
