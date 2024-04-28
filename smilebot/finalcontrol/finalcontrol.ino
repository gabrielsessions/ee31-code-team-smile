enum BotState {
  C1_WAIT, 
  C1_DIAGNOSTIC, 
  C1_FIND_RED, 
  C1_FOLLOW_RED, 
  C1_FIND_BLUE,
  C1_FOLLOW_BLUE,
  C1_FIND_YELLOW, 
  C1_RETURN_START, 
  C1_END
};

/*
  Final Junior Design Program
*/
#include <ArduinoHttpClient.h>
#include <WiFi.h>

/* 
GPIO PARAMETERS 
*/

#define STATUSORANGE 8
#define STATUSGREEN 7
#define STATUSYELLOW 12

#define MOTOR1A 10
#define MOTOR1B 5

#define MOTOR2A 9
#define MOTOR2B 6

//#define IRA 5
#define IRB 4

#define BUZZER 3

#define LEFT_TS 1
#define RIGHT_TS 0
#define HEAD_LIGHTS 2
#define TAIL_LIGHTS 13

//ANALOG PINS
#define BATT_STATUS A3
#define AMBIENT_LIGHT A2
#define COLOR_SENSE A5
#define COLL_DETECT A4


/* GLOBAL CONFIG - TO BE REPLACED */
int ambientThreshold = 115;
int buzzerTone = 128;
int battThreshold = 4;

int blackThreshold = 130;
int blueThreshold = 150;
int redThreshold = 195;
int yellowThreshold = 270;

int turn_90_time = 1000;

const float R1 = 12000.0; // 12kΩ resistor
const float R2 = 10000.0; // 10kΩ resistor
const float Vref = 4.8; // Reference voltage (5V for most Arduinos)
const float ADCmax = 1023.0; // Max ADC value for a 10-bit ADC

// END CONFIG

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
/////// WiFi Settings ///////
char ssid[] = "junior";
char pass[] = "designdesign";

char serverAddress[] = "34.173.203.110";  // server address
int port = 80;
String clientID = "4A9EDB0160D5";
String botID = clientID;


WiFiClient wifi;
WebSocketClient client = WebSocketClient(wifi, serverAddress, port); 

int status = WL_IDLE_STATUS;
int count = 0; 

void setup() {
  //Initialize Ports 

  //STATUS LEDS
  pinMode(STATUSORANGE, OUTPUT);
  pinMode(STATUSGREEN, OUTPUT);
  pinMode(STATUSYELLOW, OUTPUT);

  // COMM LEDS
  pinMode(LEFT_TS, OUTPUT);
  pinMode(RIGHT_TS, OUTPUT);
  pinMode(HEAD_LIGHTS, OUTPUT);
  pinMode(TAIL_LIGHTS, OUTPUT);

  pinMode(MOTOR1A, OUTPUT);
  pinMode(MOTOR1B, OUTPUT);

  pinMode(MOTOR2A, OUTPUT);
  pinMode(MOTOR2B, OUTPUT);

  /* pinMode(IRA, OUTPUT);
  pinMode(IRB, INPUT); */

  pinMode(BUZZER, OUTPUT);



  pinMode(BATT_STATUS, INPUT);
  pinMode(AMBIENT_LIGHT, INPUT);
  pinMode(COLOR_SENSE, INPUT);
  pinMode(COLL_DETECT, INPUT);
  Serial.begin(9600);
  
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
    delay(500);
  }

  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID()); */

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);  
  //runDebugSequence();

  //Serial.println(ip); 
  /*
  //runDebugSequence();
  digitalWrite(HEAD_LIGHTS, HIGH);
  digitalWrite(STATUSGREEN, HIGH);
  digitalWrite(TAIL_LIGHTS, HIGH);
  setMotor(1, 1, 255);
  setMotor(2, 0, 255);
  challenge1();
  */
}

