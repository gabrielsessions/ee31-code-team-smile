const int analogPin9V = A5; // Analog input pin
const int analogPin4V = A4; // 4.5V input pin
const float R1 = 12000.0; // 12kΩ resistor
const float R2 = 10000.0; // 10kΩ resistor
const float Vref = 4.8; // Reference voltage (5V for most Arduinos)
const float ADCmax = 1023.0; // Max ADC value for a 10-bit ADC

void setup() {
  pinMode(analogPin9V, INPUT);
  Serial.begin(9600);
}

void loop() {
  int sensorValue9V = analogRead(analogPin9V);
  int sensorValue4V = analogRead(analogPin4V);
  float voltage9V = sensorValue9V * (Vref / ADCmax) * ((R1 + R2) / R2);
  float voltage4V = sensorValue4V * (Vref / 1023.0);
  
  Serial.print("Battery Voltage: ");
  Serial.println(voltage9V);
  Serial.println(voltage4V);

  if (voltage9V < 7.25) {
    Serial.println("9V LOW BATTERY WARNING");
  }
  if (voltage4V < 4) {
    Serial.println("4.5V LOW BATTERY WARNING");
  }
  
  delay(1000); // Delay a second between measurements
}
