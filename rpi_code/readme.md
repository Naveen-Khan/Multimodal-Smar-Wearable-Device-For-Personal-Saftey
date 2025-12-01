# 🖥️ RASPBERRY PI – Weapon Detection & Evidence Capture System
**🎯 OVERVIEW**
The Raspberry Pi serves as the primary AI processing unit in the Wearable Security System. It runs real-time weapon detection using YOLOv8, manages camera operations, coordinates with ESP32 via TCP, and handles evidence storage.

**Primary Functions:**
1. Real-time Weapon Detection – Processes video stream using YOLOv8 model
2. Evidence Capture – Saves timestamped images upon detection
3. Emergency Coordination – Communicates with ESP32 via TCP
4. System Monitoring – Manages hardware resources and logging
5. User Interface – Optional GUI/CLI for system control

**Software Requirements:**
1. Operating System: Raspberry Pi OS (64-bit) Bullseye
2. Python Version: 3.9+
3. OpenCV: 4.5+
4. PyTorch: 2.0+ (for YOLOv8)
5. Ultralytics
6. picamera2

# ⚙️ **SYSTEM SETUP**
Step 1: Flash Raspberry Pi OS
1.	Download Raspberry Pi Imager: https://www.raspberrypi.com/software/
2.	Insert microSD card (minimum 16GB)
3.	Select:<br>
  o	OS: Raspberry Pi OS (64-bit)<br>
  o	Storage: Your microSD card<br>
  o	Settings (⚙️):<br>
        *	Enable SSH<br>
        * Set username/password<br>
        * Configure WiFi (optional)<br>
        *	Set locale settings<br>
4.	Click Write and wait for completion

Step 2: Initial Configuration
🖥️ First Boot - Complete Setup
Run the following command:
```bash
sudo raspi-config
