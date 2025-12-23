# =======================================================================
# Optimized YOLOv8 Weapon Detection + ESP32 Communication
# =======================================================================

from picamera2 import Picamera2
from ultralytics import YOLO
import cv2
import socket
import threading
import time
import os
import numpy as np

# --- SETTINGS ---
MODEL_PATH = "models/best.pt"
CONF_THRESHOLD = 0.50
IOU_THRESHOLD = 0.45
COOLDOWN_SECONDS = 10

# FIXED: ESP32 connection settings
ESP32_IP = "192.168.4.1"
ESP32_PORT = 8080  # Changed from 80 to 8080

SAVE_DIR = "captures"
os.makedirs(SAVE_DIR, exist_ok=True)

capture_requested = False
last_alert_time = 0

# --- MODEL LOAD ---
print("Loading YOLOv8 model...")
model = YOLO(MODEL_PATH)
print(" Model loaded successfully!")

# --- CAMERA SETUP ---
print(" Initializing Picamera2...")
picam2 = Picamera2()
config = picam2.create_preview_configuration(main={"size": (640, 480)})
picam2.configure(config)
picam2.start()
time.sleep(1)
print("Picamera2 started.")

# --- SIMPLIFIED ESP32 COMMUNICATION ---
def send_to_esp32(message):
    """Send message to ESP32 (simple connection)"""
    try:
        print(f" Sending to ESP32: {message}")
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.settimeout(5)
        s.connect((ESP32_IP, ESP32_PORT))
        s.send(f"{message}\n".encode())
        
        # Wait for response
        try:
            response = s.recv(1024).decode().strip()
            print(f" ESP32 response: {response}")
        except socket.timeout:
            print(" No response from ESP32")
            
        s.close()
        return True
    except Exception as e:
        print(f" Failed to send to ESP32: {e}")
        return False
        
        
# --- TCP SERVER (for ESP32 to Pi messages) ---
def tcp_server_for_esp32():
    global capture_requested
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server_socket.bind(("0.0.0.0", 8080))
    server_socket.listen(1)
    print("Pi TCP server running on port 8080")

    while True:
        try:
            client_socket, addr = server_socket.accept()
            print(f" Connection from ESP32: {addr}")
            data = client_socket.recv(1024).decode().strip()
            print(f" Message from ESP32: {data}")

            if data.lower() == "capture":
                capture_requested = True
                client_socket.send("CAPTURE_ACK\n".encode())
                print(" Capture request acknowledged")
            else:
                client_socket.send("UNKNOWN_CMD\n".encode())

            client_socket.close()
        except Exception as e:
            print(f" Server error: {e}")

# --- START SERVER THREAD ---
print("Starting TCP server thread...")
threading.Thread(target=tcp_server_for_esp32, daemon=True).start()
time.sleep(2)

# --- HELPER: Save Frame ---
def capture_and_save(frame, prefix="weapon"):
    timestamp = time.strftime("%Y%m%d_%H%M%S")
    path = os.path.join(SAVE_DIR, f"{prefix}_{timestamp}.jpg")
    cv2.imwrite(path, frame)
    print(f" Image saved: {path}")
    return path

# --- MAIN LOOP ---
print("Starting YOLOv8 weapon detection... Press 'q' to quit.")
print("Waiting for weapon detection...")
prev_time = time.time()

try:
    while True:
        frame = picam2.capture_array()

        # Convert to BGR for OpenCV
        if frame.shape[2] == 4:
            frame = cv2.cvtColor(frame, cv2.COLOR_BGRA2BGR)
        else:
            frame = cv2.cvtColor(frame, cv2.COLOR_RGB2BGR)

        # Resize for faster inference
        small_frame = cv2.resize(frame, (416, 416))

        # Run YOLO inference
        results = model.predict(source=small_frame, conf=CONF_THRESHOLD, iou=IOU_THRESHOLD, verbose=False, device="cpu")

        # Draw detection boxes
        annotated = results[0].plot()
        detected = len(results[0].boxes) > 0

        # --- WEAPON DETECTION ALERT ---
        now = time.time()
        if detected and (now - last_alert_time) > COOLDOWN_SECONDS:
            last_alert_time = now
            print("?? WEAPON DETECTED! Sending alert to ESP32...")
            save_path = capture_and_save(frame, "weapon_detected")
            send_to_esp32("WEAPON_DETECTED")

        # --- MANUAL CAPTURE (ESP32 Trigger) ---
        if capture_requested:
            print("?? Manual capture triggered by ESP32")
            capture_and_save(frame, "manual_capture")
            capture_requested = False

        # --- FPS Display ---
        new_time = time.time()
        fps = 1 / (new_time - prev_time)
        prev_time = new_time

        color = (0, 0, 255) if detected else (0, 255, 0)
        cv2.putText(annotated, f"FPS: {fps:.2f}", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.6, color, 2)
        cv2.putText(annotated, f"Weapons: {len(results[0].boxes)}", (10, 60), cv2.FONT_HERSHEY_SIMPLEX, 0.6, color, 2)

        cv2.imshow("YOLOv8 Weapon Detection", annotated)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

except KeyboardInterrupt:
    print("\nStopped by user.")

finally:
    picam2.stop()
    cv2.destroyAllWindows()
    print("Program finished.")

