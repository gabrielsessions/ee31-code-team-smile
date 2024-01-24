enum State {
  ON,
  OFF,
  SLEEP,
  RUN,
  DIAGNOSTIC
};

State ledState;


void setup() {
  // put your setup code here, to run once:
  ledState = OFF;
  pinMode(6, OUTPUT); // BLUE LED
  pinMode(3, OUTPUT); // GREEN LED
  pinMode(5, OUTPUT); // RED LED
  pinMode(12, INPUT); // Switch 1
  pinMode(13, INPUT); // Switch 4
  Serial.begin(9600);  // initialize serial communication at 9600 bits per second

}


// the loop routine runs over and over again forever:
void usePotentiometer() {
  //A5 = Potentiometer 1
  //A4 = Potentiometer 2


  // Start reading potentiometer values
  int potentiometer1 = analogRead(A5);
  int potentiometer2 = analogRead(A4);

  //Lets just print it for now
  Serial.println(potentiometer1);

  delay(1);  // delay in between reads for stability
}

/* void fade (int time_per, int num_steps){
  int brightness = 0; // This can be 0-255
  int light_interval = num_steps / 255;
  for(int i = 0; i < num_steps; i++){
    brightness += light_interval;
    set_brightness(brightness);
    delay(time_per/num_steps);
  }
} */


void loop() {

  if (Serial.available() > 0) {
    // if data is available to read
    String inputWord = Serial.readStringUntil('\n');  // read the word until a space is encountered
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
    // put your main code here, to run repeatedly:
    
    digitalWrite(13, HIGH); // sets the digital pin 13 on
    delay(1000);            // waits for a second
    digitalWrite(13, LOW);  // sets the digital pin 13 off
    delay(1000);            // waits for a second 
  }

  else if (ledState == OFF) {
    digitalWrite(13, LOW);
  }

  else if (ledState == SLEEP) {

  }
  
}