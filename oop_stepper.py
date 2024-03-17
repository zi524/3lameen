#!/usr/bin/env python3
import sys
import time
import RPi.GPIO as GPIO

class StepperMotor:
    def __init__(self, step_pins):
        self.step_pins = step_pins
        self.step_count = 8
        self.step_sequence = [[1, 0, 0, 1],
                              [1, 0, 0, 0],
                              [1, 1, 0, 0],
                              [0, 1, 0, 0],
                              [0, 1, 1, 0],
                              [0, 0, 1, 0],
                              [0, 0, 1, 1],
                              [0, 0, 0, 1]]
        self.step_dir = 1  # Set to 1 or -1 for clockwise/anti-clockwise
        self.step_counter = 0
        self.setup_pins()

    def setup_pins(self):
        GPIO.setmode(GPIO.BCM)
        for pin in self.step_pins:
            GPIO.setup(pin, GPIO.OUT)
            GPIO.output(pin, False)

    def step(self, wait_time=10):
        for pin in range(4):
            xpin = self.step_pins[pin]
            if self.step_sequence[self.step_counter][pin] != 0:
                GPIO.output(xpin, True)
            else:
                GPIO.output(xpin, False)

        self.step_counter += self.step_dir
        if self.step_counter >= self.step_count:
            self.step_counter = 0
        elif self.step_counter < 0:
            self.step_counter = self.step_count - 1

        time.sleep(wait_time / 1000.0)

    def run(self, steps, wait_time=10):
        for _ in range(steps):
            self.step(wait_time)

    def cleanup(self):
        GPIO.cleanup()

if __name__ == "__main__":
    step_pins = [17, 22, 23, 24]
    motor = StepperMotor(step_pins)

    try:
        if len(sys.argv) > 1:
            wait_time = int(sys.argv[1])
        else:
            wait_time = 10
        steps = 512  # Specify how many steps to run; adjust as needed
        motor.run(steps, wait_time)
    except KeyboardInterrupt:
        pass
    finally:
        motor.cleanup()
        print("Program exited cleanly")
