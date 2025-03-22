#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>

// Wi-Fi credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* serverUrl = "YOUR_SERVER_ENDPOINT";

// Sensor pins
#define HEART_RATE_PIN 34
#define EMG_SENSOR_PIN 35
#define MOTION_SENSOR_PIN 32
#define BP_SENSOR_SCL 22
#define BP_SENSOR_SDA 21

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to Wi-Fi");
    Wire.begin(BP_SENSOR_SDA, BP_SENSOR_SCL);
}

void loop() {
    int heartRate = analogRead(HEART_RATE_PIN);
    int emgValue = analogRead(EMG_SENSOR_PIN);
    int motionValue = digitalRead(MOTION_SENSOR_PIN);
    int bpValue = readBloodPressure();

    Serial.printf("Heart Rate: %d, EMG: %d, Motion: %d, BP: %d\n", heartRate, emgValue, motionValue, bpValue);
    sendDataToServer(heartRate, emgValue, motionValue, bpValue);

    delay(5000);
}

int readBloodPressure() {
    // Simulated BP reading, replace with actual I2C sensor reading
    return random(80, 140);
}

void sendDataToServer(int heartRate, int emg, int motion, int bp) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(serverUrl);
        http.addHeader("Content-Type", "application/json");
        
        String payload = "{"heartRate": " + String(heartRate) + ", "emg": " + String(emg) + ", "motion": " + String(motion) + ", "bp": " + String(bp) + "}";
        int httpResponseCode = http.POST(payload);
        Serial.printf("Server Response: %d\n", httpResponseCode);
        
        http.end();
    }
}
