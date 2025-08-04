# 🦾 Marion County Robotic Arm – ESP32 Control System

This repository contains the control code for a custom-built robotic utility arm designed for Marion County, Florida. The system is powered by an ESP32 microcontroller and controlled via a PS4 controller. It integrates four linear actuators and one brushed DC motor for full arm articulation, including rotation and object manipulation.

---

## 📦 System Overview

* **Microcontroller:** ESP32 DevKit V1
* **Motor Drivers:** 4x IBT-2 Dual H-Bridge Motor Drivers
* **Power Source:**

  * 12V 12Ah Battery (motors and buck converter)
  * 5V regulated output (for ESP32, via buck converter)
* **Controller:** PlayStation 4 (Bluetooth)
* **Software:** Arduino IDE
* **CAD Design:** Fusion 360, Tinkercad
* **Main Features:**

  * Swivel base motor with 100 lb torque gearbox
  * Two-arm sections powered by high-load linear actuators (\~250 lb capacity)
  * Dual clamp actuators (\~50 lb each)
  * Mode switching via Share button (slow/medium/fast)

---

## 🎮 Controls (via PS4 Controller)

| Button             | Action                                                      |
| ------------------ | ----------------------------------------------------------- |
| **R1**             | Close clamp hand                                            |
| **L1**             | Open clamp hand                                             |
| **R2**             | Rotate arm clockwise                                        |
| **L2**             | Rotate arm counter-clockwise                                |
| **Left Joystick**  | Shoulder up/down                                            |
| **Right Joystick** | Forearm up/down                                             |
| **X**              | Toggle onboard LED                                          |
| **Share**          | Switch speed mode (Green = Slow, Blue = Medium, Red = Fast) |
| **PS Button**      | Connect controller to ESP32                                 |

---

## ⚡ Wiring Summary

Each **IBT-2 Motor Driver** connects to the ESP32 as follows:

* **Shared Connections (all motor drivers):**

  * `VCC`, `R_EN`, `L_EN` → Tied together to ESP32 3.3V
  * `GND` → Tied to ESP32 GND
  * `B+ / B-` → Connected to 12V Battery
  * `M+ / M-` → Connected to motor leads

* **PWM Control Pins:**

| Motor        | R\_PWM | L\_PWM |
| ------------ | ------ | ------ |
| Swivel Base  | D23    | D22    |
| Shoulder Arm | D21    | D19    |
| Forward Arm  | D32    | D33    |
| Clamp (both) | D25    | D26    |

> 🔧 **Note:** M+/M− polarity may vary per motor depending on rotation direction.

* **Powering the ESP32:**

  * A **DC-DC buck converter** steps down the 12V battery to a **regulated 5V output**, which is then connected to the `VIN` and `GND` pins on the ESP32.
  * **⚠ Do NOT power via the 3.3V pin** — this is an output-only pin.
  * Ensure the buck converter provides a clean, regulated 5V supply to avoid damaging the microcontroller.

---

## 🚨 Setup & Safety Guidelines

1. **Inspect wiring** before each use for damage or exposed leads.
2. **Power the ESP32** (via the 5V buck converter), then the 12V battery for motor power.
3. **Connect PS4 controller** by pressing the PS button. LED indicates current speed mode.
4. **Use caution** in “Fast” mode (Red LED) — ensure surroundings are clear.
5. **Disconnect batteries** after use, especially for long-term storage.
6. **Store** in a dry, cool area away from sunlight and moisture. Cover to prevent dust.

---

## 📁 Files

* `MC_Robot.ino` – Primary control sketch for the ESP32
* `Buttons.h` – Header file defining constants for button, D-pad, and miscellaneous PS4 controller inputs
* `README.md` – This documentation

---

## 🧰 Additional Notes

* Code written in Arduino C++ using standard ESP32 and PS4 controller libraries
* No physical power switch — system powers on when batteries are connected
* Built and maintained as a team effort in collaboration with Marion County

