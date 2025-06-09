#include <Adafruit_FONA.h>
#include "Adafruit_Fingerprint.h" //fingerprint library header file
#include<EEPROM.h> //command for storing data
#include <LiquidCrystal_I2C.h>
    LiquidCrystal_I2C lcd(0x27, 16, 2);
#include <SoftwareSerial.h>
    SoftwareSerial fingerPrint(2, 3);
    SoftwareSerial sim800l(7, 8);  // RX, TX | FINGERPRINT AND SMS
#include <Wire.h>
#include "RTClib.h" //library file for DS3231 RTC Module
    RTC_DS3231 rtc;

uint8_t id;
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fingerPrint);

#define register_back 14
#define delete_ok 15
#define forward 16
#define reverse 17
#define match 5
#define indFinger 7
#define buzzer 5

#define records 10 // 10 for 10 fingerprints

int user1,user2,user3,user4,user5,user6,user7,user8,user9,user10;
const char* userNames[] = {
    "Unknown", "Eliezer", "Warren", "Joachim", "Christian", 
    "Kem", "Eugene", "Jen", "Alex", "Christine", "Ash"
};
String timestatus;
DateTime now;


void sendSMS(String userNames, DateTime now) {
    String message = "Dear parent, " + userNames + " has timed in. Date: " + String(now.month()) + "/" + String(now.day()) + "/" + String(now.year()) + " Time: " + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
    sim800l.println("AT+CMGF=1"); 
    delay(1000);
    sim800l.println("AT+CMGS=\"+639703830550\""); // Replace with the receiver's phone number
    delay(1000);
    sim800l.println(message);
    delay(1000);
    sim800l.write(26);
    delay(3000); 
    Serial.println("SMS Sent!");
}

void setup()
{
    delay(1000);
    lcd.init();               
    lcd.backlight();
    sim800l.begin(9600);
    Serial.begin(9600);
    pinMode(register_back, INPUT_PULLUP);
    pinMode(forward, INPUT_PULLUP);
    pinMode(reverse, INPUT_PULLUP);
    pinMode(delete_ok, INPUT_PULLUP);
    pinMode(match, INPUT_PULLUP);
    pinMode(buzzer, OUTPUT);
    pinMode(indFinger, OUTPUT);
    digitalWrite(buzzer, LOW);
if(digitalRead(register_back) == 0)
{
    digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    lcd.clear();
    lcd.print("Please wait !");
    lcd.setCursor(0,1);
    Serial.println("Please wait");
    Serial.println("Downloding Data..");
    Serial.println();
    Serial.print("S.No. ");
    for(int i=0;i<records;i++)
    {
    digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    Serial.print(" User ID");
    Serial.print(i+1);
    Serial.print(" ");
}
Serial.println();
int eepIndex=0;
for(int i=0;i<30;i++)
{
if(i+1<10)
    Serial.print('0');
    Serial.print(i+1);
    Serial.print(" ");
    eepIndex=(i*7);
    download(eepIndex);
    eepIndex=(i*7)+210;
    download(eepIndex);
    eepIndex=(i*7)+420;
    download(eepIndex);
    eepIndex=(i*7)+630;
    download(eepIndex);
    eepIndex=(i*7)+840;
    download(eepIndex);
    eepIndex=(i*7)+1050;
    download(eepIndex);
    eepIndex=(i*7)+1260;
    download(eepIndex);
    eepIndex=(i*7)+1470;
    download(eepIndex);
    eepIndex=(i*7)+1680;
    download(eepIndex);
    Serial.println();
}

}
if(digitalRead(delete_ok) == 0)
{
    lcd.clear();
    lcd.print("Please Wait");
    lcd.setCursor(0,1);
    lcd.print("Reseting.....");
    for(int i=1000;i<1005;i++)
    EEPROM.write(i,0);
    for(int i=0;i<841;i++)
    EEPROM.write(i, 0xff);
    lcd.clear();
    lcd.print("System Reset");
    delay(1000);
}

    lcd.clear();
    lcd.print(" Attendance ");
    lcd.setCursor(0,1);
    lcd.print("Capstone Project");
    delay(2000);
    lcd.clear();
    digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
for(int i=1000;i<1000+records;i++)
{
if(EEPROM.read(i) == 0xff)
EEPROM.write(i,0);
}

    finger.begin(57600);
    Serial.begin(9600);
    lcd.clear();
    lcd.print("Loading.....");
    lcd.setCursor(0,1);
    delay(2000);
if (finger.verifyPassword())
{
    Serial.println("Found fingerprint sensor!");
    lcd.clear();
    lcd.print(" SYSTEM Found");
    delay(2000);
}
else
{
    Serial.println("Did not find fingerprint sensor :(");
    lcd.clear();
    lcd.print("System Not Found");
    lcd.setCursor(0,1);
    lcd.print("Check Connections");
while (1);
}

if (! rtc.begin())
    Serial.println("Couldn't find RTC");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

if (rtc.lostPower())
{
    Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(2025, 2, 1, 7, 0, 0));

}
    lcd.setCursor(0,0);
    lcd.print("  STARTING....  ");
    delay(3000);

    user1=EEPROM.read(1000);
    user2=EEPROM.read(1001);
    user3=EEPROM.read(1002);
    user4=EEPROM.read(1003);
    user5=EEPROM.read(1004);
    lcd.clear();
    digitalWrite(indFinger, HIGH);

}

