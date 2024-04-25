const int grn_led = 7;
const int red_led = 8;
const int blue_led = 12;
const int orng_led = 8;

const int ylw_led = 12;
const int leftF = 11;
const int leftB = 10;
const int headlights = 72;
const int brakes = 13;
const int rightF = 9;
const int rightB = 6;
//const int start_button = 11;
const int left_sig = 1;
const int right_sig = 0;
const int color_sensor = A5;
const int collision_sensor = A4;
const int battery_sensor = A3;
const int day_night_sensor = A2;

// universal constants
const float offset = 0.98;
const int turnSpeed = 92;
const float ADC_RESOLUTION = 5.0/1024;
const float BATTERY_FACTOR = 2.4;
const int BLINK_RATE = 150;
const int BREAK_TIME = 100;

// color thresholds
const float blue_threshold_min = 0.7;
const float blue_threshold_max = 1.1;

const float yellow_threshold_min = 2.2;
const float yellow_threshold_max = 2.6;

const float red_threshold_min = 1.6;
const float red_threshold_max = 2.0;

const float collision_threshold = 3.0;

// values for reading
volatile float color = 0;
volatile int edges_found = 0;
volatile float collide = 0;
volatile float bat_lvl = 0;

// state initialization
volatile int state = 0;

/* WiFi and Server Declaration */
// Team Smile ID (bot 2): 4A9EDB0160D5
const String TeamSmileID = "4A9EDB0160D5";
// Team Infra Red ID (bot 1): DCF2BCAB6F0B
const String TeamInfraRedID = "DCF2BCAB6F0B";
#include <WiFi.h>
#include <ArduinoHttpClient.h>

char ssid[] = "junior";
char pass[] = "designdesign";

char serverAddress[] = "34.173.203.110";
int port = 80;

WiFiClient wifi;
WebSocketClient client = WebSocketClient(wifi, serverAddress, port);
String clientID = TeamInfraRedID;
int status = WL_IDLE_STATUS;

/* WiFi and Server Declaration End */
void SetupServerConnection();
void waitForServerMessage(String message);

void setup() {
  // set pin modes
  pinMode(grn_led, OUTPUT);
  pinMode(red_led, OUTPUT);
  pinMode(blue_led, OUTPUT);
  pinMode(ylw_led, OUTPUT);
  pinMode(leftF, OUTPUT);
  pinMode(leftB, OUTPUT);
  pinMode(headlights, OUTPUT);
  pinMode(rightF, OUTPUT);
  pinMode(rightB, OUTPUT);
  pinMode(brakes, OUTPUT);
  pinMode(left_sig, OUTPUT);
  pinMode(right_sig, OUTPUT);
  // pinMode(start_button, INPUT);
  pinMode(color_sensor, INPUT);
  pinMode(collision_sensor, INPUT);
  pinMode(battery_sensor, INPUT);
  // pinMode(day_night_sensor, INPUT);

  // attachInterrupt(start_button, start_sim, RISING);

  Serial.begin(9600);

  SetupServerConnection();
}

void loop() {
  switch (state) {
    // idle, wait for challenge to start
    case 0:
      parking_lights();
      break;
    
    // perform diagnostic check
    case 1:
      diagnostics();
      state = 2;
      break;
    
    // cross track, turn around and find red path
    case 2:
      forward(70);
      while(!detect_and_reverse());
      delay(BREAK_TIME);
      turn180('r');
      delay(BREAK_TIME);
      forward(70);
      while(!find_red());
      delay(BREAK_TIME);
      state = 3;
      break;
    
    // signal bot 2 to start, traverse red path
    case 3:
      // send signal to bot 2
      sendMessageToServer("greetings bot 2, please start your trek");

      // wait for signal from bot 2
      waitForServerMessage("greetings bot 1, continue your trek");

      for (int i = 0; i < 2; i++) {
        receive_light();
      }
      // playAudioTrack(horn_honking_twice, 25);
      delay(500);

      delay(BREAK_TIME);
      /* turn back around onto red if we move */
      turnLeft();
      delay(BREAK_TIME);
      // follow red until it detects a wall
      digitalWrite(headlights, HIGH);
      digitalWrite(brakes, LOW);
      
      while(!detect_wall()) {
        follow_red();
      }
      delay(BREAK_TIME);
      // blink red light twice upon finding the wall
      for (int i = 0; i < 2; i++) {
        blink_red();
      }
      digitalWrite(orng_led, HIGH);
      /* signal bot two that we detected wall via server */
      /* replace with comms */
      /* wait to receive bot two's signal back and then blink red 3 times and illuminate green */
      waitForServerMessage("bot 1, go ahead");
      sendMessageToServer("bot 1 is at the end of red path");

      for (int i = 0; i < 3; i++) {
        blink_red();
      }
      digitalWrite(grn_led, HIGH);
      delay(BREAK_TIME);
      // reverse and 90 degree left turn
      reverse(70);
      delay(500);
      stop();
      delay(BREAK_TIME);
      turnLeft(); //this is not 90 deg yet, I feel like we have that function
      delay(BREAK_TIME);
      state = 4;
      break;
    
    // find yellow path and traverse it
    case 4:
      // move until find yellow path, stop and do indications described
      forward(70);
      while(!find_yellow());
      delay(BREAK_TIME);
      // turn left again to avoid going off the other side of the yellow path
      turnLeft(); 
      delay(BREAK_TIME);
      digitalWrite(headlights, HIGH);
      digitalWrite(brakes, LOW);
      while(!detect_wall()) {
        follow_yellow();
      }
      delay(BREAK_TIME);
      /* signal to bot 2 to continue journey */
      sendMessageToServer("bot 2, continue your journey");
      delay(3000);
      state = 5;
      break;
    
    // return to starting position and end challenge
    case 5:
      // rotates itself appropaiately
      turnLeft();
      /* move forward hard-coded amount until you reach starting point */
      forward(70);
      delay(1550);
      stop();
      digitalWrite(ylw_led, LOW);
      delay(BREAK_TIME);
      // park bot
      turnLeft();
      turnLeft();
      delay(BREAK_TIME);
      reverse(70);
      delay(1200);
      stop();
      /* wait for bot 2 to finish and signal us */
      // once detects signal from bot 2 that it finished, flash indicators 2x, headlights and break 3x, horn x2
      waitForServerMessage("bot 2 is finished");
      end_of_sim_ind();
      
      // return to idle state
      state = 0;
      break;
    
    default:
      break;
  }
}

