#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>

Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345); // Create a LSM303 compass object with a unique ID

// Calibration values - these will be determined during calibration
int minX = 0, maxX = 0;
int minY = 0, maxY = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Compass Calibration: Rotate the compass in all directions");
  delay(1000); // Short delay before starting calibration
  
  if (!mag.begin()) {
    Serial.println("Compass not detected. Check wiring!");
    while (1);
  }

  // Start calibration
  calibrateCompass();
}

void calibrateCompass() {
  unsigned long startTime = millis();
  while (millis() - startTime < 10000) { // 10-second calibration time
    sensors_event_t event;
    mag.getEvent(&event);
    
    minX = (event.magnetic.x < minX) ? event.magnetic.x : minX;
    maxX = (event.magnetic.x > maxX) ? event.magnetic.x : maxX;
    minY = (event.magnetic.y < minY) ? event.magnetic.y : minY;
    maxY = (event.magnetic.y > maxY) ? event.magnetic.y : maxY;

    delay(100); // Slow down the loop to make it more manageable
  }
  Serial.println("Calibration finished");
}

void loop() {
  sensors_event_t event;
  mag.getEvent(&event);
  
  // Apply calibration
  float calibratedX = event.magnetic.x - ((maxX + minX) / 2);
  float calibratedY = event.magnetic.y - ((maxY + minY) / 2);

  float heading = atan2(calibratedY, calibratedX);
  if (heading < 0)
    heading += 2 * PI;
  if (heading > 2 * PI)
    heading -= 2 * PI;
  
  float angleDegrees = heading * 180 / PI;
  
  Serial.print("Calibrated Angle: ");
  Serial.print(angleDegrees);
  Serial.println(" degrees");
  
  delay(500);
}



pip3 install adafruit-circuitpython-lsm303dlh_mag
sudo apt-get install -y i2c-tools
sudo pip3 install adafruit-blinka
sudo usermod -aG i2c $(whoami)
sudo reboot
import time
import board
import busio
import adafruit_lsm303dlh_mag

# Initialize I2C bus and sensor.
i2c = busio.I2C(board.SCL, board.SDA)
mag_sensor = adafruit_lsm303dlh_mag.LSM303DLH_Mag(i2c)

while True:
    mag_x, mag_y, mag_z = mag_sensor.magnetic

    print('Magnetometer (gauss): ({0}, {1}, {2})'.format(mag_x, mag_y, mag_z))
    time.sleep(1)

