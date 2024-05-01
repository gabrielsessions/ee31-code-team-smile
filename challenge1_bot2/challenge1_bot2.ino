// Challenge 1 Part 1 Code
enum BotState {
  C1_WAIT, 
  C1_DIAGNOSTIC, 
  C1_DRIVE_TO_WALL,
  C1_FIND_RED, 
  C1_WAIT_FOR_BOT1_1,
  C1_FOLLOW_RED, 
  C1_FIND_BLUE,
  C1_FOLLOW_BLUE,
  C1_FIND_YELLOW, 
  C1_WAIT_FOR_BOT1_2,
  C1_FOLLOW_YELLOW,
  C1_RETURN_START, 
  C1_END
};

BotState state = C1_WAIT;
bool enteringState = true;
int debugTicker = 0;
int motorSpeeds[] = {0, 0};
int colorCount = 0;

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

#define BLINK_RATE 150


/* GLOBAL CONFIG - TO BE REPLACED */
int ambientThreshold = 115;
int buzzerTone = 128;
int battThreshold = 4;

int blackThreshold = 124;
int blueThreshold = 160;
int redThreshold = 205;
int yellowThreshold = 300;

int turn_90_time = 1000;

const float R1 = 12000.0; // 12kΩ resistor
const float R2 = 10000.0; // 10kΩ resistor
const float Vref = 4.8; // Reference voltage (5V for most Arduinos)
const float ADCmax = 1023.0; // Max ADC value for a 10-bit ADC

const int forwardLeftSpeed = 60;
const int forwardRightSpeed = 50;
const int collisDetectThreshold = 60;

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
  // put your setup code here, to run once:
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
  
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
    delay(500);
  }

  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID()); 

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

}

 
void sendMessage(String message) {
  client.beginMessage(TYPE_TEXT);
  client.print(message);
  client.endMessage();
}

void loop() {
  Serial.println("starting WebSocket client");
  client.begin();
  sendMessage(clientID);
  while (client.connected()) {

    stateAction();

    /*
    debugTicker = (debugTicker + 1) % 100;
    if (debugTicker == 0) {
      sendMessage(getDebug());
    }
    */
    

    // check if a message is available to be received
    int messageSize = client.parseMessage();

    if (messageSize > 0) {
      Serial.println("Received a message:");
      String message = client.readString();
      processMessage(message);
    }

    // wait 10ms
    delay(10);
  }


}

void processMessage(String message) {
  String params[10];
  int paramCount = 0;
  splitString(message, params, paramCount);
  String incomingBotID = params[0];
  if (incomingBotID.indexOf("4A9EDB0160D5") == -1 && incomingBotID.indexOf("DCF2BCAB6F0B") == -1) {
    //Serial.println("Invalid ID");
    return;
  }
  Serial.println(params[1]);
  if (params[1] == "motors") {
    int leftSpeed = params[2].toInt();
    int rightSpeed = params[3].toInt();
    motorSpeeds[0] = leftSpeed;
    motorSpeeds[1] = rightSpeed;
    if (leftSpeed < 0) {
      setMotor(1, 1, leftSpeed*-1);
    }
    else {
      setMotor(1, 0, leftSpeed);
    }
    if (rightSpeed < 0) {
      setMotor(2, 1, rightSpeed*-1);
    }
    else {
      setMotor(2, 0, rightSpeed);
    }
  }
  else if (params[1] == "buzzer") {
    analogWrite(BUZZER, 128);
    delay(50);
    analogWrite(BUZZER, 0);
  }
  else if (params[1] == "c1") {
    if (params[2] == "b2start") {
      analogWrite(BUZZER, 128);
      delay(50);
      analogWrite(BUZZER, 0);
       delay(50);
      analogWrite(BUZZER, 128);
      delay(50);
      analogWrite(BUZZER, 0);
      sendMessage("c1.b1followred");
      delay(5000);
      enteringState = true;
      state = C1_DRIVE_TO_WALL;
      
    }
    if (params[2] == "b2followblue") {
      
      delay(1000);
      sendMessage("c1.b1followyellow");
      state = C1_FOLLOW_BLUE;
      enteringState = true;
      
      
    }
    if (params[2] == "b2followyellow") {
      enteringState = true;
      state = C1_FIND_YELLOW;
    }
  }


}

