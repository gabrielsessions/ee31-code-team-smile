
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(2, OUTPUT); // Switch 1
  digitalWrite(2, HIGH);
}

void loop() { 
  // put your main code here, to run repeatedly:
    digitalWrite(2, HIGH);
    int potentiometer1 = analogRead(A0);
    if(potentiometer1 < 95){
      //its night

    }else{
      //it be day

      delay(1000);
      digitalWrite(2, LOW);

    }
    Serial.println(potentiometer1);
    delay(1000);
}

void dayStatus(){

}

void nightStatus(){

}