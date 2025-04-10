#include <Wire.h> // Include the Wire library for I2C
#include <LiquidCrystal.h> // Include the LCD library
#include <SoftwareSerial.h> // Include the SoftwareSerial library
#include <XBee.h>

// Define the pins for the ultrasonic sensor
const int trigPin = 9;
const int echoPin = 10;

// Define the pin for the water flow sensor
const int flowPin = 8;

// Define pins for LiquidCrystal lcd
const int rs = 12;
const int en = 11;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;

// Define the LCD dimensions
const int lcdColumns = 16;
const int lcdRows = 2;

// Initialize LCD screen
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Create an instance of the XBee class
XBee xbee;
// Create a SoftwareSerial object for communication with the XBee module
SoftwareSerial xbeeSerial(2, 3); // RX, TX (avoid using pins 0 and 1)

void setup() {
  // Initialize the serial communication
  Serial.begin(9600);
  // Initialize the XBee module
  xbeeSerial.begin(9600);
  // Initialize the LCD
  lcd.begin(lcdColumns, lcdRows);
  lcd.clear();
  lcd.setCursor(0, 0);
  // Set the pin modes
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(flowPin, INPUT);
}

void loop() {
  // Read the water level from the ultrasonic sensor
  float waterLevel = readWaterLevel();
  // Read the water pressure from the water flow sensor
  float waterPressure = readWaterPressure();
  // Print the data to the serial monitor
  Serial.print("Water Level: ");
  Serial.print(waterLevel);
  Serial.print(" cm\t");
  Serial.print("Water Pressure: ");
  Serial.print(waterPressure);
  Serial.println(" psi");
  // Display the data on the LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("W Level: ");
  lcd.print(waterLevel);
  lcd.print(" cm");
  lcd.setCursor(0, 1);
  lcd.print("W Pressure: ");
  lcd.print(waterPressure);
  lcd.print(" psi");
  // Send the data to the XBee module in transparent mode
  sendDataToXBee(waterLevel, waterPressure);
  delay(1000);
}

float readWaterLevel() {
  // Send a pulse to the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Read the duration of the echoPin pulse
  float duration = pulseIn(echoPin, HIGH);
  // Calculate the water level based on the speed of sound
  float waterLevel = duration * 0.034 / 2;
  return waterLevel;
}

float readWaterPressure() {
  // Read the frequency from the flowPin
  int frequency = pulseIn(flowPin, HIGH);
  // Convert the frequency to water pressure (adjust based on your sensor's specifications)
  float waterPressure = frequency * 0.132; // Example calibration value
  return waterPressure;
}

void sendDataToXBee(float waterLevel, float waterPressure) {
  xbeeSerial.print("Water Level: ");
  xbeeSerial.print(waterLevel);
  xbeeSerial.print(" cm, ");
  xbeeSerial.print("Water Pressure: ");
  xbeeSerial.print(waterPressure);
  xbeeSerial.println(" psi");
}
