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
 
  analogWrite(BUZZER, 120);
  delay(50);
  analogWrite(BUZZER, 0);
  delay(50);
  analogWrite(BUZZER, 120);
  delay(50);
  analogWrite(BUZZER, 0);
  sendMessage("c2.b1start");

  while (client.connected()) {
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
  else if (params[1] == "c2") {
    
    if (params[2] == "b2start") {
      challenge2();
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



void challenge2() {
  // Straight 12 inches
  forward(2000);
  stop(500);


  // Rotate 180 degrees
  setMotor(1, 1, 60);
  setMotor(2, 0, 50);
  delay(2700);
  stop(500);

  // Back 3 inches
  backward(600);
  stop(500);

  // Turn Left
  left(2800);
  stop(500);

  // Turn Right
  right(2800);
  stop(500);

  // Go Striaght for a while
  forward(1800);
  stop(500);

  // Turn Right
  right(2000);
  stop(500);

  // Go a little forward
  forward(300);
  stop(500);


  // Turn right
  right(2000);
  stop(100);

}

void stop(millis) {
  setMotor(1, 0, 0);
  setMotor(2, 0, 0);
  delay(millis)
}

void right(millis) [
  setMotor(1, 0, 0);
  setMotor(2, 0, 60);
  delay(2000);
]

void left(millis) {
  setMotor(1, 0, 60);
  setMotor(2, 0, 0);
  delay(2000);
}

void forward(millis) {
  setMotor(1, 0, 60);
  setMotor(2, 0, 50);
  delay(millis);
}

void backward(millis) {
  setMotor(1, 1, 60);
  setMotor(2, 1, 60);
  delay(millis);
}