
# 🚀 Overview
**This ESP32 module is a core part of the Multimodal Smart Wearable Device for Personal Safety.**
Its job is to:
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
