# ATmega32 BrickBreaker

This project implements a version of the classic **BlockBreaker** game using the **ATMega32 microcontroller**. The game features real-time paddle control using a **1-axis joystick** and runs on an embedded system with a graphical display.

---

## 🎮 Features

- BlockBreaker-style paddle and ball gameplay
- One-axis joystick control for paddle movement
- Collision detection with blocks, walls, and paddle

---

## 🧰 Hardware Requirements

- **ATMega32** Microcontroller
- **1-axis Joystick** Analog input via ADC
- **Display Module** OLED via SPI

---

## 🕹 Controls

* **Joystick Left/Right**: Move paddle left or right

Direction and Speed are detected through reading the analogue values.

---

## 🔌 Pin Configuration

| Component   | ATMega32 Pin |
| ----------- | ------------ |
| Joystick X  | PA0          |
| OLCD        | PORTB (SPI)  |
| VCC/GND     | 5V / GND     |

---

## 🧠 Software Details

* **Language:** C (AVR-GCC)
* **Clock Speed:** 8 MHz

---

## 🔧 How to Build & Flash

1. **Build the project**:

   run task "PlatformIO Build"

2. **Flash to ATMega32**:

   connect ATMega32 via JTAG   
   run task "PlatformIO Upload"

---

## 🧑‍💻 Author

Created by Wolfgang Pawelka and Matthias Wurmannstätter.

---

## 📝 License

This project is open-source and available under the GNU General Public License v3.0.

