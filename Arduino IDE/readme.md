
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


# ⚙️ SOFTWARE SETUP
Step 1: Install Arduino IDE
Download and install from:
📥 https://www.arduino.cc/en/software

Step 2: Add ESP32 Board Support
Open Arduino IDE

Go to File → Preferences

In "Additional Board Manager URLs", add:
