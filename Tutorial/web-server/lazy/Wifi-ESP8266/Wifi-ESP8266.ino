// WifiClientBasic.ino

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti WiFiMulti;
// Use WiFiClient class to create TCP connections
WiFiClient client;
#define ssid "******"  // network name
#define password "******" // password
#define host "192.168.0.101" // ip address of raspberry
#define port 8888         // port open of server in raspberry
#define LIGHT_PIN 4
#define DEVICE_NAME "Toilet Light"
#define IdDevice "L2"
#define INIT_IdDevice "INIT:"IdDevice

bool SentID();
bool Switch(bool set);

void setup() {
    Serial.begin(115200);
    delay(10);

    // We start by connecting to a WiFi network
    WiFiMulti.addAP(ssid, password);

    Serial.println();
    Serial.println();
    Serial.print("Wait for WiFi... ");

    while(WiFiMulti.run() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    pinMode(LIGHT_PIN, OUTPUT);
    delay(500);
}

void loop() {

    Serial.print("connecting to ");
    Serial.println(host);


    if (!client.connect(host, port)) {
        Serial.println("connection failed");
        Serial.println("wait 5 sec...");
        delay(5000);
        return;
    }

    while(!SentID()); // wait until set successful

    while(1){
        // Read all the lines of the reply from server and print them to Serial
        while(client.available()) {
            String line = client.readStringUntil('\r');
            Serial.println(line);

            if(line.equals("state=1"))
              Switch(true);
            else if(line.equals("state=0"))
              Switch(false);
        }
    }

    client.stop();

}

bool SentID() {
  Serial.print("Send identify: "); Serial.println(DEVICE_NAME);
  client.print(INIT_IdDevice);
  delay(500);
  while(client.available()) {
      String line = client.readStringUntil('\r');
      if(line.equals("OK")) {
          Serial.println("the device was successful setted");
          return true;
      }
  }
  return false;
}

bool Switch(bool set) {
  Serial.print("Switch is ");
  if(set) Serial.println("on !");
  else  Serial.println("off !");
  digitalWrite(LIGHT_PIN, set);
  return true;
}