void loop()
{
    now = rtc.now();
    lcd.setCursor(0,0);
    lcd.print("Time: ");
    lcd.print(now.hour(), DEC);
    lcd.print(':');
    lcd.print(now.minute(), DEC);
    lcd.print(':');
    lcd.print(now.second(), DEC);
    lcd.print(" ");
    lcd.setCursor(0,1);
    lcd.print("Date: ");
    lcd.print(now.month(), DEC);
    lcd.print('/');
    lcd.print(now.day(), DEC);
    lcd.print('/');
    lcd.print(now.year(), DEC);
    lcd.print(" ");
    delay(500);

int result=getFingerprintIDez();
if(result>0)
{
    digitalWrite(indFinger, LOW);
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
    lcd.clear();
        lcd.print(result);
        lcd.print("Name: ");
        lcd.print(userNames[result]); // Display name instead of ID
        lcd.setCursor(0,1);
        lcd.print("Time: ");
        lcd.print(now.hour(), DEC);
        lcd.print(':');
        lcd.print(now.minute(), DEC);
        lcd.print(':');
        lcd.print(now.second(), DEC);
        delay(1000);
        
attendance(result);
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("SUCCESSFUL");
    delay(2000);
    digitalWrite(indFinger, HIGH);
return;

}
checkKeys();
delay(300);
}

