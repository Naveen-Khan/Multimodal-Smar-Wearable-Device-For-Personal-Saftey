# saftey-device
This project is a Multimodal Smart Wearable Device designed to detect emergencies and instantly send SMS alerts along with live GPS location to trusted contacts.
It integrates AI-based weapon detection, multilingual voice command recognition, GPS tracking, GSM alerts, real-time evidence collection, and ESP32–Raspberry Pi communication into one compact wearable system.

The system detects emergencies via three input channels:
1. AI-based Weapon Detection (Gun/Knife using YOLOv8 & YOLO11)  
2. Multilingual Voice Recognition (“Help”, “Bachao”, “Maddad”)  
3. Physical Panic Button Trigger


When triggered:
•	Raspberry Pi   **captures an evidence image*
•	ESP32 reads **live GPS coordinates**
•	ESP32 sends **SMS with event type + Google Maps link**
•	System returns to monitoring mode automatically

This makes it ideal for:
•	Kidnapping attempts  
•	Robbery / armed assault  
•	Violence or harassment  
•	Emergency medical conditions  
•	Women & student safety  


#🔧 Hardware Used
•	Raspberry Pi 4 model b
•	ESP32
•	OV5647 Camera
•	VR3 Voice Recognition Module
•	NEO-6M GPS
•	GSM800L
•	Buttons, Wires, Power Unit
