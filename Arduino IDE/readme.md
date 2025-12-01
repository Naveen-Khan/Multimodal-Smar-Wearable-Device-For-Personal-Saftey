
# 🚀 Overview
This ESP32 module serves as the central alert management system for the Multimodal Wearable Security Device. It coordinates all emergency triggers, location tracking, and communication functions, operating independently without internet connectivity.

<H3>Primary Functions:</H3><li>
 Emergency Trigger Processing – Receives alerts from Raspberry Pi via TCP <li>
 GPS Data Acquisition – Reads real-time coordinates from NEO-6M module<li>
 SMS Alert Transmission – Sends emergency messages via GSM800L<li>
 Manual Input Handling – Processes panic button and voice commands<li>
 System Coordination – Manages WiFi communication with Raspberry Pi

**The ESP32 processes emergency events and sends a detailed alert including:**<li>
	Event Type (Weapon / Voice Command / Panic Button)<li>
	Live GPS coordinates<li>
 Google Maps clickable link<li>
 Timestamp<li>


# ⚙️ Software Setup
**Step 1: Install Arduino IDE**
Download and install from:
📥 https://www.arduino.cc/en/software

**Step 2: Add ESP32 Board Support**
1. Open Arduino IDE

2. Go to File → Preferences

3. In "Additional Board Manager URLs", add:
   
      https://espressif.github.io/arduino-esp32/package_esp32_index.json
4. Click OK

**Step 3: Install ESP32 Boards**
1. Go to Tools → Board → Boards Manager
2. Search for "esp32"
3. Install "ESP32 by Espressif Systems"
4. Wait for installation to complete

**Step 4: Install Required Libraries**

Go to Sketch → Include Library → Manage Libraries
Install these libraries (search and install):

Libraries
<li>
  TinyGPSPlus<li>
  ArduinoJson	<li>
  WiFi	<li>
  HTTPClient<li>
  VoiceRecognition

**Step 5: Board Configuration**

Select Board: Tools → Board → ESP32 Dev Module

**Step 6: Upload Code**

1. Open the ESP32 sketch file (esp32_alert_system.ino)
2. Click Upload button (→ arrow icon)
3. Wait for "Done uploading" message
4. Open Serial Monitor (Tools → Serial Monitor)
5. Set baud rate to 115200
