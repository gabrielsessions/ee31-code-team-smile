diff --git a/smilebot/finalcontrol/finalcontrol.ino b/smilebot/finalcontrol/finalcontrol.ino
index 97eefe7..c52eb8e 100644
--- a/smilebot/finalcontrol/finalcontrol.ino
+++ b/smilebot/finalcontrol/finalcontrol.ino
@@ -126,7 +126,7 @@ void setup() {
 
     // Connect to WPA/WPA2 network:
     status = WiFi.begin(ssid, pass);
-    delay(8000);
+    delay(6000);
   }
 
   // print the SSID of the network you're attached to:
@@ -208,7 +208,7 @@ void loop() {
     client.endMessage(); 
     // getColorString()
     // getAmbient()
-    // getCollisDetect()
+    getCollisDetect()
     // getBatVoltage()
 
     // wait 10ms
@@ -358,18 +358,18 @@ String getColorString(){
 
 
 void printDebug(){
-  Serial.print("RAW Color: ");
-  Serial.println(getColorRaw());
-  Serial.print("Guess Color: ");
-  Serial.println(getColorString());
+  // Serial.print("RAW Color: ");
+  // Serial.println(getColorRaw());
+  // Serial.print("Guess Color: ");
+  // Serial.println(getColorString());
 
 
   /* Serial.println("Ambient Light:");
   Serial.println(getAmbient()); */
   
-  //Serial.println("Collision Detection:");
-  //Serial.println(getCollisDetect());
-  //Serial.println(analogRead(A0));
+  Serial.println("Collision Detection:");
+  Serial.println(getCollisDetect());
+  Serial.println(analogRead(A0));
   /* Serial.println("Battery voltage");
   Serial.println(getBatVoltage()); */
   