// ATTENDANCE SYSTEM 
void attendance(int id) {
    int user = 0, eepLoc = 0;
    if (id == 1) { eepLoc = 0; user = user1++; }
    else if (id == 2) { eepLoc = 210; user = user2++; }
    else if (id == 3) { eepLoc = 420; user = user3++; }
    else if (id == 4) { eepLoc = 630; user = user4++; }
    else if (id == 5) { eepLoc = 840; user = user5++; }
    else if (id == 6) { eepLoc = 1050; user = user6++; }
    else if (id == 7) { eepLoc = 1260; user = user7++; }
    else if (id == 8) { eepLoc = 1470; user = user8++; }
    else if (id == 9) { eepLoc = 1680; user = user9++; }
    else if (id == 10) { eepLoc = 1890; user = user10++; }
    else return;

    // Load last recorded time and status from EEPROM
    int lastHour = EEPROM.read(eepLoc + 0);
    int lastMinute = EEPROM.read(eepLoc + 1);
    int lastDay = EEPROM.read(eepLoc + 3);
    int lastMonth = EEPROM.read(eepLoc + 4);
    int lastYear = EEPROM.read(eepLoc + 5) << 8 | EEPROM.read(eepLoc + 6);
    int timedInToday = EEPROM.read(eepLoc + 7); // Flag for "Time In" status

    // TIME IN TIME OUT SYSTEM
    if (timedInToday == 1) {
        timestatus = "TIME OUT";
        lcd.clear();
        lcd.print("Stay Safe! ");
        lcd.setCursor(0, 1);
        lcd.print("Status: TIME OUT");
        delay(2000);

        EEPROM.write(eepLoc + 7, 0); // FOR RESETTING
    } else {
        timestatus = "TIME IN";
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("Status: TIME IN");
        delay(2000);

        EEPROM.write(eepLoc + 7, 1); // Mark as "TIME IN"

        sendSMS(userNames[id], now);

    }

    // Write data to EEPROM only if it has changed
    if (EEPROM.read(eepLoc + 0) != now.hour()) EEPROM.write(eepLoc + 0, now.hour());
    if (EEPROM.read(eepLoc + 1) != now.minute()) EEPROM.write(eepLoc + 1, now.minute());
    if (EEPROM.read(eepLoc + 2) != now.second()) EEPROM.write(eepLoc + 2, now.second());
    if (EEPROM.read(eepLoc + 3) != now.day()) EEPROM.write(eepLoc + 3, now.day());
    if (EEPROM.read(eepLoc + 4) != now.month()) EEPROM.write(eepLoc + 4, now.month());
    if (EEPROM.read(eepLoc + 5) != (now.year() >> 8)) EEPROM.write(eepLoc + 5, now.year() >> 8); 
    if (EEPROM.read(eepLoc + 6) != (now.year() & 0xFF)) EEPROM.write(eepLoc + 6, now.year() & 0xFF);

    // Update user counts only if they have changed
    if (EEPROM.read(1000) != user1) EEPROM.write(1000, user1);
    if (EEPROM.read(1001) != user2) EEPROM.write(1001, user2);
    if (EEPROM.read(1002) != user3) EEPROM.write(1002, user3);
    if (EEPROM.read(1003) != user4) EEPROM.write(1003, user4);
    if (EEPROM.read(1004) != user5) EEPROM.write(1004, user5);
    if (EEPROM.read(1005) != user6) EEPROM.write(1005, user6);
    if (EEPROM.read(1006) != user7) EEPROM.write(1006, user7);
    if (EEPROM.read(1007) != user8) EEPROM.write(1007, user8);
    if (EEPROM.read(1008) != user9) EEPROM.write(1008, user9);
    if (EEPROM.read(1009) != user10) EEPROM.write(1009, user10);
}

void checkKeys()
{
    if(digitalRead(register_back) == 0)
    {
    lcd.clear();
    lcd.print("Please Wait");
    delay(1000);
    while(digitalRead(register_back) == 0);
    Enroll();
    }

    else if(digitalRead(delete_ok) == 0)
    {
    lcd.clear();
    lcd.print("Please Wait");
    delay(1000);
    delet();
    }
}

void Enroll()
    {
    int count=1;
    lcd.clear();
    lcd.print("Enter Finger ID:");

    while(1)
    {
    lcd.setCursor(0,1);
    lcd.print(count);
    if(digitalRead(forward) == 0)
    {
    count++;
    if(count>records)
    count=1;
    delay(500);
}

else if(digitalRead(reverse) == 0)
{
count--;
if(count<1)
count=records;
delay(500);
}
else if(digitalRead(delete_ok) == 0)
{
id=count;
getFingerprintEnroll();
for(int i=0;i<records;i++)
{
if(EEPROM.read(i) != 0xff)
{
EEPROM.write(i, id);
break;
}
}
return;
}

else if(digitalRead(register_back) == 0)
{
return;
}
}
}

