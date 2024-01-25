#define BLUE_LED 6
#define GREEN_LED 9
#define RED_LED 5
#define SWITCH1 2
#define SWITCH2 3
#define FADE_AMOUNT 5
#define TIME_CONSTANT 6000
#define BAUD_RATE 9600

//EE31 Phase 1

enum State {
  ON,
  OFF,
  SLEEP,
  RUN,
  DIAGNOSTIC
};

State ledState;
int greenLEDBrightness, blueLEDBrightness;
int blueLEDInterval = 1;
int global_brightness = 100;

void setup() {
  // put your setup code here, to run once:
  ledState = OFF;
  greenLEDBrightness = 255;
  blueLEDBrightness = 255;
  pinMode(BLUE_LED, OUTPUT); // BLUE
  pinMode(GREEN_LED, OUTPUT); // GREEN
  pinMode(RED_LED, OUTPUT); // RED
  pinMode(SWITCH1, INPUT); // Switch 1
  pinMode(SWITCH2, INPUT); // Switch 2
  attachInterrupt(digitalPinToInterrupt(SWITCH1), trigSw1, RISING);
  attachInterrupt(digitalPinToInterrupt(SWITCH2), trigSw2, FALLING);
  Serial.begin(BAUD_RATE);  // initialize serial communication at 9600 bits per second
  
}
/* 
In the Run state produce the desired functionality:  
1. when switch #1 is moved from OPEN to CLOSED, an interrupt is triggered 
which changes the rate at which a BLUE LED flashes to 
ten times per second from a rate of once per second.

2. When switch #2 is moved from CLOSED to OPEN, an interrupt is triggered 
only if the flash rate of the BLUE LED is ten times per second, by turning on a RED LED.  

When BLUE LED flash rate is once per second the RED LED is off.

 */
void trigSw1(){
  if(ledState = RUN){
    blueLEDInterval = 10;
  }
}
void trigSw2(){
  if(ledState == RUN and blueLEDInterval == 10){
    digitalWrite(RED_LED, HIGH);
  }
}

void fade (int time_per, int num_steps, int led){
  int brightness = 255; // This can be 0-255
  int light_interval = num_steps / 255;
  for(int i = 0; i < num_steps; i++){
    brightness -= light_interval;
    analogWrite(led, (brightness*global_brightness) / 100);
    delay(time_per/num_steps);
  }
}

void setLedState(bool on, int led){
  if(on){
    analogWrite(led, global_brightness*2);
  }else{
    analogWrite(led, 0);
  }
  
}



void loop() {
  //A5 = Potentiometer 1
  //A4 = Potentiometer 2
  int potentiometer1 = analogRead(A5);
  int potentiometer2 = analogRead(A4);
  global_brightness = potentiometer1 / 10;

  if (Serial.available() > 0) {
    // if data is available to read
    String inputWord = Serial.readStringUntil('\n');
    Serial.print("You entered: ");
    Serial.println(inputWord);  // print the entered word
    if (inputWord.equals("ON")) {
      ledState = ON;
      Serial.println("LED ON");
    } else if (inputWord.equals("OFF")) {
      ledState = OFF;
    } else if (inputWord.equals("SLEEP")) {
      ledState = SLEEP;
    } else if (inputWord.equals("RUN")) {
      ledState = RUN;
    } else if (inputWord.equals("DIAGNOSTIC")) {
      ledState = DIAGNOSTIC;
    }
  }



  if (ledState == ON) {
    setLedState(true, RED_LED);
    delay((potentiometer2 / 2) + 5);            // waits for a second
    setLedState(false, RED_LED);
    delay((potentiometer2 / 2) + 5);            // waits for a second 
  }

  else if (ledState == OFF) {
    setLedState(false, RED_LED);
  }

/*
  Blinks @ 4 Hz for 1 sec, Fade for 1 sec.  Last blink fades to off over Fade.
*/
  else if (ledState == SLEEP) {
    for(int i = 0; i < blueLEDInterval; i++){
      setLedState(true, BLUE_LED);
      delay(1000 / blueLEDInterval);
      setLedState(false, BLUE_LED);
      delay(1000 / blueLEDInterval);
    }
    fade (1000, 255, BLUE_LED);
    ledState = OFF;

  }
  else if (ledState == RUN) {
    //Fade LED for 6s
    fade (TIME_CONSTANT, 255, GREEN_LED);

    //Blink twice for duty cycle of 0.5s
    setLedState(true, GREEN_LED);
    delay(500);
    setLedState(false, GREEN_LED);
    delay(500);
    setLedState(true, GREEN_LED);
    delay(500);
    setLedState(false, GREEN_LED);
    delay(500);
  }

  else if (ledState == DIAGNOSTIC) {
    int num_problems = 5;
    for(int i = 0; i < num_problems; i++){
      setLedState(true, RED_LED);
      delay(250);
      setLedState(false, RED_LED);
      delay(250);
    }
    delay(1500);
  }
  
}
