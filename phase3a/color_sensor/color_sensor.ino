void setup() {
  Serial.begin(9600);
  Serial.println("RUNNING!");
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(A0, INPUT);

  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);
}


// 940 - 950 for RED
// 900 - 910 for BLUE
// 960 - 970 for YELLOW
// 990 - 1000 for BLACK


// 930 RED, 970 BLUE, 950 YELLOW

void loop() {
  // put your main code here, to run repeatedly:
  int val = analogRead(A0);
  Serial.println(val);
  delay(100);

}
