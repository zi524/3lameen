#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>

Adafruit_LSM303_Mag_Unified mag; // Create a LSM303 compass object

void setup() {
  Serial.begin(9600);
  if (!mag.begin()) {
    Serial.println("Compass not detected. Check wiring!");
    while (1);
  }
}

void loop() {
  sensors_event_t event;
  mag.getEvent(&event);
  
  float heading = atan2(event.magnetic.y, event.magnetic.x);
  if (heading < 0)
    heading += 2 * PI;
  if (heading > 2 * PI)
    heading -= 2 * PI;
  
  float angleDegrees = heading * 180 / PI;
  
  Serial.print("Angle: ");
  Serial.print(angleDegrees);
  Serial.println(" degrees");
  
  delay(500);
}