void splitString(String s, String output[], int &count) {
  count = 0; // Initialize count of elements
  int startPos = 0; // Starting position for search
  int endPos = 0; // End position for each found token
  
  // Continue to search for the delimiter until the end of the string
  while ((endPos = s.indexOf(".", startPos)) != -1) {
    output[count++] = s.substring(startPos, endPos); // Extract substring and store in the array
    startPos = endPos + 1; // Move start position past the last delimiter
  }
  
  // Add the last part of the string after the last delimiter
  output[count++] = s.substring(startPos);
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

void stateAction() {
  if (state == C1_WAIT) {
    digitalWrite(TAIL_LIGHTS, HIGH);
    delay(250);
    digitalWrite(TAIL_LIGHTS, LOW);
    delay(250);
    state = C1_DIAGNOSTIC;
    return;
  }
  if (state == C1_DIAGNOSTIC && enteringState) {
    analogWrite(BUZZER, 100);
    delay(50);
    analogWrite(BUZZER, 0);
    delay(50);
    analogWrite(BUZZER, 100);
    delay(50);
    analogWrite(BUZZER, 0);
    delay(50);
    analogWrite(BUZZER, 100);
    delay(50);
    analogWrite(BUZZER, 0);
    for (int i = 0; i < 3; i++) {
      digitalWrite(STATUSORANGE, HIGH);
      digitalWrite(STATUSGREEN, HIGH);
      digitalWrite(STATUSYELLOW, HIGH);
      delay(BLINK_RATE);
      digitalWrite(STATUSORANGE, LOW);
      digitalWrite(STATUSGREEN, LOW);
      digitalWrite(STATUSYELLOW, LOW);
      delay(BLINK_RATE);
    }
    //state = C1_DRIVE_TO_WALL;
    enteringState = false;
    return;
  }
  if (state == C1_DRIVE_TO_WALL) {
    if (enteringState) {
      motorSpeeds[0] = forwardLeftSpeed;
      motorSpeeds[1] = forwardRightSpeed;
      setMotor(1, 0, forwardLeftSpeed);
      setMotor(2, 0, forwardRightSpeed);
      enteringState = false;    
    }
    if (getCollisDetect() < collisDetectThreshold) {
      enteringState = true;
      setMotor(1, 0, 0);
      setMotor(2, 0, 0);
      state = C1_FIND_BLUE;
      return;
    }
    if (motorSpeeds[0] == 0 && motorSpeeds[1] == 0) {
      enteringState = true;
      state = C1_FIND_BLUE;
    }

  }

  if (state == C1_FIND_BLUE) {
    if (enteringState) {
      setMotor(1, 1, forwardLeftSpeed);
      setMotor(2, 1, forwardRightSpeed);
      delay(1000);
      setMotor(1, 0, forwardLeftSpeed);
      setMotor(2, 1, forwardLeftSpeed);
      delay(1600);
      setMotor(1, 0, forwardLeftSpeed);
      setMotor(2, 0, forwardRightSpeed);
      motorSpeeds[0] = 0;
      motorSpeeds[1] = 0;
      enteringState = false;
      return;
    }

    if (getColorString() == "Blue") {
      colorCount++;
      // Need to make sure color readings are consistent
      if (colorCount > 40) {
        setMotor(1, 0, 0);
        setMotor(2, 0, forwardRightSpeed);
        delay(1500);
        setMotor(2, 0, 0);
        analogWrite(BUZZER, 128);
        delay(50);
        analogWrite(BUZZER, 0);
        delay(50);
        analogWrite(BUZZER, 128);
        delay(50);
        analogWrite(BUZZER, 0);
        motorSpeeds[0] = 0;
        motorSpeeds[1] = 0;
        state = C1_WAIT_FOR_BOT1_1;
        sendMessage("c1.b1followyellow");
      }
      
    }
    return;
  }

  if (state == C1_FOLLOW_BLUE) {
    if (enteringState) {
      setMotor(1, 0, forwardLeftSpeed);
      setMotor(2, 0, 0);
      motorSpeeds[0] = forwardLeftSpeed;
      motorSpeeds[1] = 0;
      enteringState = false;
      return;
    }
    if (motorSpeeds[0] == 0 && motorSpeeds[1] == 0 || getCollisDetect() < collisDetectThreshold) {
      analogWrite(BUZZER, 128);
      delay(50);
      analogWrite(BUZZER, 0);
      delay(50);
      analogWrite(BUZZER, 128);
      delay(50);
      analogWrite(BUZZER, 0);
      colorCount = 0;
      setMotor(1, 0, 0);
      setMotor(2, 0, 0);
      motorSpeeds[0] = 0;
      motorSpeeds[1] = 0;
      //state = C1_FIND_YELLOW;
      //enteringState = true;
     
      state = C1_WAIT_FOR_BOT1_2;
      return;

    }

    if (getColorString() == "Blue" && motorSpeeds[1] == 0) {
      setMotor(1, 0, 0);
      setMotor(2, 0, forwardRightSpeed);
      motorSpeeds[0] = 0;
      motorSpeeds[1] = forwardRightSpeed;

    }
    else if (getColorString() != "Blue" && motorSpeeds[0] == 0) {
      setMotor(1, 0, forwardLeftSpeed*1.5);
      setMotor(2, 0, 0);
      motorSpeeds[0] = forwardLeftSpeed;
      motorSpeeds[1] = 0;
    }
    return;
  }

  if (state == C1_FIND_YELLOW) {
    if (enteringState) {
      setMotor(1, 1, forwardLeftSpeed);
      setMotor(2, 1, forwardRightSpeed);
      delay(1000);
      setMotor(1, 0, 0);
      setMotor(2, 0, forwardRightSpeed);
      delay(2500);
      setMotor(1, 0, forwardLeftSpeed);
      setMotor(2, 0, forwardRightSpeed);
      enteringState = false;

      return;
    }
    
    if (getColorString() == "Yellow") {
      colorCount++;
      // Need to make sure color readings are consistent
      if (colorCount > 11) {
        setMotor(1, 0, 0);
        setMotor(2, 0, forwardRightSpeed);
        delay(1500);
        setMotor(2, 0, 0);
        analogWrite(BUZZER, 128);
        delay(50);
        analogWrite(BUZZER, 0);
        delay(50);
        analogWrite(BUZZER, 128);
        delay(50);
        analogWrite(BUZZER, 0);
        motorSpeeds[0] = 0;
        motorSpeeds[1] = 0;
        state = C1_FOLLOW_YELLOW;
        enteringState = true;
        return;
        
      }
      
    }
    return;

  }

  if (state == C1_FOLLOW_YELLOW) {
    if (enteringState) {
      setMotor(1, 0, forwardLeftSpeed);
      setMotor(2, 0, 0);
      motorSpeeds[0] = forwardLeftSpeed;
      motorSpeeds[1] = 0;
      enteringState = false;
      return;
    }
    else if (getCollisDetect() < collisDetectThreshold || motorSpeeds[0] == 0 && motorSpeeds[1] == 0) {
      analogWrite(BUZZER, 128);
      delay(50);
      analogWrite(BUZZER, 0);
      delay(50);
      analogWrite(BUZZER, 128);
      delay(50);
      analogWrite(BUZZER, 0);
      colorCount = 0;
      state = C1_RETURN_START;
      enteringState = true;
      return;

    }
    if (getColorString() == "Yellow" && motorSpeeds[0] == 0) {
      setMotor(1, 0, forwardLeftSpeed);
      setMotor(2, 0, 0);
      motorSpeeds[0] = forwardLeftSpeed;
      motorSpeeds[1] = 0;
      return;

    }
    else if (getColorString() != "Yellow" && motorSpeeds[1] == 0) {
      setMotor(1, 0, 0);
      setMotor(2, 0, forwardRightSpeed);
      motorSpeeds[0] = 0;
      motorSpeeds[1] = forwardRightSpeed;
    }
    return;
  }

  if (state == C1_RETURN_START) {
    
    if (enteringState) {
      setMotor(1, 1, forwardLeftSpeed);
      setMotor(2, 1, forwardRightSpeed);
      delay(900);
      setMotor(1, 0, 0);
      setMotor(2, 0, forwardRightSpeed);
      delay(1500);
      setMotor(1, 0, forwardLeftSpeed);
      setMotor(2, 0, forwardRightSpeed);
      enteringState = false;
      return;
    }

    if (getCollisDetect() < collisDetectThreshold || motorSpeeds[0] == 0 && motorSpeeds[1] == 0) {
      setMotor(1, 0, 0);
      setMotor(2, 0, 0);
      sendMessage("c1.b2finished");
      state = C1_END;
    }
    
  }


}

bool isDay(){
  return analogRead(AMBIENT_LIGHT) < ambientThreshold;
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

String getDebug() {
  return "state=" + String(state) + ".collision=" + String(getCollisDetect()) 
    + ".color=" + getColorString() + "(" + getColorRaw() + ").day=" + String(analogRead(AMBIENT_LIGHT)) 
    + ".voltage=" + String(analogRead(BATT_STATUS) * (Vref / 1023.0));
}


