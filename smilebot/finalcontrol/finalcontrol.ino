/*
  Final Junior Design Program
*/
//#include <ArduinoHttpClient.h>
//#include <WiFi.h>
/* 
GPIO PARAMETERS 
*/

#define STATUSRED 13
#define STATUSBLUE 12
#define STATUSYELLOW 8

#define MOTOR1A 11
#define MOTOR1B 10

#define MOTOR2A 9
#define MOTOR2B 6

#define IRA 5
#define IRB 4

#define BUZZER 3

#define LEFT_TS 2
#define RIGHT_TS 1
#define HEAD_LIGHTS 0
#define TAIL_LIGHTS 7

//ANALOG PINS
#define BATT_STATUS A5
#define AMBIENT_LIGHT A4
#define COLOR_SENSE A3
#define COLL_DETECT A2


/* GLOBAL CONFIG - TO BE REPLACED */
int ambientThreshold = 95;
int buzzerTone = 128;
int battThreshold = 4;

int blackThreshold = 130;
int blueThreshold = 180;
int redThreshold = 220;
int yellowThreshold = 260;

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


/* WiFiClient wifi;
WebSocketClient client = WebSocketClient(wifi, serverAddress, port); */

/* int status = WL_IDLE_STATUS;
int count = 0; */

void setup() {
  //Initialize Ports 

  //STATUS LEDS
  pinMode(STATUSRED, OUTPUT);
  pinMode(STATUSBLUE, OUTPUT);
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
  /* while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
  }

  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip); */
  runDebugSequence();
}

void loop() {
  printDebug();
  /* Serial.println("starting WebSocket client");
  client.begin();
  client.beginMessage(TYPE_TEXT);
  client.print(clientID);
  client.endMessage();

  while (client.connected()) {
    if (count > 6000) {
      count = 0;
    }
    if (count % 100 == 0) {
      Serial.print("Sending hello ");
      Serial.println(count);

      // send a hello #
      client.beginMessage(TYPE_TEXT);
      client.print("hello ");
      client.print(count);
      client.endMessage();
    }
   
    // increment count for next message
    count++;

    // check if a message is available to be received
    int messageSize = client.parseMessage();
    if (messageSize > 0) {
      Serial.println("Received a message:");
      
      string message = client.readString();
      Serial.println(message);
      
      //if()
    }

    //publish status messages
    client.beginMessage(TYPE_TEXT);
    client.print(botID + ".sensor.ambient." + getAmbient());
    client.endMessage();

    client.beginMessage(TYPE_TEXT);
    client.print(botID + ".sensor.color.color." + getColorString());
    client.endMessage();

    client.beginMessage(TYPE_TEXT);
    client.print(botID + ".sensor.color.raw." + getColor());
    client.endMessage();

    client.beginMessage(TYPE_TEXT);
    client.print(botID + ".sensor.collision." + getCollisDetect());
    client.endMessage();

    client.beginMessage(TYPE_TEXT);
    client.print(botID + ".sensor.battery." + getBatVoltage());
    client.endMessage(); 
    // getColorString()
    // getAmbient()
    // getCollisDetect()
    // getBatVoltage()

    // wait 10ms
    delay(10);
  }
  */

  //Serial.println("disconnected");
 
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
  return sensorValue4V;
  //return sensorValue4V * (Vref / 1023.0);
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
  Serial.print("Detected Color: ");
  Serial.print(getColorString());
  /* if(getColor() == 0){
    Serial.print("Black");
  }else if(getColor() == 1){
    Serial.print("Blue");
  }else if(getColor() == 2){
    Serial.print("Red");
  }else if(getColor() == 3){
    Serial.print("Yellow");
  }else{
    Serial.print("Unknown");
  } */

  Serial.print("Ambient Light: \n");
  Serial.print(getAmbient());
  
  Serial.print("Collision Detection: \n");
  Serial.print(getCollisDetect());

  Serial.print("Battery voltage\n");
  Serial.print(getBatVoltage());
  delay(1000);

}

void runDebugSequence(){
  //turn on head lights and tail lights
  digitalWrite(HEAD_LIGHTS, HIGH);
  digitalWrite(TAIL_LIGHTS, HIGH);
  setMotor(1, 0, 255);
  setMotor(2, 0, 255);
  

  //BLINK ALL STATUS LEDS 3 times
  for(int i = 0; i < 3; i++){
    digitalWrite(STATUSRED, HIGH);
    digitalWrite(STATUSBLUE, HIGH);
    digitalWrite(STATUSYELLOW, HIGH);
    setBuzzer(true);
    delay(500);
    digitalWrite(STATUSRED, LOW);
    digitalWrite(STATUSBLUE, LOW);
    digitalWrite(STATUSYELLOW, LOW);
    setBuzzer(false);
    delay(500);
  }
  

  //Blink left ts
  for(int i = 0; i < 3; i++){
    digitalWrite(LEFT_TS, HIGH);
    delay(100);
    digitalWrite(LEFT_TS, LOW);
    delay(100);
  }

  //Blink right ts
  for(int i = 0; i < 3; i++){
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
    digitalWrite(STATUSRED, HIGH);
    digitalWrite(STATUSBLUE, HIGH);
    delay(100);
    digitalWrite(STATUSRED, LOW);
    digitalWrite(STATUSBLUE, LOW);
    delay(100);
  }
  if(roll == 0){

  }else if(roll == 1){

  }
}