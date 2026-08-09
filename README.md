CODE_baby_ai.ino: Code for arduino ide
baby_ai.py: Python code in Visual Studio Code

# 🍼 Smart Baby Monitor & Smart Cot Safety System

An IoT-based smart baby monitoring system designed to improve baby safety by monitoring environmental conditions, detecting potentially unsafe situations, and providing real-time alerts and automatic responses.

## 📌 Project Overview

The **Smart Baby Monitor & Smart Cot Safety System** combines environmental sensing, AI-based detection, and IoT technology to create a safer and smarter monitoring system for babies.

The system continuously monitors the baby's environment and detects conditions that may require attention. When an unsafe situation is detected, the system provides local alerts through LEDs and a buzzer while also sending notifications through **Blynk** when an internet connection is available.

The system can also monitor temperature and automatically activate a cooling fan when the temperature becomes too high.

## 🎯 Problem Statement

Traditional baby monitoring systems may require continuous supervision and may not provide automatic responses to potentially unsafe conditions.

Babies may be exposed to situations such as:

* Unsafe sleeping positions
* Face obstruction
* Excessive temperature
* Other abnormal conditions requiring attention

Therefore, this project aims to develop an affordable and intelligent monitoring system that can detect potential safety problems, provide immediate alerts, and automatically respond to environmental changes.

## 🎯 Objectives

The main objectives of this project are:

1. To monitor the baby's surrounding temperature in real time.
2. To detect potentially unsafe conditions using AI-based image detection.
3. To provide immediate local alerts using LEDs and a buzzer.
4. To automatically activate a fan when the temperature exceeds the set limit.
5. To display the current safety status and temperature on an LCD.
6. To send remote notifications through Blynk when an unsafe condition is detected.
7. To maintain basic monitoring functionality even when Wi-Fi is unavailable.

## ✨ Main Features

### 🤖 AI Safety Detection

A camera and AI model are used to analyze the baby's condition and identify predefined safe or unsafe states.

The AI detection system is developed using:

* Python
* OpenCV
* Google Teachable Machine
* A trained image classification model

### 🌡️ Temperature Monitoring

A **DHT11 temperature sensor** continuously measures the surrounding temperature.

The system uses a predefined temperature threshold to determine when additional cooling is required.

### 🌬️ Automatic Fan Control

When the detected temperature reaches the configured threshold:

```text
Temperature too high
        ↓
   Fan activated
        ↓
Temperature monitored
```

When the temperature returns to a suitable level, the fan can be switched off automatically.

### 🚨 Safety Alerts

When an unsafe condition is detected:

* 🔴 Red LED indicates an unsafe state.
* 🔊 Buzzer provides an audible warning.
* 📱 Blynk can send a notification when Wi-Fi is available.

When the environment is safe:

* 🟢 Green LED indicates a safe state.
* 🔇 Buzzer remains inactive.

### 📺 LCD Display

The LCD provides important information locally without requiring a phone or computer.

Example:

```text
SAFE
T: 28C FAN: OFF
```

or:

```text
UNSAFE!
T: 29C FAN: OFF
```

This allows the user to check the system status directly from the device.

### 📱 IoT Monitoring with Blynk

Blynk is used to connect the monitoring system to a mobile device.

The system can provide:

* Safety status
* Temperature information
* Remote notifications
* IoT dashboard monitoring

If Wi-Fi is unavailable, the local hardware functions can continue operating independently.

---

# 🧩 System Architecture

```text
                    ┌─────────────────────┐
                    │       Camera        │
                    └──────────┬──────────┘
                               │
                               ▼
                    ┌─────────────────────┐
                    │   AI Detection      │
                    │ Python + OpenCV     │
                    └──────────┬──────────┘
                               │
                         Safe / Unsafe
                               │
                               ▼
                    ┌─────────────────────┐
                    │   ESP8266 Control   │
                    │       System        │
                    └──────┬──────┬───────┘
                           │      │
              ┌────────────┘      └────────────┐
              ▼                                ▼
      ┌────────────────┐              ┌────────────────┐
      │   DHT11 Sensor │              │     Blynk      │
      │  Temperature   │              │ Notifications  │
      └───────┬────────┘              └────────────────┘
              │
              ▼
      ┌────────────────┐
      │  Control Logic  │
      └───────┬────────┘
              │
       ┌──────┼───────────┐
       ▼      ▼           ▼
    ┌─────┐ ┌──────┐   ┌──────┐
    │ LCD │ │ LEDs │   │Buzzer│
    └─────┘ └──────┘   └──────┘
                           │
                           │
                           ▼
                     ┌──────────┐
                     │   Fan    │
                     └──────────┘
```

