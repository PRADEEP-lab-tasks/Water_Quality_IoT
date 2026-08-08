// created by pradeep its a lora reciever using aurdino and lora module
#include <SPI.h>
#include <LoRa.h>

void setup() {
  Serial.begin(9600); // Initialize Serial communication at 9600 baud
  while (!Serial);

  Serial.println("LoRa Receiver");

  // Initialize LoRa module with 447 MHz frequency
  if (!LoRa.begin(447E6)) { 
    Serial.println("Starting LoRa failed!");
    while (1); // Stop execution if LoRa initialization fails
  }
}

void loop() {
  // Continuously parse incoming packets
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // Received a packet
    Serial.print("Received packet: ");

    // Read the incoming data
    String receivedData = "";
    while (LoRa.available()) {
      char incomingByte = (char)LoRa.read();
      receivedData += incomingByte; // Append each character to a string
    }

    // Print the received data and RSSI
    Serial.println(receivedData);
    Serial.print("RSSI: ");
    Serial.println(LoRa.packetRssi()); // Signal strength

    // Optional: Add a delay to process packets at regular intervals
    delay(500); 
  }
}
