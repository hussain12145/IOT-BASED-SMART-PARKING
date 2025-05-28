// CREATE BE MADAN.R 18/05/2025 //

#include <SPI.h> 
#include <MFRC522.h>
#include <Servo.h>
#include <SoftwareSerial.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);

Servo entryServo, exitServo;
SoftwareSerial espSerial(A4, A5); // RX, TX for ESP8266

// IR sensor pins
const int irSlot1 = A0;
const int irSlot2 = A1;
const int irSlot3 = A2;

// Feedback components
const int greenLED = 2, redLED = A3, buzzer = A3;

// Ultrasonic sensor pins
const int entryTrig = 7, entryEcho = 8;
const int exitTrig = 4, exitEcho = 3;

 
int maxCars = 3, carCount = 0;

// List of valid UIDs
String validUIDs[] = {"DE67E056", "E38839DA"}; //REPLACE YOUR CARD UID 

// Track UIDs currently inside
String activeUIDs[3];  // maxCars = 3, so tracking 3 UIDs

// ========== FUNCTION DECLARATIONS ==========

String getUID();
bool isValidUID(String UID);
bool detectCar(int trig, int echo);
void openGate(Servo &servo);
void denyEntry();
int assignSlot();
int getAvailableSlots();
void printSlotStatus();
void sendDataToESP(String status, String UID, int available, int slot);

bool isActiveUID(String UID);
void addActiveUID(String UID);
void removeActiveUID(String UID);

// ========== SETUP ==========

void setup() {
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();
  espSerial.begin(9600);

  pinMode(greenLED, OUTPUT);  
  pinMode(redLED, OUTPUT);  
  pinMode(buzzer, OUTPUT);  
  pinMode(entryTrig, OUTPUT);  
  pinMode(entryEcho, INPUT);  
  pinMode(exitTrig, OUTPUT);  
  pinMode(exitEcho, INPUT);  

  pinMode(irSlot1, INPUT);
  pinMode(irSlot2, INPUT);
  pinMode(irSlot3, INPUT);

  entryServo.attach(5);  
  exitServo.attach(6);  
  entryServo.write(0);  
  exitServo.write(0);

  Serial.println("RFID Smart Parking System Initialized");
 
  for (int i = 0; i < 3; i++) {
    digitalWrite(redLED, HIGH);
    digitalWrite(greenLED, HIGH);
    delay(200);
    digitalWrite(redLED, LOW);
    digitalWrite(greenLED, LOW);
    delay(200);
  }

for (int pos = 0; pos <= 90; pos++) 
    {
    entryServo.write(pos);
    exitServo.write(pos);            
    delay(15);                      
  }

  for (int pos = 90; pos >= 0; pos--)
    {
    entryServo.write(pos);
    exitServo.write(pos);
    delay(15);
    }

}

// ========== MAIN LOOP ==========

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String UID = getUID();
    Serial.println("\nRFID UID Detected: " + UID);

    if (isValidUID(UID)) {  
      Serial.println("Valid UID. Checking Car Detection...");
      bool entryDetected = detectCar(entryTrig, entryEcho);  
      bool exitDetected = detectCar(exitTrig, exitEcho);  

      Serial.print("Entry Sensor Detected: "); Serial.println(entryDetected);  
      Serial.print("Exit Sensor Detected: "); Serial.println(exitDetected);  

      if (entryDetected) { // Entry  
        if (isActiveUID(UID)) {
          Serial.println("This car is already inside. Cannot enter again.");
          denyEntry();
        } else if (carCount < maxCars) {
          int assignedSlot = assignSlot();
          if (assignedSlot > 0) {    
            carCount++;  
            addActiveUID(UID);
            Serial.println("Entry Granted | Car Count: " + String(carCount)); 
            printSlotStatus();  
            Serial.print("Available Slots: ");
            Serial.println(getAvailableSlots()); 
            //openGate(entryServo);  
            sendDataToESP("ENTRY", UID, getAvailableSlots(), assignedSlot);  
            openGate(entryServo);
          } else {  
            Serial.println("Parking Full! Cannot Enter.");  
            denyEntry();  
          }  
        } else {
          Serial.println("Parking Full! Cannot Enter.");  
          denyEntry(); 
        }
      } else if (exitDetected) { // Exit  
        if (isActiveUID(UID)) {
          carCount--;  
          removeActiveUID(UID);
          Serial.println("Exit Granted | Car Count: " + String(carCount));
          printSlotStatus();  
          Serial.print("Available Slots: ");
          Serial.println(getAvailableSlots());  
          //openGate(exitServo);  
          sendDataToESP("EXIT", UID, getAvailableSlots(), 0);  
          openGate(exitServo);
        } else {
          Serial.println("This car didn't enter or already exited. Exit Denied.");
          denyEntry();
        }
      } else {  
        Serial.println("No Car Detected! Please position the vehicle properly.");
        for (int i = 0; i < 5; i++) {
        digitalWrite(greenLED, HIGH);
        delay(200);
        digitalWrite(greenLED, LOW);
        delay(200);
        }  
      }

    } else {  
      Serial.println("Unauthorized Card! Access Denied.");  
      denyEntry();  
    }

    mfrc522.PICC_HaltA();  
  }
}

