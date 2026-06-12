#include<stdio.h>
#include<conio.h>
// Define LCD I2C address and screen size
LiquidCrystal_I2C lcd(0x27, 16, 2); // 0x27 is a common I2C address for LCDs

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

    lcd.init();  // Initialize I2C LCD
    lcd.backlight();
    lcd.print("Initializing...");

    if (!LoRa.begin(433E6)) {
        Serial.println("LoRa init failed!");
        lcd.clear();
        lcd.print("LoRa Error!");
        while (1);
    }

    LoRa.setTxPower(20);
    lcd.clear();
    lcd.print("LoRa Ready!");
    delay(2000);
}

void loop() {
    int rawTurbidity = analogRead(turbidityPin);
    float turbidityValue_T1 = calculateTurbidity(rawTurbidity);

    int rawPH = analogRead(phPin);
    float phvalue_T1 = calculatePH(rawPH, 5.0, 7.0, 2.5);

    int rawLevel = analogRead(levelPin);
    float levelvalue_T1 = calculateLevel(rawLevel);

    int rawTDS = analogRead(tdsPin);
    float tdsValue_T1 = calculateTDS(rawTDS);

    // Display Sensor Data on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Turb: "); lcd.print(turbidityValue_T1); lcd.print(" NTU");
    lcd.setCursor(0, 1);
    lcd.print("pH: "); lcd.print(phvalue_T1);

    delay(3000); // Give time to view readings

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Level: "); lcd.print(levelvalue_T1); lcd.print("%");
    lcd.setCursor(0, 1);
    lcd.print("TDS: "); lcd.print(tdsValue_T1); lcd.print(" ppm");

    delay(3000); // Another delay to view readings before sending

    // Send Data via LoRa
    Serial.print("Sending packet: "); Serial.println(counter);

    String message = "TANK 1 - Turb: " + String(turbidityValue_T1) +
                     ", pH: " + String(phvalue_T1) +
                     ", Level: " + String(levelvalue_T1) +
                     ", TDS: " + String(tdsValue_T1);

    LoRa.beginPacket();
    LoRa.print(message);
    LoRa.endPacket();

    counter++;
    delay(300);
}
