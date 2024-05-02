/*
  Final Junio Design Program
*/
#include <ArduinoHttpClient.h>
#include <WiFi.h>

#define MOTOR1A 5
#define MOTOR1B 9

#define MOTOR2A 3
#define MOTOR2B 10

/*
Pin Definitions

*/
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
/////// WiFi Settings ///////
char ssid[] = "junior";
char pass[] = "designdesign";

char serverAddress[] = "34.173.203.110";  // server address
int port = 80;

WiFiClient wifi;
WebSocketClient client = WebSocketClient(wifi, serverAddress, port);
String clientID = "4A9EDB0160D5";
int status = WL_IDLE_STATUS;
int count = 0;

void setup() {
  Serial.begin(9600);
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
  }

  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void loop() {
  Serial.println("starting WebSocket client");
  client.begin();
  client.beginMessage(TYPE_TEXT);
  client.print(clientID);
  client.endMessage();

  while (client.connected()) {
    if (count > 6000) {
      count = 0;
    }
    if (count % 100 == 0) {
       Serial.print("Sending hello ");
      Serial.println(count);

      // send a hello #
      client.beginMessage(TYPE_TEXT);
      client.print("hello ");
      client.print(count);
      client.endMessage();
    }
   
    // increment count for next message
    count++;

    // check if a message is available to be received
    int messageSize = client.parseMessage();
    if (messageSize > 0) {
      Serial.println("Received a message:");
      Serial.println(client.readString());
    }

    // wait 10ms
    delay(10);
  }

  Serial.println("disconnected");
 
}

void motor