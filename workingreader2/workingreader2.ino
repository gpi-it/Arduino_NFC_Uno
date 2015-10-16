/**************************************************************************/
/*! 
 *  For Arduino Uno - !!!Won't work with yun!!!
 *  Edited from
    @file     readMifareClassic.pde
    @author   Adafruit Industries
	  @license  BSD (see license.txt)
*/
/**************************************************************************/
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>

// If using the breakout or shield with I2C, define just the pins connected
// to the IRQ and reset lines.  Use the values below (2, 3) for the shield!
#define PN532_IRQ   (2) //cut the trace between IRQ and pin2 and wire it/set it to 6 for arduino yun
#define PN532_RESET (3)  // Not connected by default on the NFC Shield

// Or use this line for a breakout or shield with an I2C connection:
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

void setup(void) {
  Serial.begin(115200); //set Baudrate to 115200
  Serial.println("Initialized"); //we know the board and I2C connection so this simply confirmed loaded code
  pinMode(10,OUTPUT); //set Pin10 for output
  nfc.begin();

  // configure board to read RFID tags
  nfc.SAMConfig();
  Serial.println("");
  Serial.println("Waiting for card..");Serial.println("");
}


void loop(void) {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength); //success if the card is mifare iso14443a
  int inByte = 0;

  while(!Serial); //test code for arduino yun
  
  if (success) {
      uint32_t cardid = uid[0];
      cardid <<= 8;
      cardid |= uid[1];
      cardid <<= 8;
      cardid |= uid[2];  
      cardid <<= 8;
      cardid |= uid[3]; 
      Serial.print("DeviceID #");
      Serial.print(cardid);
      Serial.print(" in range");Serial.println("");
        if (cardid == 16909060) { //if card matchs unique number
          Serial.print("Gabes Phone");Serial.println("");Serial.println("");
          inByte = Serial.read(); //this has to be here and I'm not sure why
          digitalWrite(10,HIGH); //signal on pin10
          delay(1000); //delay for pin on
          digitalWrite(10,LOW); //signal off pin10
          delay(2000); //to stop console spam when a card is near
        }
        else {
          Serial.print("Not recognised device");Serial.println("");Serial.println("");
          delay(2000);
        }
  }
  else {
    Serial.print("Read Failed");Serial.println("");
  }
}

