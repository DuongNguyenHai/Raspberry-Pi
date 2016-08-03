/*
 *  This sketch sends a message to a TCP server
 *
 */

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti WiFiMulti;

const uint16_t port = 8888;         // port open of server in raspberry
const char * host = "192.168.1.23"; // ip or dns of raspberry
    
void setup() {
    Serial.begin(115200);
    delay(10);

    // We start by connecting to a WiFi network
    WiFiMulti.addAP("My home", "sodienthoaicuatao");

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

    delay(500);
}


void loop() {
    
    Serial.print("connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;

    if (!client.connect(host, port)) {
        Serial.println("connection failed");
        Serial.println("wait 5 sec...");
        delay(5000);
        return;
    }
    while(1){
      if (Serial.available() > 0) {
        // read the incoming byte:
        String str = Serial.readString();
        
        // This will send the request to the server
        client.print(str);
        // gửi cái này lên seril
        Serial.print("message: ");
        Serial.println(str);
      }
        
    }

    //read back one line from server
    //String line = client.readStringUntil('\r');
    //client.println(line);

    //Serial.println("closing connection");
    client.stop();
    
}