---

# 🔧 Hardware Components

| Component                     | Purpose                                                     |
| ----------------------------- | ----------------------------------------------------------- |
| **ESP8266 NodeMCU**           | Main hardware controller                                    |
| **DHT11**                     | Temperature sensing                                         |
| **16×2 I2C LCD**              | Displays temperature and safety status                      |
| **Green LED**                 | Indicates safe condition                                    |
| **Red LED**                   | Indicates unsafe condition                                  |
| **Buzzer**                    | Provides an audible safety alert                            |
| **DC Fan**                    | Provides automatic cooling                                  |
| **Transistor**                | Controls the fan from the microcontroller                   |
| **Diode**                     | Helps protect the circuit from the fan's electrical effects |
| **Camera/Webcam**             | Captures images for AI detection                            |
| **Breadboard & Jumper Wires** | Circuit connections                                         |

---

# 💻 Software & Technologies

### Microcontroller

* ESP8266 NodeMCU
* Arduino IDE

### AI & Computer Vision

* Python
* OpenCV
* Google Teachable Machine
* Image classification model

### IoT

* Blynk
* Wi-Fi connectivity

### Sensors & Hardware Control

* DHT11
* I2C LCD
* LEDs
* Buzzer
* Fan control circuit

---

# ⚙️ How the System Works

The system operates through several stages.

### Step 1 — Environmental Monitoring

The DHT11 measures the surrounding temperature.

```text
DHT11
  ↓
Temperature reading
  ↓
ESP8266
```

### Step 2 — AI Detection

The camera captures the baby's condition and sends the image to the AI detection system.

```text
Camera
  ↓
Image
  ↓
AI Model
  ↓
Safe / Unsafe
```

### Step 3 — Safety Decision

The system determines the appropriate response based on the detected condition.

If the condition is safe:

```text
SAFE
 ↓
Green LED ON
Buzzer OFF
```

If the condition is unsafe:

```text
UNSAFE
 ↓
Red LED ON
Buzzer ON
Blynk notification
```

### Step 4 — Temperature Control

The temperature is continuously compared with the configured threshold.

```text
Temperature
     ↓
Above threshold?
   ↙       ↘
 YES       NO
  ↓         ↓
Fan ON    Fan OFF
```

### Step 5 — LCD Display

The current system condition and temperature are displayed on the LCD.

Example:

```text
SAFE
T: 28C FAN: OFF
```

---

# 📱 Blynk Notification System

When the system detects an unsafe condition and Wi-Fi is available, it can communicate with the Blynk platform.

The general process is:

```text
Unsafe condition
       ↓
ESP8266
       ↓
Wi-Fi
       ↓
Blynk
       ↓
📱 User notification
```

To prevent excessive notifications, the system uses notification control logic rather than repeatedly sending an alert every time the program loops.

---

# 🌐 Offline Operation

One important feature of this project is that the local monitoring system does not completely depend on the internet.

When Wi-Fi is unavailable:

* The DHT11 can continue measuring temperature.
* The LCD can continue displaying information.
* The LEDs can continue showing the safety state.
* The buzzer can continue providing local alerts.
* The fan can continue responding to temperature.

Blynk remote notifications require an internet connection.

This provides basic local protection even if the IoT connection is temporarily unavailable.

---

# 📂 Project Structure