void loop() {
  /*
  printDebug();
  Serial.println("First TEST: ");
  analogWrite(MOTOR1A, 70);
  digitalWrite(MOTOR1B, LOW);
  delay(3000);
  Serial.println("Second TEST: ");
  analogWrite(MOTOR1B, 70);
  digitalWrite(MOTOR1A, LOW);
  delay(3000);
  */

  
  
 
}

void processMessage(String message) {
  int prevPeriod = 0;
  int nextPeriod = message.indexOf(".");
  String clientID = message.substring(prevPeriod, nextPeriod);
  if (clientID.indexOf("4A9EDB0160D5") != -1 || clientID.indexOf("DCF2BCAB6F0B") != -1) {
    prevPeriod = nextPeriod;
    nextPeriod = message.substring(nextPeriod + 1).indexOf(".") == 1 ? message.substring(nextPeriod + 1).indexOf(".") : message.length();
    Serial.print(message.substring(prevPeriod, nextPeriod));

  }

}

/*
  setMotor(MOTOR #, speed)
  direction 0 = forwards
            1 = backward
            2 = stop
*/
void setMotor(uint8_t num, uint8_t direction, uint8_t speed){
  if(num == 1){
    if(direction == 0){
      analogWrite(MOTOR1A, speed);
      digitalWrite(MOTOR1B, 0);
    }else if(direction == 1){
      analogWrite(MOTOR1B, speed);
      digitalWrite(MOTOR1A, 0);
    }else if(direction == 2){
      digitalWrite(MOTOR1B, 0);
      digitalWrite(MOTOR1A, 0);
    }
  }else if(num == 2){
    if(direction == 0){
      analogWrite(MOTOR2A, speed);
      digitalWrite(MOTOR2B, 0);
    }else if(direction == 1){
      analogWrite(MOTOR2B, speed);
      digitalWrite(MOTOR2A, 0);
    }else if(direction == 2){
      digitalWrite(MOTOR2B, 0);
      digitalWrite(MOTOR2A, 0);
    }
  }
}

void turnLeft(int degrees){
  setMotor(1, 0, 255);
  setMotor(2, 1, 255);
  delay(degrees);
  setMotor(1, 2, 0);
  setMotor(2, 2, 0);
}

void turnRight(int degrees){
  setMotor(2, 0, 255);
  setMotor(1, 1, 255);
  delay(degrees);
  setMotor(1, 2, 0);
  setMotor(2, 2, 0);
}

int getAmbient(){
  return analogRead(AMBIENT_LIGHT);
}

bool isDay(){
  return getAmbient() < ambientThreshold;
}

void setBuzzer(bool on){
  if(on){
    analogWrite(BUZZER, buzzerTone);
  }else{
    digitalWrite(BUZZER, 0);
  }
}

int getBatVoltage(){
  int sensorValue4V = analogRead(BATT_STATUS);
  //return sensorValue4V;
  return sensorValue4V * (Vref / 1023.0);
}

bool isBatLow(){
  return getBatVoltage() < battThreshold;
}

int getColorRaw(){
  return analogRead(COLOR_SENSE);
}

int getCollisDetect(){
  return analogRead(COLL_DETECT);
}

/* 
 0 -> black
 1 -> blue
 2 -> red
 3 -> yellow
 4 -> unknown
 */
int getColor(){
  int val = getColorRaw();
  if (val < blackThreshold) {
    return 0;
  } 
  else if (val < blueThreshold) {
    return 1;
  }
  else if (val < redThreshold) {
    return 2;
  }
  else if (val < yellowThreshold) {
    return 3;
  }
  else {
    return 4;
  }
}

String getColorString(){
  int val = getColor();
  if(getColor() == 0){
    return("Black");
  }else if(getColor() == 1){
    return("Blue");
  }else if(getColor() == 2){
    return("Red");
  }else if(getColor() == 3){
    return("Yellow");
  }else{
    return("Unknown");
  }
}