// ========== FUNCTION DEFINITIONS ==========

String getUID() {
  String UID = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    UID += String(mfrc522.uid.uidByte[i], HEX);
  }
  UID.toUpperCase();
  return UID;
}

bool isValidUID(String UID) {
  for (String valid : validUIDs) {
    if (UID.equalsIgnoreCase(valid)) return true;
  }
  return false;
}

bool detectCar(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duration = pulseIn(echo, HIGH);  
  int distance = duration * 0.034 / 2;  
  return (distance > 0 && distance < 5); // within 5cm
}

void openGate(Servo &servo) {
  digitalWrite(greenLED, HIGH);
  delay(600);
  digitalWrite(greenLED, LOW);
  delay(600);
  Serial.println("GATES ARE OPEN NOW.");
  for (int pos = 0; pos <= 90; pos++) {
    servo.write(pos);
    delay(15);  
  }
  delay(5000); 
  Serial.println("GATES ARE CLOSE NOW."); 
  for (int pos = 90; pos >= 0; pos--) {
    servo.write(pos);
    delay(15);
  }
}

void denyEntry() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(redLED, HIGH);
    digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(redLED, LOW);
    digitalWrite(buzzer, LOW);
    delay(500);
  }
}


int assignSlot() {
  if (digitalRead(irSlot1) == HIGH) return 1;
  if (digitalRead(irSlot2) == HIGH) return 2;
  if (digitalRead(irSlot3) == HIGH) return 3;
  return 0;
}

int getAvailableSlots() {
  int available = 0;
  if (digitalRead(irSlot1) == HIGH) available++;
  if (digitalRead(irSlot2) == HIGH) available++;
  if (digitalRead(irSlot3) == HIGH) available++;
  return available;
}

void printSlotStatus() {
  Serial.print("Slot 1: "); Serial.println(digitalRead(irSlot1) == LOW ? "Occupied" : "Empty");
  Serial.print("Slot 2: "); Serial.println(digitalRead(irSlot2) == LOW ? "Occupied" : "Empty");
  Serial.print("Slot 3: "); Serial.println(digitalRead(irSlot3) == LOW ? "Occupied" : "Empty");
}

void sendDataToESP(String status, String UID, int available, int slot) {
  espSerial.println(status + ":" + UID + ":" + String(available) + ":" + String(slot));
}

// ========== UID TRACKING FUNCTIONS ==========

bool isActiveUID(String UID) {
  for (int i = 0; i < 3; i++) {
    if (activeUIDs[i] == UID) return true;
  }
  return false;
}

void addActiveUID(String UID) {
  for (int i = 0; i < 3; i++) {
    if (activeUIDs[i] == "") {
      activeUIDs[i] = UID;
      break;
    }
  }
}

void removeActiveUID(String UID) {
  for (int i = 0; i < 3; i++) {
    if (activeUIDs[i] == UID) {
      activeUIDs[i] = "";
      break;
    }
  }
}
