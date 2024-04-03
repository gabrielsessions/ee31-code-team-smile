/*
  Web client
 This sketch connects to a website (http://www.google.com)
 using a WiFi shield.
 This example is written for a network using WPA encryption. For
 WEP or WPA, change the Wifi.begin() call accordingly.
 This example is written for a network using WPA encryption. For
 WEP or WPA, change the Wifi.begin() call accordingly.

 modified 26 March 2024
 Initial Code by Tom Igoe
 Modified Code by Gabriel Sessions
 */

#include <SPI.h>
#include <WiFi.h>

// Requires Arduino_JSON library by Arduino (v0.2)
#include <Arduino_JSON.h>

char ssid[] = "junior"; //  your network SSID (name)
char pass[] = "designdesign";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
char server[] = "34.31.106.88";    // name address for Google (using DNS)

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):

WiFiClient client;
String response = "";
String botID = "4A9EDB0160D5";

void setup() {
  Serial.begin(9600);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  String fv = WiFi.firmwareVersion();


  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 6 seconds for connection:
    // Shorten if the device can connect more quickly
    delay(6000);

  }

  Serial.println("Connected to wifi");

  JSONVar testBody;
  testBody["motor1"] = "-50";
  testBody["motor2"] = "50";
  
  postRequest(botID, "motors", testBody);
  //getRequest(botID);
  //printWifiStatus();
}



void loop() {
  // if there are incoming bytes available
  // from the server, read them and print them:
  while (client.available()) {
    char c = client.read();
    response += c;
    //Serial.write(c);
  }

  // if the server's disconnected, stop the client:
  if (response.length() > 0 && !client.connected()) {
    if (response.indexOf("\r\n\r\n") == -1) {
      Serial.println(response);
    }
    else {
      response = response.substring(response.indexOf("\r\n\r\n") + 4);
      JSONVar responseJSON = JSON.parse(response);
      Serial.println(responseJSON);
    }
    
    Serial.println("disconnecting from server.");
    client.stop();
    response = "";
   
  }
}

void getRequest(String id) {
  Serial.println("\nStarting connection to server...");

  // if you get a connection, report back via serial:
  // Use HTTP connection over port 80, SSL/HTTPS is also supported but requires additional setup
  if (client.connect(server, 80)) {
    Serial.println("connected to server");

    // Make a HTTP request:
    client.println("GET /botstate/" + id + " HTTP/1.1");
    client.println("Host: 34.31.106.88");
    client.println("Connection: close");
    client.println();
  }
}

void postRequest(String id, String key, JSONVar value) {
  // Convert the JSON object to a String
  JSONVar body;
  body["id"] = id;
  body["key"] = key;
  body["value"] = value;
  String requestBody = JSON.stringify(body);

  if (client.connect(server, 80)) {
    Serial.println("connected to server");
    
    // Calculate the length of the request body
    int contentLength = requestBody.length();

    // Make a HTTP request:
    client.println("POST /botstate HTTP/1.1");
    client.println("Host: 34.31.106.88");
    client.println("Content-Type: application/json");
    client.println("Connection: close");
    client.print("Content-Length: "); client.println(String(contentLength));
    client.println(); // End of headers
    client.println(requestBody); // Send the JSON data

  }

}