void printDebug(){
  /* Serial.print("RAW Color: ");
  Serial.println(getColorRaw());
  Serial.print("Guess Color: ");
  Serial.println(getColorString());
 */

  /* Serial.println("Ambient Light:");
  Serial.println(getAmbient()); */
  
  Serial.println("Collision Detection:");
  Serial.println(getCollisDetect());
  //Serial.println(analogRead(A0));
  /* Serial.println("Battery voltage");
  Serial.println(getBatVoltage()); */
  

}

void runDebugSequence(){
  analogWrite(BUZZER, 100);
  delay(50);
  analogWrite(BUZZER, 120);
  delay(50);
  analogWrite(BUZZER, 80);
  delay(50);
  analogWrite(BUZZER, 120);
  delay(100);
  analogWrite(BUZZER, 150);
  delay(100);
  analogWrite(BUZZER, 160);
  delay(100);
  analogWrite(BUZZER, 170);
  delay(100);
  analogWrite(BUZZER, 180);
  delay(1000);
  //turn on head lights and tail lights
  digitalWrite(HEAD_LIGHTS, HIGH);
  digitalWrite(TAIL_LIGHTS, HIGH);
  setMotor(1, 0, 255);
  setMotor(2, 0, 255);
  

  //BLINK ALL STATUS LEDS 3 times
  for(int i = 0; i < 3; i++){
    digitalWrite(STATUSORANGE, HIGH);
    digitalWrite(STATUSGREEN, HIGH);
    digitalWrite(STATUSYELLOW, HIGH);
    setBuzzer(true);
    delay(500);
    digitalWrite(STATUSORANGE, LOW);
    digitalWrite(STATUSGREEN, LOW);
    digitalWrite(STATUSYELLOW, LOW);
    setBuzzer(false);
    delay(500);
  }
  

  //Blink left ts
  for(int i = 0; i < 10; i++){
    digitalWrite(LEFT_TS, HIGH);
    delay(100);
    digitalWrite(LEFT_TS, LOW);
    delay(100);
  }

  //Blink right ts
  for(int i = 0; i < 10; i++){
    digitalWrite(RIGHT_TS, HIGH);
    delay(100);
    digitalWrite(RIGHT_TS, LOW);
    delay(100);
  }

  setBuzzer(false);
  setMotor(1, 0, 0);
  setMotor(2, 0, 0);
  //turn off head lights and tail lights
  digitalWrite(HEAD_LIGHTS, LOW);
  digitalWrite(TAIL_LIGHTS, LOW);
}

/* 
roll 0 = Bot 1 routine
roll 1 = Bot 2 routine
 */
void routineChallenge1(int roll){
  for(int i = 0; i < 3; i++){
    digitalWrite(STATUSORANGE, HIGH);
    digitalWrite(STATUSGREEN, HIGH);
    delay(100);
    digitalWrite(STATUSORANGE, LOW);
    digitalWrite(STATUSGREEN, LOW);
    delay(100);
  }
  if(roll == 0){

  }else if(roll == 1){

  }
}

void challenge1(){
  Serial.println("STARTING CHALLENGE 5");
  /* setMotor(1, 1, 100);
  setMotor(2, 0, 95); */
  setMotor(1, 1, 60);
  setMotor(2, 0, 60);
  delay(20000);
  setMotor(1, 1, 0);
  setMotor(2, 0, 0);
  Serial.println("CHALLENGE 5 DONE");

}


void splitStringByPeriod(char inputStr[], char *parts[], int &partCount) {
    const char *delimiter = ".";
    char *token;
    int i = 0;

    // Get the first token
    token = strtok(inputStr, delimiter);

    // Walk through other tokens
    while (token != NULL) {
        parts[i++] = token;  // store the token in the array
        token = strtok(NULL, delimiter);  // get next token
    }

    // Update the part count
    partCount = i;
}