void delet()
{
int count=1;
lcd.clear();
lcd.print("Enter Finger ID");

while(1)
{
    lcd.setCursor(0,1);
    lcd.print(count);
    if(digitalRead(forward) == 0)
    {
count++;
if(count>records)
count=1;
delay(500);
}

else if(digitalRead(reverse) == 0)
{
count--;
if(count<1)
count=records;
delay(500);
}
else if(digitalRead(delete_ok) == 0)
{
id=count;
deleteFingerprint(id);
for(int i=0;i<records;i++)
{
if(EEPROM.read(i) == id)
{
EEPROM.write(i, 0xff);
break;
}
}
return;
}

else if(digitalRead(register_back) == 0)
{
return;
}
}
}

uint8_t getFingerprintEnroll()
{
int p = -1;
    lcd.clear();
    lcd.print("finger ID:");
    lcd.print(id);
    lcd.setCursor(0,1);
    lcd.print("Place Finger");
    delay(3000);
while (p != FINGERPRINT_OK)
{
p = finger.getImage();
switch (p)
{
case FINGERPRINT_OK:
    Serial.println("Image taken");
    lcd.clear();
    lcd.print("Image taken");
break;
case FINGERPRINT_NOFINGER:
    Serial.println("No Finger");
    lcd.clear();
    lcd.print("No Finger Found");
break;
case FINGERPRINT_PACKETRECIEVEERR:
    Serial.println("Communication error");
    lcd.clear();
    lcd.print("Comm Error");
break;
case FINGERPRINT_IMAGEFAIL:
    Serial.println("Imaging error");
    lcd.clear();
    lcd.print("Imaging Error");
break;
default:
    Serial.println("Unknown error");
    lcd.clear();
    lcd.print("Unknown Error");
break;
}
}

// OK success!

p = finger.image2Tz(1);
switch (p) {
case FINGERPRINT_OK:
    Serial.println("Image converted");
    lcd.clear();
    lcd.print("Image converted");
break;
case FINGERPRINT_IMAGEMESS:
    Serial.println("Image too messy");
    lcd.clear();
    lcd.print("Image too messy");
return p;
case FINGERPRINT_PACKETRECIEVEERR:
    Serial.println("Communication error");
    lcd.clear();
    lcd.print("Comm Error");
return p;
case FINGERPRINT_FEATUREFAIL:
    Serial.println("Could not find fingerprint features");
    lcd.clear();
    lcd.print("Feature Not Found");
return p;
case FINGERPRINT_INVALIDIMAGE:
    Serial.println("Could not find fingerprint features");
    lcd.clear();
    lcd.print("Feature Not Found");
return p;
default:
    Serial.println("Unknown error");
    lcd.clear();
    lcd.print("Unknown Error");
return p;
}

    Serial.println("Remove finger");
    lcd.clear();
    lcd.print("Remove Finger");
    delay(2000);
p = 0;
while (p != FINGERPRINT_NOFINGER) {
p = finger.getImage();
}
Serial.print("ID "); Serial.println(id);
p = -1;
    Serial.println("Place same finger again");
    lcd.clear();
    lcd.print("Place Finger");
    lcd.setCursor(0,1);
    lcd.print(" Again");
while (p != FINGERPRINT_OK) {
  p = finger.getImage();
switch (p) {
case FINGERPRINT_OK:
    Serial.println("Image taken");
break;
case FINGERPRINT_NOFINGER:
    Serial.print(".");
break;
case FINGERPRINT_PACKETRECIEVEERR:
    Serial.println("Communication error");
break;
case FINGERPRINT_IMAGEFAIL:
    Serial.println("Imaging error");
break;
default:
    Serial.println("Unknown error");
return;
}
}

// success!
p = finger.image2Tz(2);
switch (p) {
case FINGERPRINT_OK:
    Serial.println("Image converted");
break;
case FINGERPRINT_IMAGEMESS:
    Serial.println("Image too messy");
return p;
case FINGERPRINT_PACKETRECIEVEERR:
    Serial.println("Communication error");
return p;
case FINGERPRINT_FEATUREFAIL:
    Serial.println("Could not find fingerprint features");
return p;
case FINGERPRINT_INVALIDIMAGE:
    Serial.println("Could not find fingerprint features");
return p;
default:
    Serial.println("Unknown error");
return p;
}

// converted!
Serial.print("Creating model for #"); Serial.println(id);

p = finger.createModel();
if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
} else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
return p;
} else if (p == FINGERPRINT_ENROLLMISMATCH) {
Serial.println("Fingerprints did not match");
return p;
} else {
Serial.println("Unknown error");
return p;
}

    Serial.print("ID "); Serial.println(id);
