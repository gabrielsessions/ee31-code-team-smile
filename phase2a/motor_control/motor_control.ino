#define PORT1 3
#define PORT2 5

void setup() {
  // motor 0 and 1
  pinMode(PORT1, OUTPUT);
  pinMode(PORT2, OUTPUT);
  Serial.begin(9600);
  Serial.println("Hello, World!");
  digitalWrite(PORT2, LOW);
  analogWrite(PORT1, 100);
  
}

void loop() {
  // put your main code here, to run repeatedly:

  if (Serial.available() > 0) {
    // if data is available to read
    String inputWord = Serial.readStringUntil('\n');
    Serial.print("You entered: ");
    Serial.println(inputWord);
    int newSpeed = inputWord.toInt();
    moveMotor1(newSpeed);
  }
  
  delay(1);
}


void moveMotor1(int speed) {
  Serial.println(speed);
  if (speed > 0) {
      digitalWrite(PORT2, LOW);
      analogWrite(PORT1, speed);
      return;
  }
  digitalWrite(PORT1, LOW);
  analogWrite(PORT2, -speed);
}

