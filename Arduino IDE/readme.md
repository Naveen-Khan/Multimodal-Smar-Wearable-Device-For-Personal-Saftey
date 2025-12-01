
# 🚀 Overview

**This ESP32 module is a core part of the Multimodal Smart Wearable Device for Personal Safety.
Its job is to:**
•	Read GPS module data (live coordinates)

•	Receive emergency trigger messages from Raspberry Pi

•	Send SMS alerts through GSM800L

•	Handle panic button input

•	Communicate with Raspberry Pi through TCP

•	Provide offline operation (no internet needed)

**The ESP32 processes emergency events and sends a detailed alert including:**
•	Event Type (Weapon / Voice Command / Panic Button)

•	Live GPS coordinates

•	Google Maps clickable link

•	Timestamp


# ▶️ 5. HOW TO RUN THE ESP32 CODE (Step-by-Step)
🔹 Step 1 — Install Arduino IDE Download from:
       https://www.arduino.cc/en/software
       
🔹 Step 2 — Add ESP32 Board
   Arduino IDE → File → Preferences → Additional Board URLs:

       https://espressif.github.io/arduino-esp32/package_esp32_index.json

Then:
        Tools → Board → Boards Manager → “ESP32” → Install

🔹 Step 3 — Install Required Libraries

Go to:

Sketch → Include Library → Manage Libraries

Install:

TinyGPSPlus

ArduinoJson (optional)

ESP32 core (already installed)

🔹 Step 4 — Connect ESP32

Select:

Tools → Board → ESP32 Dev Module
Tools → Port → COM
