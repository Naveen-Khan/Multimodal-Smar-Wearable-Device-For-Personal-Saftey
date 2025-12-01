
# 🚀 Overview
This ESP32 module serves as the central alert management system for the Multimodal Wearable Security Device. It coordinates all emergency triggers, location tracking, and communication functions, operating independently without internet connectivity.

<H3>Primary Functions:</H3>
1. **Emergency Trigger Processing** – Receives alerts from Raspberry Pi via TCP <li>
2. **GPS Data Acquisition** – Reads real-time coordinates from NEO-6M module
3. **SMS Alert Transmission** – Sends emergency messages via GSM800L
4. **Manual Input Handling** – Processes panic button and voice commands
5. **System Coordination** – Manages WiFi communication with Raspberry Pi

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
🔹 Step 6 — Upload Code

Click Upload.
