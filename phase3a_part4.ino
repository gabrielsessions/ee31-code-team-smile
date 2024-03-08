const int analogPin = A5; // Analog input pin
const float R1 = 12000.0; // 12kΩ resistor
const float R2 = 10000.0; // 10kΩ resistor
const float Vref = 4.8; // Reference voltage (5V for most Arduinos)
const float ADCmax = 1023.0; // Max ADC value for a 10-bit ADC

void setup() {
  pinMode(analogPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(analogPin);
  float voltage = sensorValue * (Vref / ADCmax) * ((R1 + R2) / R2);
  
  Serial.print("Battery Voltage: ");
  Serial.println(voltage);
  if (voltage < 7.25) {
    Serial.println("LOW BATTERY WARNING");
  }
  
  delay(1000); // Delay a second between measurements
}
