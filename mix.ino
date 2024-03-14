#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <math.h>
Adafruit_LSM303_Mag_Unified mag; // Create a LSM303 compass object
// temp is the perivous reading 
//counter is the instant reading
#define ticksPerRevolution 1000
volatile unsigned int temp, counter = 0,l,x,y; //This variable will increase or decrease depending on the rotation of encoder
#define radius 0.05    
void setup() {
  Serial.begin (9600);

  pinMode(2, INPUT_PULLUP); // internal pullup input pin 2 
  
  pinMode(3, INPUT_PULLUP); // internal pullup input pin 3
//Setting up interrupt
  //A rising pulse from encodenren activated ai0(). AttachInterrupt 0 is DigitalPin nr 2 on moust Arduino.
  attachInterrupt(0, ai0, RISING);
   
  //B rising pulse from encodenren activated ai1(). AttachInterrupt 1 is DigitalPin nr 3 on moust Arduino.
  attachInterrupt(1, ai1, RISING);
  
  if (!mag.begin()) {
    Serial.println("Compass not detected. Check wiring!");
    while (1);
  }
  }
   
  void loop() {
  // Send the value of counter
  sensors_event_t event;
  mag.getEvent(&event);
  
  float heading = atan2(event.magnetic.y, event.magnetic.x);
  if (heading < 0)
    heading += 2 * PI;
  if (heading > 2 * PI)
    heading -= 2 * PI;
  
  float angleDegrees = heading * 180 / PI;
  if( counter != temp ){
  temp = counter; 
  }
  l=(2 * (22/7) * radius * temp) / ticksPerRevolution;
  x=l*cos(heading);
  y=l*sin(heading);
  Serial.print ("x:");
  Serial.print(x);
  Serial.print ("y:");
  Serial.print(y);
  }
  /*
  if A phase is rising and B phase is low OR if B phase is rising and B phase is high => counter will increment   
  */ 
  void ai0() {
  // ai0 is activated if DigitalPin nr 2 is going from LOW to HIGH
  // Check pin 3 to determine the direction
  if(digitalRead(3)==LOW) {
  counter++;
  }else{
  counter--;
  }
  }
   
  void ai1() {
  // ai1 is activated if DigitalPin nr 3 is going from LOW to HIGH
  // Check with pin 2 to determine the direction
  if(digitalRead(2)==LOW) {
  counter--;
  }else{
  counter++;
  }
  }