float debounce(int sensor_pin) {
  const int debounceDelay = 170; // Debounce delay in milliseconds
  bool not_stable = true;
  float sensor_value = analogRead(sensor_pin) * ADC_RESOLUTION;
  float new_sensor_value;

  while (not_stable) {
    delay(debounceDelay);
    new_sensor_value = analogRead(sensor_pin) * ADC_RESOLUTION;
    if (abs(new_sensor_value - sensor_value) < 0.04) {
      not_stable = false;
    } else {
      sensor_value = new_sensor_value;
    }
  }
  return sensor_value;
}

void start_sim(void) {
  digitalWrite(brakes, LOW);
  state = 1;
}

void end_of_sim_ind(void) {
  // flash indicators
  for (int i = 0; i < 2; i++) {
    digitalWrite(left_sig, HIGH);
    digitalWrite(right_sig, HIGH);
    delay(BLINK_RATE);
    digitalWrite(left_sig, LOW);
    digitalWrite(right_sig, LOW);
    delay(BLINK_RATE);
  }
  // flash headlights and brake lights
  for (int i = 0; i < 3; i++) {
    digitalWrite(headlights, HIGH);
    digitalWrite(brakes, HIGH);
    delay(BLINK_RATE);
    digitalWrite(headlights, LOW);
    digitalWrite(brakes, LOW);
    delay(BLINK_RATE);
  }
  // honk horn twice
  // playAudioTrack(horn_honking_twice, 25);
}

void parking_lights(void) {
  digitalWrite(brakes, HIGH);
  delay(250);
  digitalWrite(brakes, LOW);
  delay(250);
}

void diagnostics(void) {
  // playAudioTrack(loading_sound, 30);
  for (int i = 0; i < 3; i++) {
    digitalWrite(red_led, HIGH);
    digitalWrite(blue_led, HIGH);
    delay(BLINK_RATE*3);
    digitalWrite(red_led, LOW);
    digitalWrite(blue_led, LOW);
    delay(BLINK_RATE*3);
  }
}

void blink_red(void) {
  digitalWrite(red_led, HIGH);
  delay(BLINK_RATE);
  digitalWrite(red_led, LOW);
  delay(BLINK_RATE);
}

void receive_light(void) {
  digitalWrite(headlights, HIGH);
  digitalWrite(brakes, HIGH);
  delay(BLINK_RATE);

  digitalWrite(headlights, LOW);
  digitalWrite(brakes, LOW);
  delay(BLINK_RATE);
}

float get_color() {
  return analogRead(color_sensor) * ADC_RESOLUTION;
}

bool find_red() {
  color = debounce(color_sensor);
  if ((color >= red_threshold_min) && (color <= red_threshold_max)) {
    stop();
    // playAudioTrack(red_found, 30);
    digitalWrite(red_led, HIGH);
    return true;
  }
  return false;
}

bool find_yellow() {
  color = debounce(color_sensor);
  if ((color >= yellow_threshold_min) && (color <= yellow_threshold_max)) {
    stop();
    // playAudioTrack(yellow_found, 30);
    digitalWrite(ylw_led, HIGH);
    digitalWrite(grn_led, LOW);
    // playAudioTrack(horn_honking_twice, 25);
    return true;
  }
  return false;
}

void follow_red() {
  color = get_color();
  if ((color >= red_threshold_min) && (color <= red_threshold_max)) {
    move_left();
  } else {
    move_right();
  }
}

void follow_yellow() {
  color = get_color();
  if ((color >= yellow_threshold_min) && (color <= yellow_threshold_max)) {
    move_left();
  } else {
    move_right();
  }
}

