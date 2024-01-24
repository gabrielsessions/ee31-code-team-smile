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
  pinMode(13, OUTPUT); // BLUE
  pinMode(12, OUTPUT); // GREEN
  pinMode(11, OUTPUT); // RED
  pinMode(10, OUTPUT); // Switch 1
  pinMode(9, OUTPUT); // Switch 2
  Serial.begin(9600);  // initialize serial communication at 9600 bits per second

}

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
