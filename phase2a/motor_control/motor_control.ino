#define PORT1 5
#define PORT2 9

#define MOTOR2A 3
#define MOTOR2B 10

void setup() {
  // motor 0 and 1
  pinMode(PORT1, OUTPUT);
  pinMode(PORT2, OUTPUT);
  pinMode(MOTOR2A, OUTPUT);
  pinMode(MOTOR2B, OUTPUT);
  //enables
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);

  /* Serial.begin(9600);
  Serial.println("Hello, World!"); */
  //digitalWrite(PORT2, LOW);
  //analogWrite(PORT1, 100);
  digitalWrite(PORT1, LOW);
  digitalWrite(PORT2, HIGH);
  digitalWrite(MOTOR2A, HIGH);
  digitalWrite(MOTOR2B, LOW);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
 /*  if (Serial.available() > 0) {
    // if data is available to read
    String inputWord = Serial.readStringUntil('\n');
    Serial.print("You entered: ");
    Serial.println(inputWord);
    int newSpeed = inputWord.toInt();
    moveMotor1(newSpeed);
  } */
  
  /* delay(1000);
  digitalWrite(PORT2, LOW);
  digitalWrite(PORT1, HIGH);

  digitalWrite(MOTOR2B, HIGH);
  digitalWrite(MOTOR2A, LOW);
  delay(1000); */
}


/* void moveMotor1(int speed) {
  Serial.println(speed);
  if (speed > 0) {
      digitalWrite(PORT2, LOW);
      analogWrite(PORT1, speed);
      return;
  }
  digitalWrite(PORT1, LOW);
  analogWrite(PORT2, -speed);
} */