void move_left() {
  analogWrite(leftF, 0);
  analogWrite(rightF, 80);
  analogWrite(leftB, 0);
  analogWrite(rightB, 0);  
}

void move_right() {
  analogWrite(leftF, 80);
  analogWrite(rightF, 0);
  analogWrite(leftB, 0);
  analogWrite(rightB, 0);  
}

bool detect_wall(){
  collide = analogRead(collision_sensor) * ADC_RESOLUTION;
  if (collide >= collision_threshold) {
    stop();
    // playAudioTrack(detected_wall, 30);
    return true;
  }
  return false;
}

bool detect_and_reverse() {
  collide = analogRead(collision_sensor) * ADC_RESOLUTION;
  if (collide >= collision_threshold) {
    stop();
    delay(100);
    reverse(70);
    delay(500);
    stop();
    return true;
  }
  return false;
}

void stop() {
  digitalWrite(leftF, LOW);
  digitalWrite(leftB, LOW);
  digitalWrite(rightF, LOW);
  digitalWrite(rightB, LOW);
  digitalWrite(headlights, LOW);
  digitalWrite(brakes, HIGH);
}

void forward(int speed) {
  digitalWrite(brakes, LOW);
  digitalWrite(headlights, HIGH);
  digitalWrite(leftB, LOW);
  digitalWrite(rightB, LOW);
  analogWrite(leftF, speed*offset);
  analogWrite(rightF, speed);
}

void reverse(int speed) {
  digitalWrite(brakes, LOW);
  digitalWrite(headlights, HIGH);
  digitalWrite(leftF, LOW);
  digitalWrite(rightF, LOW);
  analogWrite(leftB, speed*offset);
  analogWrite(rightB, speed);   
}

void turnLeft() {
  digitalWrite(brakes, LOW);
  digitalWrite(left_sig, HIGH);
  digitalWrite(leftF, LOW);
  digitalWrite(rightB, LOW);
  analogWrite(leftB, turnSpeed);
  analogWrite(rightF, turnSpeed);
  delay(655);
  stop();
  digitalWrite(left_sig, LOW);
}

void turnRight() {
  digitalWrite(brakes, LOW);
  digitalWrite(right_sig, HIGH);
  digitalWrite(leftB, LOW);
  digitalWrite(rightF, LOW);
  analogWrite(leftF, turnSpeed);
  analogWrite(rightB, turnSpeed);
  delay(655);
  stop();
  digitalWrite(right_sig, LOW);
}

void turn180(char direction) {
  if (direction=='l') {
    digitalWrite(leftF, LOW);
    digitalWrite(rightB, LOW);
    analogWrite(leftB, turnSpeed);
    analogWrite(rightF, turnSpeed);
    delay(1130);
    digitalWrite(leftB, LOW);
    digitalWrite(rightF, LOW);
  } else if (direction=='r') {
    digitalWrite(leftB, LOW);
    digitalWrite(rightF, LOW);
    analogWrite(leftF, turnSpeed);
    analogWrite(rightB, turnSpeed);
    delay(1130);
    digitalWrite(leftF, LOW);
    digitalWrite(rightB, LOW);
  }
}

// call this function to receive a message 
// from Team Smile on the server
// returns the message received
String receiveMessageFromServer() {
  if (client.connected()) {
      // check if a message is available to be received
      int messageSize = client.parseMessage();

      String command = "";
      if (messageSize > 0) {
        String message = client.readString();
        String companion_id = message.substring(0, 11);
        String web_client = message.substring(0, 22);
        int pos = 11;

        if(companion_id == TeamSmileID || web_client == "WebClient_" + TeamSmileID){ //correct companion id 
          // increase space in string array until reach white space
          while(message[pos] != '.'){
            pos++;
          }
          // take position after white space until the end and that is the message
          command = message.substring(pos + 1); 
        }

        if (command.length() > 0){
          Serial.println("Received the command: ");
          Serial.println(command);
          return command;
        }
      }
    }
    Serial.println("disconnected");
    return "disconnected";
}

// call this function to send a message to server
void sendMessageToServer(String message) {
  if (client.connected()) {
    client.beginMessage(TYPE_TEXT);
    client.print(message);
    client.endMessage();
    // playAudioTrack(sending_message, 30);
  }
}

// void playAudioTrack(int track_number, int sound_volume) {
  //mp3.initialize();
  // mp3.playTrackNumber(track_number, sound_volume, false);
  //mp3.playTrackNumber(track_number, sound_volume);
  // while(!mp3.playCompleted());
// }

void waitForServerMessage(String message) {
  volatile bool messageReceived = false;
  while(!messageReceived) {
    String receivedStr = receiveMessageFromServer();
    if (receivedStr == message) {
      // playAudioTrack(message_recieved, 30);
      messageReceived = true;
      return;
    }
  }
}

void SetupServerConnection() {
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
  }

  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  Serial.println("Starting WebSocket client");
  client.begin();
  client.beginMessage(TYPE_TEXT);
  client.print(clientID);
  client.endMessage();
}