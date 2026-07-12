# Wheelchair

Smart Wheelchair using Bluetooth

This project controls a small wheelchair prototype using an Arduino, HC-05 Bluetooth module, L298N motor driver, HC-SR04 ultrasonic sensor, and an emergency button.

The phone sends direction commands to the HC-05 over Bluetooth. The HC-05 passes those commands to Arduino using serial communication. Arduino checks the command and moves the motors forward, backward, left, right, or stop.

## Components

| Component | Use |
| --- | --- |
| Arduino Uno/Nano | Main controller |
| HC-05 Bluetooth module | Receives commands from phone |
| L298N motor driver | Drives left and right DC motors |
| HC-SR04 ultrasonic sensor | Detects obstacle in front |
| Push button | Emergency stop |
| Two DC motors | Wheel movement |
| Battery | Motor and Arduino power |

## Wiring

| Arduino Pin | Connect To |
| --- | --- |
| D10 | HC-05 TXD |
| D11 | HC-05 RXD through voltage divider |
| D5 | L298N ENA |
| D6 | L298N IN1 |
| D7 | L298N IN2 |
| D8 | L298N IN3 |
| D9 | L298N IN4 |
| D3 | L298N ENB |
| D12 | HC-SR04 TRIG |
| D13 | HC-SR04 ECHO |
| D2 | Emergency button |
| 5V | HC-05 VCC and HC-SR04 VCC |
| GND | Common ground for all modules |

Connect the emergency button between Arduino `D2` and `GND`. The code uses `INPUT_PULLUP`, so no extra resistor is needed.

## Bluetooth Direction Control

1. Pair the phone with the HC-05 Bluetooth module.
2. The HC-05 password is usually `1234` or `0000`.
3. Open any Bluetooth terminal or Bluetooth voice-control app on the phone.
4. Connect the app to HC-05.
5. Send a command from the app. The Arduino receives it and moves the wheelchair.

For voice control, use a phone app that converts speech to text and sends it through Bluetooth. Speak commands like `forward`, `backward`, `left`, `right`, or `stop`.

## Commands

| Command | Meaning |
| --- | --- |
| `F` or `forward` | Move forward |
| `B`, `back`, or `backward` | Move backward |
| `L` or `left` | Turn left |
| `R` or `right` | Turn right |
| `S` or `stop` | Stop |
| `E` or `emergency` | Emergency stop |
| `reset` | Clear emergency stop |

If the ultrasonic sensor detects an obstacle within `25 cm`, the wheelchair stops while moving forward.

## Upload

1. Open `wheelchair/wheelchair.ino` in Arduino IDE.
2. Select Arduino Uno/Nano from the board menu.
3. Select the correct COM port.
4. Upload the code.
5. If upload fails, disconnect HC-05 TX/RX wires and upload again.

> Note: Use L298N only for a small prototype. For a real wheelchair motor, use a high-current motor driver rated for the motor current.
