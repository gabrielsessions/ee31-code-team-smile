// Receiver (Arduino 2)
#include <IRremote.h>

IRrecv irrecv(2); // Define pin 2 as the receiver pin
decode_results results;

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  if (irrecv.decode(&results)) {
    // Print received data
    Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value
  }
}