```text
Smart-Baby-Monitor/
│
├── Arduino/
│   └── Smart_Baby_Monitor.ino
│
├── AI/
│   ├── baby_ai.py
│   └── labels.txt
│
├── Images/
│   ├── circuit.jpg
│   ├── project.jpg
│   └── system-diagram.png
│
├── Documentation/
│   └── project-report.pdf
│
└── README.md
```

The exact files may change as the project develops.

---

# 🔌 Hardware Connections

The current ESP8266 implementation uses the following main connections:

| Component   | ESP8266 Pin |
| ----------- | ----------- |
| DHT11       | D4          |
| Green LED   | D5          |
| Red LED     | D6          |
| Buzzer      | D7          |
| Fan control | D0          |
| I2C LCD     | I2C pins    |

The fan is controlled through a transistor rather than being connected directly to an ESP8266 GPIO pin.

> ⚠️ **Important:** The exact wiring should follow the project's circuit diagram and the specifications of the individual components.

---

# 🧠 AI Model

The AI component uses image classification to distinguish between predefined conditions.

The model was trained using **Google Teachable Machine** and integrated with the Python/OpenCV detection system.

The general workflow is:

```text
Collect Images
      ↓
Train AI Model
      ↓
Export Model
      ↓
Python + OpenCV
      ↓
Camera Input
      ↓
Prediction
      ↓
Safe / Unsafe
```

The AI model should only be treated as a **prototype detection system** and not as a replacement for responsible adult supervision or professional safety equipment.

---

# 🧪 Testing

The system is tested by simulating different conditions using a baby doll and controlled environmental changes.

Testing includes:

### Safety Detection

* Safe condition
* Unsafe condition
* Transition between safe and unsafe states

### Temperature

* Normal temperature
* Temperature above the configured threshold
* Fan activation
* Fan deactivation

### Alerts

* Green LED
* Red LED
* Buzzer
* Blynk notification

### Connectivity

* Wi-Fi connected
* Wi-Fi disconnected
* Local hardware operation without Wi-Fi

---

# 📊 Expected System Behaviour

| Condition                   | Green LED | Red LED | Buzzer | Fan | Blynk  |
| --------------------------- | --------- | ------- | ------ | --- | ------ |
| Safe + normal temperature   | ON        | OFF     | OFF    | OFF | Normal |
| Safe + high temperature     | ON        | OFF     | OFF    | ON  | Normal |
| Unsafe + normal temperature | OFF       | ON      | ON     | OFF | Alert  |
| Unsafe + high temperature   | OFF       | ON      | ON     | ON  | Alert  |

The exact behaviour depends on the configured thresholds and detection results.

---

# 🔐 Security

Sensitive information should **never be uploaded to a public GitHub repository**.

For example, do not publicly upload:

```cpp
#define BLYNK_AUTH_TOKEN "YOUR_REAL_TOKEN"
```

Instead, use a placeholder:

```cpp
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_TOKEN"
```

Wi-Fi passwords, API keys, authentication tokens, and other private credentials should also be kept out of public repositories.

---

# 🚀 Future Improvements

Possible future improvements include:

* 📷 Better camera integration
* 🤖 Improved AI detection accuracy
* 🌡️ Additional environmental sensors
* 💨 Air-quality monitoring
* 📱 Improved mobile dashboard
* 📊 Historical temperature and safety data
* ☁️ Cloud-based data storage
* 🔔 More advanced alert management
* 🔋 Battery-powered operation
* 🧠 More advanced AI-based safety detection
* 🏠 Integration with other smart-home devices

---

# 👥 Project Team

**Project:** Smart Baby Monitor & Smart Cot Safety System

**Developed by:**
[Add team member names here]

**School:**
[Add school name here]

**Year:**
2026

---

# 📜 Disclaimer

This project is an **educational prototype** developed to demonstrate IoT, computer vision, environmental sensing, and automated control.

It is **not a certified medical device or professional baby-safety system** and should not replace responsible adult supervision.

---

# ⭐ Project Goals

The long-term goal of this project is to demonstrate how **IoT, artificial intelligence, sensors, and automated control** can be combined to create practical technology that improves safety and provides timely information to users.

**Smart technology. Real-time monitoring. Safer environments.**
