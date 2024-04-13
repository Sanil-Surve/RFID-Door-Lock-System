// #include <SPI.h>
// #include <MFRC522.h>
// #include <Servo.h>

// #define SS_PIN 10
// #define RST_PIN 9
// #define SERVO_PIN A5

// MFRC522 rfid(SS_PIN, RST_PIN);
// Servo servo;

// byte authorizedUID[4] = {0xE0, 0xF4, 0x53, 0x13};
// int angle = 0; // the current angle of servo motor

// void setup()
// {
//     Serial.begin(9600);
//     SPI.begin();     // init SPI bus
//     rfid.PCD_Init(); // init MFRC522
//     servo.attach(SERVO_PIN);
//     servo.write(angle); // rotate servo motor to 0°

//     Serial.println("Tap RFID Tag on reader");
// }

// void loop()
// {
//     if (rfid.PICC_IsNewCardPresent())
//     { // new tag is available
//         if (rfid.PICC_ReadCardSerial())
//         { // NUID has been readed
//             MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

//             if (rfid.uid.uidByte[0] == authorizedUID[0] &&
//                 rfid.uid.uidByte[1] == authorizedUID[1] &&
//                 rfid.uid.uidByte[2] == authorizedUID[2] &&
//                 rfid.uid.uidByte[3] == authorizedUID[3])
//             {
//                 Serial.println("Authorized Tag");

//                 // change angle of servo motor
//                 if (angle == 0)
//                     angle = 90;
//                 else // if(angle == 90)
//                     angle = 0;

//                 // control servo motor according to the angle
//                 servo.write(angle);
//                 Serial.print("Rotate Servo Motor to ");
//                 Serial.print(angle);
//                 Serial.println("°");
//             }
//             else
//             {
//                 Serial.print("Unauthorized Tag with UID:");
//                 for (int i = 0; i < rfid.uid.size; i++)
//                 {
//                     Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
//                     Serial.print(rfid.uid.uidByte[i], HEX);
//                 }
//                 Serial.println();
//             }

//             rfid.PICC_HaltA();      // halt PICC
//             rfid.PCD_StopCrypto1(); // stop encryption on PCD
//         }
//     }
// }


// ================================================================

#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN    10
#define RST_PIN   9
#define SERVO_PIN A5
#define BUZZER_PIN 8 // Define the buzzer pin

MFRC522 rfid(SS_PIN, RST_PIN);
Servo servo;
int angle = 0; // the current angle of servo motor
unsigned long rotationTime = 0; // variable to store the time of rotation

void setup() {
  Serial.begin(9600);
  SPI.begin(); // init SPI bus
  rfid.PCD_Init(); // init MFRC522
  servo.attach(SERVO_PIN);
  servo.write(angle); // rotate servo motor to 0°
  pinMode(BUZZER_PIN, OUTPUT); // initialize the buzzer pin

  Serial.println("Tap RFID/NFC Tag on reader");
}

void loop() {
  if (rfid.PICC_IsNewCardPresent()) { // new tag is available
    if (rfid.PICC_ReadCardSerial()) { // NUID has been read
      if (rfid.uid.uidByte[0] == 0xE0 &&
          rfid.uid.uidByte[1] == 0xF4 &&
          rfid.uid.uidByte[2] == 0x53 &&
          rfid.uid.uidByte[3] == 0x13) {
        Serial.println("Authorized Tag");

        // change angle of servo motor
        if (angle == 0)
          angle = 180;
        else
          angle = 0;

        // control servo motor according to the angle
        servo.write(angle);
        Serial.print("Rotate Servo Motor to ");
        Serial.print(angle);
        Serial.println("°");

        // Record the time of rotation
        rotationTime = millis();

        // Trigger the buzzer when the servo motor rotates to 180 degrees
        if (angle == 180) {
          digitalWrite(BUZZER_PIN, HIGH);
          delay(500); // Buzz for 500 milliseconds
          digitalWrite(BUZZER_PIN, LOW);
        }
      } else {
        Serial.println("Unauthorized Tag");
      }

      rfid.PICC_HaltA(); // halt PICC
      rfid.PCD_StopCrypto1(); // stop encryption on PCD
    }
  }

  // Check if it's time to return the servo to its initial state
  if (angle == 180 && millis() - rotationTime >= 7000) {
    angle = 0;
    servo.write(angle);
    Serial.println("Servo returned to initial state");
  }
}
