#include <Arduino.h>
#include <ESP32Servo.h>

Servo myServo;

void startCalibration();

const int photoResistorPin = 2;   // Photoresistor pin
const int servoPin = 27;          // Servo motor pin
const int ledPin = 13;            // LED pin
int maxLight = 0;                 // Max light value during calibration
int minLight = 4095;              // Min light value during calibration
unsigned long startTime;

void setup() {
  Serial.begin(9600);
  myServo.attach(servoPin);       // Attach servo motor to the specified pin
  pinMode(ledPin, OUTPUT);        // Set LED pin as OUTPUT
  startCalibration();             // Start calibration
}

void loop() {
  // After calibration, read the photoresistor value continuously and move servo
  int lightValue = analogRead(photoResistorPin);

  // Map light values between minLight and maxLight to servo angles (0° to 180°)
  int servoPosition = map(lightValue, minLight, maxLight, 0, 180);
  
  // Ensure servo position is within bounds
  servoPosition = constrain(servoPosition, 0, 180);

  // Move the servo
  myServo.write(servoPosition);

  // Debug: Print the mapped servo position
  Serial.print("Light Value: ");
  Serial.print(lightValue);
  Serial.print(" -> Servo Position: ");
  Serial.println(servoPosition);

  delay(100);  // Adjust as necessary
}

void startCalibration() {
  delay(5000);
  Serial.println("Starting Calibration...");
  startTime = millis();

  // Calibration phase lasts for 10 seconds
  while (millis() - startTime < 10000) {
    int lightValue = analogRead(photoResistorPin);  // Read the photoresistor value
    
    // Update max and min light values during calibration
    if (lightValue > maxLight) {
      maxLight = lightValue;
    }
    if (lightValue < minLight) {
      minLight = lightValue;
    }

    // Blink the LED during calibration (ON and OFF every 500ms)
    digitalWrite(ledPin, HIGH);   // Turn on LED
    delay(250);
    digitalWrite(ledPin, LOW);    // Turn off LED
    delay(250);
  }

  // Calibration done, print the results
  Serial.println("Calibration Complete.");
  Serial.print("Max Light: ");
  Serial.println(maxLight);
  Serial.print("Min Light: ");
  Serial.println(minLight);
}
