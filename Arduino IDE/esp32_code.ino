

//===================================================================library==========================================================================================
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <HardwareSerial.h>
#include <TinyGPS++.h>
#include <WiFi.h>
#include "VoiceRecognitionV3_ESP32.h"
//================================================================= OLED Display Configuration==========================================================================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
//================================================================== GPS Configuration=================================================================================
HardwareSerial SerialGPS(1); // UART1 for GPS
TinyGPSPlus gps;
#define GPS_RX_PIN 25
#define GPS_TX_PIN 33
//================================================================= GSM Configuration==================================================================================
HardwareSerial SerialGSM(0); // UART2 for GSM
#define SIM800_RST_PIN 26 //5
//==================================================================== Button Configuration============================================================================
#define BUTTON_PIN 32
bool buttonPressed = false;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;
//==================================================================== Voice Recognition Configuration=================================================================
VR myVR(17, 16);   // RX=17, TX=16 for VoiceRecognitionV3 (WITH VOLTAGE DIVIDER!)
uint8_t buf[64];
// Command IDs
#define CMD_ENGLISH1 0
#define CMD_ENGLISH2 1
#define CMD_ENGLISH3 2
#define CMD_URDU1    3
#define CMD_URDU2    4
#define CMD_SINDHI1  5
#define CMD_SINDHI2  6
//==================================================================== WiFi Server==================================================================================== 
WiFiServer server(8080);
const char* PI_IP = "192.168.4.2";
const int PI_PORT = 8080;
//====================================================================  GPS Data====================================================================================== 
float latitude = 0.0;
float longitude = 0.0;
int satellites = 0;
bool gpsFixed = false;
//==================================================================== Voice Recognition Variables====================================================================
bool voiceTriggered = false;
unsigned long lastVoiceTime = 0;
const unsigned long voiceCooldown = 10000; // 10 seconds cooldown
//#######################################################################  SET UP FUNCTION ######################################################################################
void setup() {
  Serial.begin(115200);
  
  //..............................Initialize OLED................................
  Wire.begin(21, 22);          // SDA, SCL pins for ESP32
  display.begin(0x3C, true);   // I2C address 0x3C or 0x3D
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0,0);
  display.println("Starting...");
  display.display();
  delay(1000);
  
  //...........................Initialize GPS....................................
  SerialGPS.begin(9600, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
  
  //................................Initialize GSM...............................
  SerialGSM.begin(9600, SERIAL_8N1, 3, 1); // RX, TX
  pinMode(SIM800_RST_PIN, OUTPUT);
  digitalWrite(SIM800_RST_PIN, LOW);
  delay(1000);
  digitalWrite(SIM800_RST_PIN, HIGH);
  delay(2000);
  //.............................Initialize Voice Recognition....................
  myVR.begin(9600);
  
  if (myVR.clear() == 0) {
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("VR: Cleared");
    display.display();
    delay(500);
  } else {
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("VR: Not Found!");
    display.display();
    while (1);
  }
  // Load all command IDs
  uint8_t commandIDs[] = {0, 1, 2, 3, 4, 5, 6};
  if (myVR.load(commandIDs, 7) >= 0) {
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("VR: Loaded OK");
    display.println("7 Commands Ready");
    display.display();
    delay(1000);
  } else {
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("VR: Load Failed!");
    display.display();
    while (1);
  }
  //............................. Initialize Button..............................
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  //.............................Setup WiFi AP...................................
  WiFi.softAP("MUNAWAR KHAN-5G", "Mamapagal456*kk");
  server.begin();
  //.............................Setup GSM Module................................
  sendATCommand("AT");
  delay(1000);
  sendATCommand("AT+CMGF=1"); // Text mode
  
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("System Ready");
  display.print("IP: ");
  display.println(WiFi.softAPIP());
  display.println("Voice: Listening");
  display.println("GPS: Waiting...");
  display.display();
  delay(2000);
}
void loop() {
  //..........................Read GPS data.........................
  while (SerialGPS.available() > 0) {
    if (gps.encode(SerialGPS.read())) {
      if (gps.location.isValid()) {
        latitude = gps.location.lat();
        longitude = gps.location.lng();
        satellites = gps.satellites.value();
        gpsFixed = true;
      } else {
        gpsFixed = false;
      }
    }
  }
  //................Check Voice Recognition.................
  handleVoiceRecognition();
  
  //................calling update led display function.............
  updateDisplay();
  
  //...............handling panic button function...................
  handlePanicButton();
  
  //................handle pi message...............................
  handlePiMessages();
  
  delay(100);
}
//============================================================================== Voice Recognition Handler=====================================================================
void handleVoiceRecognition() {
  // Check if cooldown period has passed
  if (voiceTriggered && (millis() - lastVoiceTime > voiceCooldown)) {
    voiceTriggered = false;
  }
  
  // Listen for voice command
  int ret = myVR.recognize(buf, 50);
  if (ret > 0 && !voiceTriggered) {
    displayVoiceCommandOnOLED(buf[1]);
    
    switch (buf[1]) {
      case CMD_ENGLISH1:
        triggerEmergency("Voice: HELP");
        break;
      case CMD_ENGLISH2:
        triggerEmergency("Voice: EMERGENCY");
        break;
      case CMD_ENGLISH3:
        triggerEmergency("Voice: SAVE ME");
        break;
      case CMD_URDU1:
        triggerEmergency("Voice: BACHAO");
        break;
      case CMD_URDU2:
        triggerEmergency("Voice: BACHAO");
        break;
      case CMD_SINDHI1:
        triggerEmergency("Voice: MADDAD");
        break;
      case CMD_SINDHI2:
        triggerEmergency("Voice: MADDAD");
        break;
      default:
        break;
    }
  }
}
//============================================================================== Display Voice Command on OLED==================================================================
void displayVoiceCommandOnOLED(uint8_t commandID) {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Alert Detected by");
  
  switch (commandID) {
    case CMD_ENGLISH1:
      display.println("English cammand (help)");
      break;
    case CMD_ENGLISH2:
       display.println("English cammand (help)");
      break;
    case CMD_ENGLISH3:
       display.println("English cammand (help)");
      break;
    case CMD_URDU1:
       display.println("Urdu cammand (Bachao)");
      break;
    case CMD_URDU2:
      display.println("Urdu cammand (Bachao)");
      break;
    case CMD_SINDHI1:
      display.println("Sindhi cammand (Maddad)");
      break;
    case CMD_SINDHI2:
      display.println("Sindhi cammand (Maddad)");
      break;
    default:
      display.println("Command: Unknown");
      display.println("Language: Unknown");
      break;
  }
  
  display.println("Processing...");
  display.display();
  delay(2000); // Show command for 2 seconds
}
//============================================================================== Emergency Trigger Function=====================================================================
void triggerEmergency(String alertType) {
  if (voiceTriggered) return; // Prevent multiple triggers
  
  voiceTriggered = true;
  lastVoiceTime = millis();
  
  // Display emergency on OLED
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("VOICE ALERT!");
  display.println(alertType);
  display.println("Sending alerts...");
  display.display();
  // Send capture command to Pi
  sendToPi("capture");
  
  // Send SMS with location
  sendLocationSMS(alertType);
}
//============================================================================== Handling Panic Button=======================================================================
void handlePanicButton(){
  // Button handling with debounce
  int reading = digitalRead(BUTTON_PIN);
  if (reading == LOW && !buttonPressed && (millis() - lastDebounceTime) > debounceDelay) {
    buttonPressed = true;
    lastDebounceTime = millis();
    // Display emergency on OLED
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("ALERT: Panic Button");
    display.println("Sending alerts...");
    display.display();
    // Send capture command to Pi
    sendToPi("capture");
    
    // Send SMS
    sendLocationSMS("Panic Button Pressed");
 
  } else if (reading == HIGH && buttonPressed) {
    buttonPressed = false;
  }
}
//============================================================================  Handle Pi SMS   ===============================================================================
void handlePiMessages() {
  WiFiClient client = server.available();
  if (client) {
    String msg = client.readStringUntil('\n');
    msg.trim();
    
    if (msg == "WEAPON_DETECTED") {
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("ALERT: Robbery");
      display.println("Detected by Pi");
      display.display();
      
      // SMS send 
      sendLocationSMS("Weapon Detected by Camera");
      
      client.println("ALERT_RECEIVED");
    } else {
      client.println("OK");
    }
    client.stop();
  }
}
//==============================================================================Sending SMS To PI===============================================================================
void sendToPi(String message) {
  WiFiClient piClient;
  if (piClient.connect(PI_IP, PI_PORT)) {
    piClient.println(message);
    
    // Wait for response
    unsigned long startTime = millis();
    while (piClient.connected() && (millis() - startTime < 5000)) {
      if (piClient.available()) {
        String response = piClient.readStringUntil('\n');
        break;
      }
      delay(100);
    }
    piClient.stop();
  }
}
//============================================================================== Update Display=================================================================================
void updateDisplay() {
  display.clearDisplay();
  display.setCursor(0,0);
  
  if (gpsFixed) {
    display.println("System: ACTIVE");
    display.print("Sats: ");
    display.println(satellites);
    display.println("Voice: LISTENING");
  } else {
    display.println("System: ACTIVE");
    display.print("Sats: ");
    display.println(satellites);
    display.println("GPS: ACQUIRING...");
    display.println("Voice: READY");
  }
  
  if (voiceTriggered) {
    display.println("*** COOLDOWN ***");
  } else {
    display.println("Press BTN Emergency");
  }
  
  display.display();
}
//============================================================================== Send Location SMS==============================================================================
void sendLocationSMS(String alertType) {
  if (!gpsFixed) {
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("SMS Failed!");
    display.println("No GPS Fix");
    display.display();
    return;
  }
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Sending SMS...");
  display.display();
  String message = "🚨 EMERGENCY ALERT! 🚨\n";
  message += "Reason: " + alertType + "\n";
  message += "Current Location:\n";
  message += "Lat: " + String(latitude, 6) + "\n";
  message += "Lng: " + String(longitude, 6) + "\n";
  message += "Maps: https://maps.google.com/?q=" + String(latitude, 6) + "," + String(longitude, 6);
  
  bool smsSent = sendSMS("+923700301504", message.c_str()); // Replace with recipient number
  display.clearDisplay();
  display.setCursor(0,0);
  if(smsSent) {
    display.println("SMS Sent!");
    display.println("Alert: " + alertType);
    display.println("Status: Delivered");
  } else {
    display.println("SMS Failed!");
    display.println("Try again");
  }
  display.display();
  delay(3000);
}
//============================================================================== AT Command Function============================================================================
void sendATCommand(const char* command) {
  SerialGSM.println(command);
  delay(100);
  while(SerialGSM.available()) {
    Serial.write(SerialGSM.read());
  }
}
//============================================================================== Send SMS Function==============================================================================
bool sendSMS(const char* phoneNumber, const char* message) {
  SerialGSM.print("AT+CMGS=\"");
  SerialGSM.print(phoneNumber);
  SerialGSM.println("\"");
  delay(500);
  
  SerialGSM.println(message);
  delay(500);
  
  SerialGSM.write(26); // CTRL+Z
  
  unsigned long startTime = millis();
  while (millis() - startTime < 10000) {
    if (SerialGSM.available()) {
      String response = SerialGSM.readString();
      if (response.indexOf("OK") != -1) {
        return true;
      } else if (response.indexOf("ERROR") != -1) {
        return false;
      }
    }
  }
  return false;
}
