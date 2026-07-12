#include <SoftwareSerial.h>

/*
  Smart Wheelchair using Bluetooth
  Board: Arduino Uno/Nano
  Bluetooth: HC-05
  Motor driver: L298N
  Sensor: HC-SR04 ultrasonic sensor
*/

SoftwareSerial bluetooth(10, 11); // RX, TX\

// L298N motor driver pins
const int enA = 5;
const int in1 = 6;
const int in2 = 7;
const int in3 = 8;
const int in4 = 9;
const int enB = 3;

// Ultrasonic sensor pins
const int trigPin = 12;
const int echoPin = 13;

// Emergency button pin
const int emergencyPin = 2;

const int motorSpeed = 180;       // PWM speed: 0 to 255
const int stopDistance = 25;      // Stop if obstacle is within 25 cm

bool emergencyStop = false;
char currentDirection = 'S';

void setup() {
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(emergencyPin, INPUT_PULLUP);

  Serial.begin(9600);
  bluetooth.begin(9600);
  bluetooth.setTimeout(80);

  stopWheelchair();
  bluetooth.println("Smart wheelchair ready");
}

void loop() {
  if (digitalRead(emergencyPin) == LOW) {
    emergencyStop = true;
    stopWheelchair();
  }

  if (currentDirection == 'F' && getDistance() <= stopDistance) {
    stopWheelchair();
    bluetooth.println("Obstacle detected");
  }

  if (bluetooth.available()) {
    String command = bluetooth.readStringUntil('\n');
    command.trim();
    command.toLowerCase();
    runCommand(command);
  }
}

void runCommand(String command) {
  if (command == "reset") {
    emergencyStop = false;
    stopWheelchair();
    bluetooth.println("Emergency reset");
    return;
  }

  if (command == "e" || command.indexOf("emergency") >= 0) {
    emergencyStop = true;
    stopWheelchair();
    bluetooth.println("Emergency stop");
    return;
  }

  if (emergencyStop) {
    stopWheelchair();
    bluetooth.println("Send reset first");
    return;
  }

  if (command == "f" || command.indexOf("forward") >= 0) {
    if (getDistance() > stopDistance) {
      forward();
    } else {
      stopWheelchair();
      bluetooth.println("Forward blocked");
    }
  } else if (command == "b" || command.indexOf("back") >= 0) {
    backward();
  } else if (command == "l" || command.indexOf("left") >= 0) {
    left();
  } else if (command == "r" || command.indexOf("right") >= 0) {
    right();
  } else if (command == "s" || command.indexOf("stop") >= 0) {
    stopWheelchair();
  } else {
    stopWheelchair();
  }
}

void forward() {
  currentDirection = 'F';
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  setSpeed();
}

void backward() {
  currentDirection = 'B';
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  setSpeed();
}

void left() {
  currentDirection = 'L';
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  setSpeed();
}

void right() {
  currentDirection = 'R';
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  setSpeed();
}

void stopWheelchair() {
  currentDirection = 'S';
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void setSpeed() {
  analogWrite(enA, motorSpeed);
  analogWrite(enB, motorSpeed);
}

long getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 25000);

  if (duration == 0) {
    return 999;
  }

  return duration / 58;
}
