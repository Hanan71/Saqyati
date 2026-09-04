# 🌱 Saqyati | Smart Plant Care IoT

<p align="center">
  <b>From Automatic Watering to Remotly Autonomous Indoor Plant Care 🌿🤖</b><br>
  <i>An upcycled, AI-ready IoT misting ecosystem for sensitive indoor plants</i>
</p>

---

## 📌 Project Overview
**Saqyati** is an intelligent IoT plant-care device designed for indoor greenery in apartments, offices, and modern homes. It addresses the common struggle of erratic watering schedules, under-watering during travel, and over-watering that leads to root rot, offering an automated, data-driven alternative to manual watering.

---

## 💡 Why Ultrasonic Evaporative Misting?
Rather than using conventional heavy drip irrigation or submersible water pumps, **Saqyati** utilizes **ultrasonic evaporative misting**:
* **Gentle on Delicate Plants:** Many popular indoor species (such as tropical ferns, orchids, calatheas, and begonias) suffer under conventional soil soaking, which frequently causes waterlogging and fungal root decay. Misting provides micro-droplets directly to the aerial microclimate and topsoil.
* **Biomimicking Natural Microclimates:** Generates fine cool vapor that mimics morning dew and natural humidity, letting plants absorb water steadily through leaves and pores without hydrostatic stress.
* **Precise & Efficient Water Consumption:** Minimizes runoff and water wastage while ensuring uniform surrounding humidity.

---

## 🛠️ Hardware Innovation & Upcycling
The MVP of **Saqyati** embodies circular electronics and **hardware upcycling**:
* **Repurposed Humidifier Driver Board:** 
  * Extracted an operational piezoelectric ultrasonic driver circuit from a broken, discarded household diffuser/humidifier.
* **Bridging to IoT via Hardware Hacking:**
  * Reverse-engineered the push-button trigger and driver lines on the salvaged circuit board.
  * Soldered precision jumper connections between the board's control pins and a microcontroller (**ESP8266 / ESP32**).
  * The ESP microcontroller sends digital high/low logic pulses to switch the misting transducer on and off on command, converting an analog appliance into a smart, connected cloud IoT actuator.

---

## 🧪 Nutrient-Enriched Hydration
* **Current Prototype (MVP):** 
  * The water reservoir accommodates liquid hydroponic fertilizers and bio-nutrients manually dosed by the owner. The ultrasonic disk diffuses the nutrient-rich solution directly into the air and plant surroundings.
* **Future Autonomous Roadmap:**
  * Implementing automated peristaltic micro-dosing cartridges to dynamically blend and inject liquid nutrients into the reservoir based on plant growth stages.

---

## ⚙️ System Architecture

```text
  [ User Interface (Web / Mobile App) ]
                   │
                   ▼
  [ Plant Care Engine (AI/ML Recommendations) ]
                   │
                   ▼
       [ Customized Schedule Engine ]
                   │ (Wi-Fi / HTTP / MQTT)
                   ▼
           [ ESP Microcontroller ]
                   │ (Direct Soldered GPIO Trigger)
                   ▼
      [ Upcycled Ultrasonic Mist Board ]
                   │
                   ▼
  🌱 Ultrasonic Mist & Nutrient Delivery to Plant
```


##  🚀 Roadmap & Future Enhancements
[ ] Soil Moisture Sensing: Integrate capacitive soil sensors to avoid dry spells and prevent over-saturation.

[ ] Water Level Monitoring: Ultrasound/float sensor alerts when reservoir replenishment is needed.

[ ] Automated Fertilizer Dosing: Cartridge-based mechanical micro-pumping for autonomous nutrient feeding.

[ ] Data-Driven ML Models: Tailor misting duration based on ambient room temperature, humidity, and plant variety.

[ ] Multi-Plant Clustering: Support multiple mist heads controlled from a single centralized hub.


## 💻 Getting Started
Hardware Prerequisites
ESP microcontroller (ESP32 or ESP8266)

Salvaged ultrasonic mist driver board + piezoelectric atomization disk

Soldering iron, rosin core solder, and 28–30 AWG hookup wire

Switching transistor / Optocoupler (depending on trigger circuit topology)

5V USB power supply & water reservoir container

## Firmware Setup
1- Clone the repository:

```
git clone [https://github.com/your-username/saqyati.git](https://github.com/your-username/saqyati.git)
cd saqyati
```
2- Open the project in Arduino IDE or PlatformIO.

3- Configure your network credentials in config.h:
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASS = "YOUR_WIFI_PASSWORD";

Flash the code to your ESP board and verify pin triggers to the mist driver board.

[![View Pitch Deck on Canva](https://img.shields.io/badge/Canva-View%20Pitch%20Deck-4CAF50?style=for-the-badge&logo=canva&logoColor=white)](https://canva.link/d7cszetln016mrj) 👈 click here to view

📄 All rights reserved © 2026e.
