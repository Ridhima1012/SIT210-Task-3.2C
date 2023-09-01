#include <WiFiNINA.h>

#include <BH1750.h>
#include <Wire.h>

//please enter your sensitive data in the Secret tab
char ssid[] = "Ridhima";
char pass[] = "Ridhima@123";

WiFiClient client;
BH1750 lightMeter;

char   HOST_NAME[] = "maker.ifttt.com";
String PATH_NAME  = "/trigger/Light/with/key/fanj7DJdg52KEq5ap2sZz6GNIY-pKYiu25jctGYt-2f";
String queryString = "?value1=57&value2=25";

void setup() {
  // initialize WiFi connection
  // WiFi.begin(ssid, pass);
  Serial.begin(9600);
Serial.print("START");
  while (!Serial);

  // connect to web server on port 80:
     WiFi.begin(ssid, pass);
     Wire.begin();
while(true)
  {
  if (client.connect(HOST_NAME, 80)) {
    // if connected:
    Serial.println("Connected to server");
    break;
  }
  else {// if not connected:
    Serial.println("connection failed");
    
  }
  delay(500);
}
lightMeter.begin();
Serial.println("Connected to server");
}
void loop() {
  Serial.print("START");
  float lux = lightMeter.readLightLevel();

  // Serial.println("Light sensor: ");
  // Serial.p'int(lux);

  queryString +="?value1=" ;
  queryString += lux;
  Serial.println(queryString);

  if (lux > 500) {

  
    
    if (!lightDetected) {
      // When light is detected (and it was previously not detected), send notification.
      sendNotification(true);
      client.println("GET " + PATH_NAME + queryString + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println(); // end HTTP header // Send notification for light detection.
    }
    lightDetected = true; // Set the flag to indicate light is detected.
  } else {
    if (lightDetected) {
      // When light goes away (and it was previously detected), send notification.
      sendNotification(false); 
      client.println("GET " + PATH_NAME + queryString + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println(); // end HTTP header
    // Send notification for light absence.
    }
    lightDetected = false; // Set the flag to indicate light is not detected.
  }
  
  queryString = "";
  delay(3000);
}

   void sendNotification(bool lightStatus) {
    while (client.connected()) {
      if (client.available()) {
        // read an incoming byte from the server and print it to serial monitor:
        char c = client.read();
        Serial.print(c);
      }
    }

    // the server's disconnected, stop the client:
    client.stop();
    Serial.println();
    Serial.println("disconnected");
  }
  queryString = "";
  delay(3000);
}