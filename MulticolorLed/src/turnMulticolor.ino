#include <Homie.h>
#define REDPIN D5
#define BLUEPIN D7
#define GREENPIN D6

const int PIN_RELAY = D6;

HomieNode lightNode("multicolor", "multicolor");

bool botonHandler(String value) {
  if (value == "red") {
    digitalWrite(REDPIN, HIGH);
    digitalWrite(BLUEPIN, LOW);
    digitalWrite(GREENPIN, LOW);
    Homie.setNodeProperty(lightNode, "on", "true"); // Update the state of the light
    Serial.println("Light is red");
  } else if (value == "green") {
    digitalWrite(REDPIN, LOW);
    digitalWrite(BLUEPIN, LOW);
    digitalWrite(GREENPIN, HIGH);
    Homie.setNodeProperty(lightNode, "on", "false");
    Serial.println("Light is green");
  } else if (value == "blue") {
    digitalWrite(REDPIN, LOW);
    digitalWrite(BLUEPIN, HIGH);
    digitalWrite(GREENPIN, LOW);
    Homie.setNodeProperty(lightNode, "on", "false");
    Serial.println("Light is blue");
  } else {
    return false;
  }

  return true;
}

void setup() {
  Serial.begin(115200);
  pinMode(REDPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  digitalWrite(REDPIN, LOW);
  digitalWrite(BLUEPIN, LOW);
  digitalWrite(GREENPIN, LOW);

  delay(2000);

  Homie.setFirmware("multicolor-led", "1.0.0");
  lightNode.subscribe("on", botonHandler);
  Homie.registerNode(lightNode);
  Homie.setup();
}

void loop() {
  Homie.loop();
}
