#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <MAX30105.h>

// WiFi credentials
const char* WIFI_SSID = "POCO M3";
const char* WIFI_PASSWORD = "kisal123";


unsigned long previousMillis = 0; // Stores the last time the screen was updated
const long interval = 2000;       // Interval to switch screens (2 seconds)
bool showTemperature = false;    // Flag to alternate between screens

// MQTT topics
const char* heart_rate_topic = "heart_rate_anju";
const char* spo2_topic = "spo2_anju";
const char* temperature_topic = "temperature_anju";

// Sensor and LCD Setup
LiquidCrystal_I2C lcd(0x27, 16, 2);
WiFiClient espClient; 
PubSubClient mqttClient(espClient);
MAX30105 particleSensor;

// Global Variables
float temperature = 0;
float heartRate = 0;
float spo2 = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22); // SDA on GPIO21, SCL on GPIO22
  Wire.setClock(100000); // Set I2C clock to 100kHz
  
  lcd.begin(16, 2);
  lcd.backlight();

  setupWiFi();
  mqttClient.setServer("test.mosquitto.org", 1883);

  if (!particleSensor.begin(Wire, I2C_SPEED_STANDARD)) {
    Serial.println("MAX30102 not found. Check wiring/power.");
    while (1);
  }
  Serial.println("MAX30102 initialized.");
  
  // Configure MAX30102
  particleSensor.setup(); // Default settings
  particleSensor.setPulseAmplitudeRed(0x0A); // Turn Red LED to low to save power
  particleSensor.setPulseAmplitudeGreen(0);  // Turn off Green LED
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    setupWiFi();
  }
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();

  // Read data from the sensor
  updateSensorData();

  // Display data on LCD
  displayData();
  
  // Publish data to MQTT
  publishData();

  delay(2000); // Adjust delay as needed
}

void setupWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi.");
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (mqttClient.connect("ESP32Client")) {
      Serial.println("Connected to MQTT.");
      
      // Subscribe to topics with QoS 2
      mqttClient.subscribe(heart_rate_topic, 2);
      mqttClient.subscribe(spo2_topic, 2);
      mqttClient.subscribe(temperature_topic, 2);
      
    } else {
      Serial.print("Failed, rc=");
      Serial.println(mqttClient.state());
      delay(5000);
    }
  }
}

void updateSensorData() {
  long redValue = particleSensor.getRed();
  long irValue = particleSensor.getIR();

  // Simulated algorithm for HR and SpO2 (replace with actual calculations or library methods)
  heartRate = 75 + (irValue % 10); // Dummy calculation
  spo2 = 98 - (redValue % 5);      // Dummy calculation

  temperature = particleSensor.readTemperature(); // Measure temperature in Celsius
}

void publishData() {
  boolean success1 = mqttClient.publish(heart_rate_topic, String(heartRate).c_str(), strlen(String(heartRate).c_str()), false, 2);
  boolean success2 = mqttClient.publish(spo2_topic, String(spo2).c_str(), strlen(String(spo2).c_str()), false, 2);
  boolean success3 = mqttClient.publish(temperature_topic, String(temperature).c_str(), strlen(String(temperature).c_str()), false, 2);
  
  if (success1 && success2 && success3) {
    Serial.println("Data published:");
    Serial.println("Heart Rate: " + String(heartRate));
    Serial.println("SpO2: " + String(spo2));
    Serial.println("Temperature: " + String(temperature));
  } else {
    Serial.println("Failed to publish one or more messages");
  }
}


void displayData() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    showTemperature = !showTemperature; // Toggle between screens
  }

  lcd.clear();
  
  if (!showTemperature) {
    // Show HR 
    lcd.setCursor(0, 0);
    lcd.print("HR: ");
    lcd.print(heartRate);
    lcd.print(" BPM");
    // Show SpO2
    lcd.setCursor(0, 1);
    lcd.print("SpO2: ");
    lcd.print(spo2);
    lcd.print("%");
  } else {
    // Show Temperature
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.print(" C");
  }
}
