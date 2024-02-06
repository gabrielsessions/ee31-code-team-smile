void setup() {
  // motor 0 and 1
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(3, HIGH);
  digitalWrite(5, LOW);
  delay(1000);
  digitalWrite(5, HIGH);
  digitalWrite(3, LOW);
  delay(1000);
}
