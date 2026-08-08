#include <SPI.h>
#include <LoRa.h>

const int turbidityPin = A0;
const int phPin = A1;
const int levelPin = A2;
const int tdsPin = A3;

int counter = 0;

// Functions to calculate sensor values
float calculateTurbidity(int raw_adc, float v_ref = 5.0) {
    return (raw_adc / 1023.0) * v_ref * 100.0;
}
float calculatePH(int raw_adc, float v_ref = 5.0, float ph_ref = 7.0, float voltage_at_ref = 2.5) {
    float voltage = (raw_adc / 1023.0) * v_ref;
    float slope = -0.17;
    return ph_ref + (voltage - voltage_at_ref) / slope;
}
float calculateLevel(int raw_adc, float v_ref = 5.0) {
    return (raw_adc / 1023.0) * v_ref * 100.0;
}
float calculateTDS(int raw_adc, float v_ref = 5.0) {
    return (raw_adc / 1023.0) * v_ref * 500;
}

void setup() {
    Serial.begin(9600);
    Serial.println("LoRa and Sensor Monitor");

    if (!LoRa.begin(433E6)) {
        Serial.println("LoRa init failed!");
        while (1);
    }

    LoRa.setTxPower(20);
}

void loop() {
    int rawTurbidity = analogRead(turbidityPin);
    float turbidityValue = calculateTurbidity(rawTurbidity);

    int rawPH = analogRead(phPin);
    float phValue = calculatePH(rawPH, 5.0, 7.0, 2.5);

    int rawLevel = analogRead(levelPin);
    float levelValue = calculateLevel(rawLevel);

    int rawTDS = analogRead(tdsPin);
    float tdsValue = calculateTDS(rawTDS);

    // Print sensor data to Serial Monitor
    Serial.println("Sensor Data:");
    Serial.print(" - Turbidity: "); Serial.print(turbidityValue); Serial.println(" NTU");
    Serial.print(" - pH Value: "); Serial.println(phValue);
    Serial.print(" - Liquid Level: "); Serial.print(levelValue); Serial.println("%");
    Serial.print(" - TDS Value: "); Serial.print(tdsValue); Serial.println(" ppm");

    // Send Data via LoRa
    Serial.print("Sending packet: "); Serial.println(counter);

    String message = "Sensor Data - Turb: " + String(turbidityValue) +
                     ", pH: " + String(phValue) +
                     ", Level: " + String(levelValue) +
                     ", TDS: " + String(tdsValue);

    LoRa.beginPacket();
    LoRa.print(message);
    LoRa.endPacket();

    counter++;
    delay(5000);
}