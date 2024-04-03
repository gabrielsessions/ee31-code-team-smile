void setup() {
  Serial.begin(9600);
  Serial.println("RUNNING!");
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(A0, INPUT);

  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);
}


// 180 - 220 for RED
// 130 - 180 for BLUE
// 220 - 260 for YELLOW
// Below 130 for BLACK


// 930 RED, 970 BLUE, 950 YELLOW

void loop() {
  // put your main code here, to run repeatedly:
  int val = analogRead(A0);
  Serial.println(val);
  delay(100);

  if (val < 130) {
    Serial.print(" BLACK ");
  } 
  else if (val < 180) {
    Serial.print(" BlUE ");
  }
  else if (val < 220) {
    Serial.print(" RED ");
  }
  else if (val < 260) {
    Serial.print(" YELLOW ");
  }
  else {
    Serial.print(" UNKNOWN ");
  }

}
