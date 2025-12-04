# 🖥️ RASPBERRY PI – Weapon Detection & Evidence Capture System
# 🎯 OVERVIEW

The Raspberry Pi serves as the primary AI processing unit in the Wearable Security System. It runs real-time weapon detection using YOLOv8, manages camera operations, coordinates with ESP32 via TCP, and handles evidence storage.

**Primary Functions:**
1. Real-time Weapon Detection – Processes video stream using YOLOv8 model
2. Evidence Capture – Saves timestamped images upon detection
3. Emergency Coordination – Communicates with ESP32 via TCP
4. System Monitoring – Manages hardware resources and logging
5. User Interface – Optional GUI/CLI for system control

**Software Requirements:**
1. Operating System: Raspberry Pi OS (64-bit) Bullseye
2. Python Version: 3.11+
3. OpenCV: 
4. PyTorch:  (for YOLOv8)
5. Ultralytics
6. picamera2

# ⚙️ **SYSTEM SETUP**
**Step 1: Flash Raspberry Pi OS**
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

**Step 2: Initial Configuration**

🖥️ First Boot - Complete Setup

Run the following command:
```bash
sudo raspi-config
```
⚙️ **Recommended Settings**

1. Interface Options → Enable Camera

2. Interface Options → Enable SSH

**Step 3: Update System**
 Update package lists
```bash
sudo apt update
sudo apt upgrade -y
```
 Install essential tools
```bash
sudo apt install -y git curl wget vim tmux htop
```

# 📦 SOFTWARE INSTALLATION
**Step 4: Install Python Dependencies**


 1. Install Python and pip
```bash
sudo apt install -y python3 python3-pip python3-venv
```
 2. Create virtual environment
```bash
python3 -m venv ~/myvenv
source ~/myvenv/bin/activate
```
 3. Upgrade pip
```bash
pip install --upgrade pip
```
4. install dependancy
   Install OpenCV and camera libraries
```bash
sudo apt install python3-opencv python3-picamera2 -y
```
 Install Python packages
```bash
pip3 install ultralytics torch torchvision numpy
```

# Directory Structure
```bash
weapon-detection-rpi/
├── weapon_detection.py          # Main detection script
├── models/
│   └── best.pt                 # Trained YOLOv8 model
├── outputs/                    # Detection outputs
├── samples/                    # Test images/videos
└── README.md                   # This file
```
 **Run Detection Script**
 ```bash
cd weapon-detection-rpi
python3 weapon_detection.py
```
