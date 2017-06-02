#include <Homie.h>

#define LedAzulD4 2

void onHomieEvent(HomieEvent event) {
  switch(event) {
    case HOMIE_CONFIGURATION_MODE:
      digitalWrite(LedAzulD4, HIGH);
      Serial.println("Configuration mode started");
      break;
    case HOMIE_NORMAL_MODE:
      digitalWrite(LedAzulD4, HIGH);
      Serial.println("Normal mode started");
      break;
    case HOMIE_OTA_MODE:
      digitalWrite(LedAzulD4, HIGH);
      Serial.println("OTA mode started");
      break;
    case HOMIE_ABOUT_TO_RESET:
      digitalWrite(LedAzulD4, HIGH);
      Serial.println("About to reset");
      break;
    case HOMIE_WIFI_CONNECTED:
      digitalWrite(LedAzulD4, HIGH);
      Serial.println("Wi-Fi connected");
      break;
    case HOMIE_WIFI_DISCONNECTED:
      digitalWrite(LedAzulD4, HIGH);
      Serial.println("Wi-Fi disconnected");
      break;
    case HOMIE_MQTT_CONNECTED:
      digitalWrite(LedAzulD4, HIGH);
      Serial.println("MQTT connected");
      break;
    case HOMIE_MQTT_DISCONNECTED:
      digitalWrite(LedAzulD4, HIGH);
      Serial.println("MQTT disconnected");
      Serial.println("No vamos a prender el led");
      break;
  }
}

void setup() {
  pinMode(LedAzulD4, OUTPUT);
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Homie.enableLogging(false);
  Homie.setFirmware("events-test", "1.0.0");
  Homie.onEvent(onHomieEvent);
  Homie.setup();
}

void loop() {
  Homie.loop();
}
