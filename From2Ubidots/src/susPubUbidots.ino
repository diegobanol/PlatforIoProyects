#include <Homie.h>

#define BUTTON 16
#define BUTTON2 15

HomieNode dataNode("catchAction", "switch");
bool sendCode = false;

int boton=0;
int boton2=0;

bool actionHandler(){
  if (sendCode){
    Homie.setNodeProperty(dataNode, "code", "{\"variable\": true}"); //Arduino Json
    sendCode = false;
  }
  return true;
}

void setup(){
  pinMode(BUTTON, INPUT);      // INPUT BUTTONS
  pinMode(BUTTON2, INPUT);
  Homie.setFirmware("UbidotsPubSus", "1.0.0");
  Homie.registerNode(dataNode);
  Homie.setLoopFunction(actionHandler);
  //Homie.setLoopFunction(verifyHandler);
  Homie.setup();
}

void loop(){
  boton = digitalRead(BUTTON);   // read the input pin
  boton2 = digitalRead(BUTTON2);
  if (boton == 1 || boton2 == 1) //Aqui van los UIDs autorizados
  {
    Serial.println("Acceso autorizado");
    sendCode = true;
    //digitalWrite(RelayD8, LOW);
  }
  Homie.loop();

}
