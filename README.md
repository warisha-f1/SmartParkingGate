# 🚗 Smart Parking Gate

An Arduino-based Smart Parking Gate System that automatically detects vehicles using an HC-SR04 ultrasonic sensor, controls a servo motor barrier, displays real-time status on a 16×2 I2C LCD, and provides buzzer alerts using a Finite State Machine (FSM).

## 📌 Features

- 🚘 Automatic vehicle detection using HC-SR04
- 🚧 Automatic gate opening and closing
- 📟 Real-time status on 16×2 I2C LCD
- 🔊 Buzzer indication during gate operation
- ⚙️ Finite State Machine (FSM) implementation
- 💻 Serial Monitor debugging
- 🔧 Modular and well-structured Arduino code

## 🛠️ Components Used

| Component | Quantity |
|----------|:--------:|
| Arduino Uno | 1 |
| HC-SR04 Ultrasonic Sensor | 1 |
| SG90 Servo Motor | 1 |
| 16×2 LCD with I2C Backpack | 1 |
| Active Buzzer | 1 |
| Breadboard | 1 |
| Jumper Wires | As required |
| USB Cable | 1 |

## 🔌 Pin Connections

| Component | Arduino Pin |
|-----------|-------------|
| HC-SR04 Trig | D9 |
| HC-SR04 Echo | D10 |
| Servo Signal | D6 |
| Buzzer | D5 |
| LCD SDA | A4 (SDA) |
| LCD SCL | A5 (SCL) |
| LCD VCC | 5V |
| LCD GND | GND |

### LCD Backlight Fix

> **Note:** During testing, the I2C backpack's backlight circuit stopped powering the LCD backlight. The issue was resolved by connecting:
>
> - LCD **BLA → Arduino 5V**
> - LCD **BLK → Arduino GND**
>
> The I2C backpack is still used for data communication (SDA/SCL), while the backlight is powered directly from the Arduino.

# ⚙️ Working Principle

1. The HC-SR04 ultrasonic sensor continuously measures the distance to detect an approaching vehicle.
2. When a vehicle is detected within the predefined threshold distance, the servo motor rotates to open the gate.
3. The LCD displays the current gate status and measured distance.
4. A buzzer beeps whenever the gate opens or closes.
5. The gate remains open while the vehicle is present.
6. Once the vehicle leaves the detection area, a timer starts.
7. After the timer expires, the gate automatically closes.

# 🔄 Finite State Machine (FSM)

The project is implemented using a Finite State Machine (FSM) with four states:

- **IDLE** – Waiting for a vehicle.
- **OPEN** – Opens the gate when a vehicle is detected.
- **WAIT** – Keeps the gate open while the vehicle is passing.
- **CLOSE** – Closes the gate after the vehicle has left.

State transition:

IDLE
  │
Vehicle Detected
  │
  ▼
OPEN
  │
  ▼
WAIT
  │
Vehicle Leaves + Timer
  ▼
CLOSE
  │
  ▼
IDLE

# 🚀 Future Improvements

- RFID-based vehicle authentication
- Automatic vehicle counting
- IoT monitoring using ESP32
- Cloud-based parking management
- Mobile application integration
- Automatic payment system
