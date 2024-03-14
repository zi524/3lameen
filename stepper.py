#!/usr/bin/env python3
#libraries
import sys
import time
import RPi.GPIO as GPIO

# Set GPIO mode
GPIO.setmode(GPIO.BCM)

# Define GPIO pins for the stepper motor
StepPins = [17, 22, 23, 24]

# Set all the motor pins to output mode and initialize them to low
for pin in StepPins:
    print("Setup pins")
    GPIO.setup(pin, GPIO.OUT)
    GPIO.output(pin, False)

# Define the step sequence as per the motor's datasheet
Seq = [[1, 0, 0, 1],
       [1, 0, 0, 0],
       [1, 1, 0, 0],
       [0, 1, 0, 0],
       [0, 1, 1, 0],
       [0, 0, 1, 0],
       [0, 0, 1, 1],
       [0, 0, 0, 1]]

StepCount = len(Seq)
StepDir = 1  # Set to 1 or -1 for clockwise/anti-clockwise

# Read wait time from command line or set to default
if len(sys.argv) > 1:
    WaitTime = int(sys.argv[1]) / float(1000)
else:
    WaitTime = 10 / float(1000)
#reset counter
StepCounter = 0

# Start main loop
try:
    while True:
        print(StepCounter, Seq[StepCounter])

        for pin in range(4):
            xpin = StepPins[pin]
            if Seq[StepCounter][pin] != 0:
                print(f"Enable GPIO {xpin}")
                GPIO.output(xpin, True)
            else:
                GPIO.output(xpin, False)

        StepCounter += StepDir

        # If we reach the end of the sequence start again
        if StepCounter >= StepCount:
            StepCounter = 0
        if StepCounter < 0:
            StepCounter = StepCount + StepDir

        # Wait before exit
        time.sleep(WaitTime)

except KeyboardInterrupt:
    # Clean up GPIO 
    GPIO.cleanup()

GPIO.cleanup()

print("Program exited cleanly")
