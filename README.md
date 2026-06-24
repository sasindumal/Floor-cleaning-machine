# 🤖 Autonomous Floor Cleaning Machine — R1

> **Design Prototype Project** | Group R1 | Sasindu Malhara (2022/E/126)

An autonomous floor-cleaning robot designed and built from scratch, featuring dual-mode operation (manual Bluetooth control and ML-powered automatic navigation), custom mechanical design, CoppeliaSim simulation, and a machine learning model embedded directly into Arduino firmware.

---

## 📖 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [System Architecture](#system-architecture)
- [Hardware Components](#hardware-components)
- [Software & Tech Stack](#software--tech-stack)
- [Project Structure](#project-structure)
- [Operating Modes](#operating-modes)
  - [Manual Mode (Bluetooth)](#manual-mode-bluetooth)
  - [Automatic Mode (ML-based)](#automatic-mode-ml-based)
- [Machine Learning Model](#machine-learning-model)
- [Simulation](#simulation)
- [Mechanical Design](#mechanical-design)
- [Code Versions](#code-versions)
- [Pin Configuration](#pin-configuration)
- [Bluetooth Command Reference](#bluetooth-command-reference)
- [Setup & Deployment](#setup--deployment)
- [Reinforcement Learning Notes](#reinforcement-learning-notes)
- [Videos & Demos](#videos--demos)
- [Author](#author)

---

## Overview

The **Autonomous Floor Cleaning Machine R1** is a prototype robotic system capable of navigating indoor environments while performing floor-cleaning tasks. The robot integrates:

- A **dual H-bridge motor driver** (L298N) for differential drive
- **4 ultrasonic sensors** for 360° obstacle detection
- A **Bluetooth module (HC-05)** for wireless manual control
- A **machine learning model** trained on sensor data and compiled into C++ to run directly on an Arduino Uno
- A **roller brush**, **mop**, **water pump**, and **dryer** for complete floor-cleaning functionality
- **SolidWorks CAD** drawings and **CoppeliaSim** simulation models for design validation

---

## Features

- ✅ **Manual Bluetooth Control** — real-time directional commands via serial/Bluetooth
- ✅ **Autonomous Obstacle-Avoidance Navigation** — ML model predicts movement decisions from ultrasonic sensor readings
- ✅ **Embedded ML on Arduino** — trained model converted to C++ (`predict()` function) and flashed to Arduino Uno
- ✅ **Complete Cleaning System** — roller brush, mop motor, water pump, and dryer actuators
- ✅ **Dual-mode switching** — seamlessly switch between manual and auto mode over Bluetooth
- ✅ **CoppeliaSim Simulation** — multiple simulation versions for testing before hardware deployment
- ✅ **SolidWorks CAD** — full mechanical assembly with custom parts
- ✅ **URDF Models** — ROS-compatible robot description files (3 versions)

---

## System Architecture

```
┌─────────────────────────────────────────────────────────┐
│                     Arduino Uno                         │
│                                                         │
│  ┌──────────┐   ┌──────────────┐   ┌─────────────────┐ │
│  │ Bluetooth│   │  Ultrasonic  │   │  ML predict()   │ │
│  │  HC-05   │──▶│  4x Sensors  │──▶│  (C++ model)    │ │
│  └──────────┘   └──────────────┘   └────────┬────────┘ │
│                                             │           │
│  ┌──────────────────────────────────────────▼────────┐  │
│  │              Motor Control (L298N)                │  │
│  │         Left Motor  |  Right Motor                │  │
│  └──────────────────────────────────────────────────┘  │
│                                                         │
│  ┌──────────┐  ┌─────┐  ┌──────┐  ┌─────┐             │
│  │  Roller  │  │ MOP │  │ Pump │  │ Dry │             │
│  │  Brush   │  │Motor│  │      │  │     │             │
│  └──────────┘  └─────┘  └──────┘  └─────┘             │
└─────────────────────────────────────────────────────────┘
```

---

## Hardware Components

| Component | Model / Spec | Purpose |
|---|---|---|
| Microcontroller | Arduino Uno | Main controller |
| Motor Driver | L298N Dual H-Bridge | Controls left & right drive motors |
| Drive Motors | DC Motors (x2) | Differential drive locomotion |
| Ultrasonic Sensors | HC-SR04 (x4) | Front-left, front-right, left, right obstacle detection |
| Bluetooth Module | HC-05 | Wireless serial communication |
| Roller Brush Motor | DC Motor | Floor brushing/sweeping |
| Mop Motor | DC Motor | Wet mopping actuation |
| Water Pump | KDP-370EB 12V | Water dispensing for mopping |
| Dryer | DC Motor | Floor drying after mopping |
| Battery | KT-1213 12V 1.3Ah AGM Lead-Acid | Main power supply |
| Chassis | Custom SolidWorks design | Machine body & mounting |
| Wheels | 85mm custom wheels | Locomotion |
| Vacuum Fan | 80mm vacuum fan | Debris suction |

---

## Software & Tech Stack

| Layer | Technology |
|---|---|
| Firmware | Arduino C++ (.ino) |
| ML Training | Python, Jupyter Notebook, scikit-learn / neural network |
| ML Deployment | C++ model code embedded in Arduino firmware |
| Simulation | CoppeliaSim (.ttt scene files) |
| CAD Design | SolidWorks (.SLDPRT, .SLDASM) |
| Robot Description | URDF (ROS-compatible) |
| Data | CSV dataset for ML training |
| RL Research | Unity ML-Agents (referenced in RL notes) |

---

## Project Structure

```
02 Floor-cleaning-machine/
│
├── Code/                                    # All firmware and software
│   ├── Floor-Cleanig-machine-R1/
│   │   └── Floor-Cleanig-machine-R1.ino    # Revision 1 firmware
│   ├── Floor-Cleanig-machine-R1-Final/
│   │   └── Floor-Cleanig-machine-R1-Final.ino  # Final production firmware
│   ├── Floor_Cleanig_machine_R1_Final_test/
│   │   └── (test sketches)
│   ├── ML/
│   │   ├── Floor-Cleaning-Machine-ML.ipynb  # ML training notebook
│   │   ├── Floor cleaning machine ML dataset.csv  # Training dataset
│   │   ├── model.json                        # Trained model (JSON)
│   │   ├── Floor-Cleaning-Machine-ML.pdf    # ML documentation
│   │   ├── c++ model code.txt               # C++ exported model code
│   │   └── cpp-model-check.cpp              # C++ model integration test
│   ├── US_check/
│   │   └── US_check.ino                     # Ultrasonic sensor test sketch
│   ├── check-sensors/
│   │   └── check-sensors.ino                # Sensor + motor diagnostic sketch
│   ├── Code 2.0.txt                         # Code snapshot v2.0
│   └── Code 3.0.txt                         # Code snapshot v3.0
│
├── Machine/                                 # Mechanical design files
│   ├── CoppeliaSim/
│   │   ├── Machine.ttt                      # Simulation scene v1
│   │   ├── Machine 2.0.ttt                  # Simulation scene v2
│   │   ├── Machine 3.0.ttt                  # Simulation scene v3
│   │   ├── Machine 4.0.ttt                  # Simulation scene v4
│   │   ├── Machine 5.0.ttt                  # Simulation scene v5
│   │   └── Machine-ML/                      # ML-enabled simulation
│   ├── Drawing/                             # SolidWorks parts & assembly
│   │   ├── Machine.SLDASM                   # Full machine assembly
│   │   ├── Body.SLDPRT                      # Machine body
│   │   ├── Arduino Uno.SLDPRT              # Arduino Uno model
│   │   ├── HCSR04 Ultrasonic Sensor.SLDPRT # HC-SR04 model
│   │   ├── L298N_Stepper_motor_driver.SLDPRT
│   │   ├── Roller Brush.SLDPRT
│   │   ├── 85mm_Wheel.SLDPRT
│   │   ├── KT-1213 Battery.SLDASM          # Battery assembly
│   │   └── ... (27 component files total)
│   ├── URDF/                                # ROS robot description v1
│   ├── URDF 2.0/                            # ROS robot description v2
│   └── URDF 3.0/                            # ROS robot description v3
│
├── RL/
│   └── Rl steps.txt                         # Reinforcement learning research notes
│
├── Videos/                                  # Physical build & simulation demos
│   ├── Simulation.mkv                       # CoppeliaSim simulation video
│   ├── Cad design.mkv                       # SolidWorks CAD walkthrough
│   └── VID-20240806-*.mp4                   # Physical prototype test videos
│
├── .gitignore
└── README.md
```

---

## Operating Modes

### Manual Mode (Bluetooth)

The robot defaults to **manual mode** on startup. Commands are sent via the HC-05 Bluetooth module (serial at 9600 baud).

The robot receives a single character over serial and executes the corresponding action:

| Command | Action |
|---------|--------|
| `F` | Move **Forward** + activate pump |
| `f` | Stop (forward stop) |
| `B` | Move **Backward** |
| `b` | Stop (backward stop) |
| `L` | Turn **Left** |
| `l` | Stop (left stop) |
| `R` | Turn **Right** |
| `r` | Stop (right stop) |
| `S` | **Stop** all drive motors |
| `N` | Turn **Pump ON** |
| `n` | Turn **Pump OFF** |
| `M` | Turn **Mop Motor ON** |
| `m` | Turn **Mop Motor OFF** |
| `K` | Turn **Roller Brush ON** |
| `k` | Turn **Roller Brush OFF** |
| `D` | Turn **Dryer ON** |
| `d` | Turn **Dryer OFF** |
| `A` | Switch to **Automatic Mode** |

### Automatic Mode (ML-based)

Activated by sending `A` over Bluetooth. The robot:

1. **Reads** all 4 ultrasonic sensors (front-left, front-right, left, right)
2. **Feeds** the sensor distances into the embedded `predict()` function (C++ ML model)
3. **Executes** the predicted movement command:

| Prediction | Action |
|---|---|
| `0` | Turn Left |
| `1` | Turn Right |
| `2` | Move Backward |
| `3` | Move Forward |

Send `X` to exit automatic mode and return to manual control.

---

## Machine Learning Model

The navigation decision model was trained in Python using sensor distance data.

### Training Pipeline

1. **Dataset**: `Floor cleaning machine ML dataset.csv` — collected sensor readings labeled with correct movement decisions
2. **Training**: Jupyter Notebook (`Floor-Cleaning-Machine-ML.ipynb`) — model trained using scikit-learn / neural network
3. **Export**: Model exported to `model.json` and converted to C++ code (`c++ model code.txt`)
4. **Deployment**: The C++ `predict(float[])` function is compiled directly into Arduino firmware — **no external libraries or runtime needed**

### Inputs / Outputs

```
Inputs:  [front_distance, left_distance, right_distance]  (float, in cm)
Output:  int prediction -> { 0: Left, 1: Right, 2: Backward, 3: Forward }
```

---

## Simulation

CoppeliaSim simulation scenes are located in `Machine/CoppeliaSim/`. Multiple versions were developed iteratively:

| File | Version | Description |
|---|---|---|
| `Machine.ttt` | v1.0 | Initial simulation |
| `Machine 2.0.ttt` | v2.0 | Updated physics |
| `Machine 3.0.ttt` | v3.0 | Sensor integration |
| `Machine 4.0.ttt` | v4.0 | Full sensor + motor simulation |
| `Machine 5.0.ttt` | v5.0 | Final simulation |
| `Machine-ML/` | ML | ML-integrated simulation |

A simulation demo video is available at `Videos/Simulation.mkv`.

---

## Mechanical Design

The machine body was designed in **SolidWorks** with a full assembly (`Machine.SLDASM`) comprising 27 custom and standard parts:

- Custom body chassis and mounting brackets
- DC motor + bracket assemblies
- 85mm drive wheels
- Roller brush assembly
- HC-SR04 ultrasonic sensor mounts
- L298N motor driver mount
- Arduino Uno and HC-05 Bluetooth module placements
- 12V AGM lead-acid battery compartment
- 80mm vacuum fan housing

A CAD design walkthrough video is available at `Videos/Cad design.mkv`.

**URDF models** (3 versions) are provided for ROS-based simulation or further robot modeling.

---

## Code Versions

| Version | File | Description |
|---|---|---|
| v1.0 (R1) | `Code/Floor-Cleanig-machine-R1/Floor-Cleanig-machine-R1.ino` | Initial prototype firmware |
| v2.0 | `Code/Code 2.0.txt` | Intermediate version snapshot |
| v3.0 | `Code/Code 3.0.txt` | Feature-complete snapshot |
| v1-Final | `Code/Floor-Cleanig-machine-R1-Final/Floor-Cleanig-machine-R1-Final.ino` | **Production firmware with embedded ML** |
| Test | `Code/Floor_Cleanig_machine_R1_Final_test/` | Test sketches |
| Sensor check | `Code/check-sensors/check-sensors.ino` | Sensor + motor diagnostic |
| US check | `Code/US_check/US_check.ino` | Ultrasonic-only diagnostic |

---

## Pin Configuration

### Drive Motors (via L298N)

| Pin | Arduino | Function |
|---|---|---|
| `L1` | D2 | Left motor direction A |
| `L2` | D4 | Left motor direction B |
| `LENB` | D3 (PWM) | Left motor enable (speed) |
| `R1` | D5 (PWM) | Right motor direction A |
| `R2` | D7 | Right motor direction B |
| `RENB` | D6 (PWM) | Right motor enable (speed) |

### Cleaning Actuators

| Pin | Arduino | Function |
|---|---|---|
| `Roller` | D8 | Roller brush motor |
| `MOP` | D9 | Mop motor |
| `Pump` | D12 | Water pump |
| `Dry` | D11 | Dryer motor |

### Ultrasonic Sensors (HC-SR04)

| Sensor | Trig Pin | Echo Pin |
|---|---|---|
| Front-Left (`Usfl`) | A1 | A0 |
| Front-Right (`Usfr`) | D13 | D10 |
| Left (`Usl`) | A3 | A2 |
| Right (`Usr`) | A5 | A4 |

---

## Bluetooth Command Reference

Connect a Bluetooth terminal app (e.g., **Serial Bluetooth Terminal** on Android) to the HC-05 module:

- **Baud rate**: `9600`
- **Device name**: `HC-05` (default)
- **Pairing PIN**: `1234` or `0000`

Send single characters as listed in the [Manual Mode](#manual-mode-bluetooth) section above.

---

## Setup & Deployment

### Prerequisites

- [Arduino IDE](https://www.arduino.cc/en/software) (v1.8+ or v2.x)
- [CoppeliaSim](https://www.coppeliarobotics.com/) (for simulation)
- [SolidWorks](https://www.solidworks.com/) (for CAD files)
- Python 3.x + Jupyter Notebook (for ML training)

### Flash Firmware

1. Open `Code/Floor-Cleanig-machine-R1-Final/Floor-Cleanig-machine-R1-Final.ino` in Arduino IDE
2. Select **Board**: `Arduino Uno`
3. Select the correct **Port** for your connected Arduino
4. Click **Upload**

### Run Simulation

1. Open CoppeliaSim
2. Load `Machine/CoppeliaSim/Machine 5.0.ttt` (latest simulation)
3. Press **Play** to start the simulation

### Retrain ML Model (Optional)

1. Open `Code/ML/Floor-Cleaning-Machine-ML.ipynb` in Jupyter Notebook
2. Ensure the dataset `Floor cleaning machine ML dataset.csv` is in the same directory
3. Run all cells to retrain and export the model
4. Copy the generated C++ `predict()` function into the main `.ino` firmware file

---

## Reinforcement Learning Notes

The `RL/Rl steps.txt` file documents research into using **Reinforcement Learning** (RL) as an alternative navigation approach:

- **Simulation environment**: Unity with ML-Agents Toolkit
- **Algorithm considered**: PPO (Proximal Policy Optimization), DQN (Deep Q-Networks)
- **Reward design**: Obstacle avoidance + staying on track
- **Deployment challenge**: Converting RL model for Arduino's limited hardware

> The current production firmware uses a **supervised ML model** (trained on labeled sensor data) rather than RL, as it is more practical for Arduino deployment.

---

## Videos & Demos

| File | Description |
|---|---|
| `Videos/Simulation.mkv` | CoppeliaSim simulation demo |
| `Videos/Cad design.mkv` | SolidWorks CAD design walkthrough |
| `Videos/VID-20240806-WA0002.mp4` | Physical prototype test |
| `Videos/VID-20240806-WA0009.mp4` | Obstacle avoidance test |
| `Videos/VID-20240806-WA0015.mp4` | Full cleaning run demo |
| `Videos/2024-08-08 08-30-12.mkv` | Final build test session |

---

## Author

**Sasindu Malhara** — `2022/E/126`
Group R1 — Design Prototype Project

---

*This project was developed as a design prototype for academic purposes. All code and design files are owned by Group R1.*