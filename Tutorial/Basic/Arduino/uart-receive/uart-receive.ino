void setup() {
	Serial.begin(9600); // opens serial port, baudrate : 9600 bps
	Serial.println("Arduino's receiving : ");
}


void loop() {
	if (Serial.available() > 0) {
		String str = Serial.readString();
		Serial.print("Received: ");
		Serial.println(str);
	}
}