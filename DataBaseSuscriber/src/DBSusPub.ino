#include <Homie.h>
#include "FS.h"
#define ROWS 20
#define ID_SIZE 11
#include <SPI.h>
#include "MFRC522.h"

/* wiring the MFRC522 to ESP8266 (ESP-12)
RST     = D5
SDA(SS) = LedAzulD4
MOSI    = D13
MISO    = D12
SCK     = D14
GND     = GND
3.3V    = 3.3V
*/

#define RST_PIN	5  // RST-PIN fÃ¼r RC522 - RFID - SPI - Modul D5
#define SS_PIN	4  // SDA-PIN fÃ¼r RC522 - RFID - SPI - Modul LedAzulD4
#define LedAzulD4 2
#define LedRojoD3 0
#define BUTTON 16
#define BUTTON2 15

int ledPin = 13; // LED connected to digital pin 13
int inPin = 7;   // pushbutton connected to digital pin 7
int val = 0;     // variable to store the read value
int boton=0;
int boton2=0;

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance
HomieNode dataNode("data-base", "switch");
HomieNode verifyNode("verify","verify");
//HomieNode codeNode("code-rfid","code-rfid");

//Global flags for control loop
/******************************/
bool itsCorrect = false;
bool sendCode = false;
bool openDoor =false;

//Global variables
char *keys[ROWS];
char *code2sent = (char *) malloc(ID_SIZE);
unsigned int size=0;
bool AllowEntrance = false;

//bool readDataBase(char **keys){
bool readDataBase(){
  char *aux;
  int i=0;
  File f = SPIFFS.open("/data-base/data.txt", "r");
  if (!f){
    Serial.println("File can't be readed");
  }else{
    Serial.println("********************");
    Serial.println("***Base de datos****");
    while(f.available()){
      String line = f.readStringUntil('\n');
      keys[i] = (char *) malloc(ID_SIZE);
      strcpy(keys[i], line.c_str());
      keys[i][11]='\0';
      Serial.println(keys[i]);
      i++;
      size++;
    }
    Serial.println("********************");
  }
  f.close();
  return true;
}

bool addSentences(char *word){
  File f = SPIFFS.open("/data-base/data.txt", "a");
  keys[size] = (char *) malloc(ID_SIZE);
  word[11]='\0';
  strcpy(keys[size], word);
  f.println(word);
  f.close();
  size++;
  return true;
}

bool deleteSentances(char *word){
  int i , j=0;
  bool UpdIsNecesary = false;
  /* Compares the string with given word */

    for (i = 0;i < size; i++){
        if (strcmp(keys[i], word) == 0){
            for (j = i; j < size-1; j++){
              strcpy(keys[j], keys[j + 1]);
            }
            size--;
            free(keys[size]);
            UpdIsNecesary = true;
        }
    }

    if(UpdIsNecesary==true){
      File f = SPIFFS.open("/data-base/data.txt", "w");
      for(i=0; i < size; i++){
        keys[i][11]='\0'; //Necesario porque se escriben lineas de mas de 30bits.
        f.println(keys[i]);
      }
      f.close();
    }
    return true;
}

bool addOrDelete(String value){
  char* aux = NULL;
  char* dest = NULL;
  int i=0;
  dest = (char *) malloc(ID_SIZE+1);
  strcpy(dest, value.c_str());
  dest++;
  if(value[0] == '1'){
    Serial.println("Se agregara el codigo RFID: ");
    Serial.println(dest);
    if(addSentences(dest)){
      itsCorrect = true;
    }else{
      itsCorrect = false;
    }
  }else if(value[0] == '0'){
    Serial.println("Se eliminara el codigo RFID: ");
    Serial.println(dest);
    if(deleteSentances(dest)){
      itsCorrect = true;
    }else{
      itsCorrect = false;
    }
  }else{
    itsCorrect = false;
    Serial.println("No se pudo realizar correctamente la operacion");
  }
  Serial.println("**********************");
  Serial.println("Despues de actualizar:");
  for(i = 0; i < size; i++ ){
   Serial.println(keys[i]);
  }
  Serial.println("**********************");
  return true;
}

bool rfidHandler(String value) {
  addOrDelete(value);
  return true;
}

bool openHandler(String value) {
  Serial.println(value);
  if(strcmp(value.c_str(), "true") == 0){
    openDoor =true;
  }
  //addOrDelete(value);
  return true;
}

bool verifyHandler() {
  if (itsCorrect){
    Homie.setNodeProperty(verifyNode, "updated", "true");
    itsCorrect = false;
  }
  if (sendCode){
    Homie.setNodeProperty(verifyNode, "code", code2sent);
    sendCode = false;
  }
  return true;
}

void readAndOpen(){

  int i=0;
  bool AllowEntrance = false;

  //RFID Read code//
  boton = digitalRead(BUTTON);   // read the input pin
  boton2 = digitalRead(BUTTON2);
  digitalWrite(LedAzulD4, HIGH);

  if (boton == 1 || boton2 == 1 || openDoor == true) //Aqui van los UIDs autorizados
  {
    Serial.println("Acceso autorizado");
    Serial.println();

        //digitalWrite(RelayD8, HIGH);
        digitalWrite(LedAzulD4, LOW);
        delay(2000);
        digitalWrite(LedAzulD4, HIGH);
        openDoor = false;
        //digitalWrite(RelayD8, LOW);
  }


  // Busca nuevas tarjetas
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Selecciona una de las tarjetas
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  //Muestra el UID en el monitor serial.
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  Serial.print(content.substring(1).c_str());
  if(!sendCode){
    strcpy(code2sent, content.substring(1).c_str());
    sendCode = true;
  }
  for(i=0; i<size; i++){
    //Serial.println(strlen(content.substring(1).c_str()));
    //Serial.println(content.substring(1).c_str());
    //Serial.println(strlen(keys[i]));
    //Serial.println(keys[i]);
    if (strcmp(content.substring(1).c_str(), keys[i]) == 0){
      AllowEntrance = true;
      //Serial.println(strlen(content.substring(1).c_str()));
    }
  }
  if ( AllowEntrance == true
  ) //Aqui van los UIDs autorizados
  {
    Serial.println("Acceso autorizado");
    Serial.println();
    digitalWrite(LedAzulD4, LOW);
    delay(2000);
    digitalWrite(LedAzulD4, HIGH);
  }else{
    Serial.println(" Acceso negado");
    for(i=0; i<4;i++){
        digitalWrite(LedRojoD3, HIGH);
        delay(100);
        digitalWrite(LedRojoD3, LOW);
        delay(100);
    }
  }
}

void setup(){
  pinMode(BUTTON, INPUT);      // INPUT BUTTONS
  pinMode(BUTTON2, INPUT);
  pinMode(LedRojoD3, OUTPUT);
  pinMode(LedAzulD4, OUTPUT);

  Serial.begin(115200);
  int i=0;
  bool result = SPIFFS.begin();
  SPI.begin();	         // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522
  Serial.println("SPIFFS opened: " + result);
  readDataBase();
  delay(4000);
  Homie.setFirmware("databasesus", "1.0.0");
  dataNode.subscribe("rfidCode", rfidHandler);
  dataNode.subscribe("openSentence", openHandler);
  Homie.registerNode(dataNode);
  Homie.registerNode(verifyNode);
  Homie.setLoopFunction(verifyHandler);
  Homie.setup();
}

void loop(){
  readAndOpen();
  Homie.loop();

}
