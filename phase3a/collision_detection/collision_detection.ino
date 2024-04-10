void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(analogRead(A0));
  Serial.println(detectCollision());
  delay(100);

}

bool detectCollision() {
  return analogRead(A0) > 996;
}