p = finger.storeModel(id);
if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
    lcd.clear();
    lcd.print(" Finger Stored!");
    delay(2000);
} else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
return p;
} else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
return p;
} else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
return p;
}
else {
    Serial.println("Unknown error");
return p;
}
}

int getFingerprintIDez()
{
uint8_t p = finger.getImage();

if (p != FINGERPRINT_OK)
return -1;

p = finger.image2Tz();
if (p != FINGERPRINT_OK)
return -1;

p = finger.fingerFastSearch();
if (p != FINGERPRINT_OK)
{
    digitalWrite(buzzer, HIGH);
    delay(50);
    digitalWrite(buzzer, LOW);
    lcd.clear();
    lcd.print("Bio Not Found");
    lcd.setCursor(0,1);
    delay(1000);
return -1;
}
// found a match!
    Serial.print("{\"ID\": \"");
    Serial.print(finger.fingerID);
    Serial.print("\", \"Name\": \"");
    Serial.print(userNames[finger.fingerID]);
    Serial.print("\", \"Time\": \"");
    Serial.print(now.hour());
    Serial.print(":");
    Serial.print(now.minute());
    Serial.print(":");
    Serial.print(now.second());
    Serial.println("\"}");  // End JSON properly on the same line
    Serial.println("SMS Sent");
    delay(3000);
    return finger.fingerID;
}

uint8_t deleteFingerprint(uint8_t id)
{
uint8_t p = -1;
    lcd.clear();
    lcd.print("Please wait");
    p = finger.deleteModel(id);
if (p == FINGERPRINT_OK)
{
    Serial.println("Deleted!");
    lcd.clear();
    lcd.print("Finger Deleted");
    lcd.setCursor(0,1);
    lcd.print("Successfully");
    delay(1000);
}

else
{
    Serial.print("Something Wrong");
    lcd.clear();
    lcd.print("Something Wrong");
    lcd.setCursor(0,1);
    lcd.print("Try Again Later");
    delay(2000);
return p;
}
}

void download(int eepIndex)
{

    if(EEPROM.read(eepIndex) != 0xff)
{
  Serial.print("T->");
  if(EEPROM.read(eepIndex)<10)
  Serial.print('0');
  Serial.print(EEPROM.read(eepIndex++));
  Serial.print(':');
  if(EEPROM.read(eepIndex)<10)
  Serial.print('0');
  Serial.print(EEPROM.read(eepIndex++));
  Serial.print(':');
  if(EEPROM.read(eepIndex)<10)
  Serial.print('0');
  Serial.print(EEPROM.read(eepIndex++));
  Serial.print(" D->");
  if(EEPROM.read(eepIndex)<10)
  Serial.print('0');
  Serial.print(EEPROM.read(eepIndex++));
  Serial.print('/');
  if(EEPROM.read(eepIndex)<10)
  Serial.print('0');
  Serial.print(EEPROM.read(eepIndex++));
  Serial.print('/');
  Serial.print(EEPROM.read(eepIndex++)<<8 | EEPROM.read(eepIndex++));
}
else
{
Serial.print("---------------------------");
}

Serial.print(" ");
}
