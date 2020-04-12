
#include <SPI.h>
#include <MFRC522.h>

#define SDA_PIN 10
#define RST_PIN 9
int vybraneCislo;
int reading;
int reading1; 
int P1suma;
int P2suma;
int cierna[] = { 2, 4, 6, 8, 10, 11, 13, 15, 17, 20, 22, 24, 26, 28, 29, 31, 33, 35 };
int cervena[] = { 1, 3, 5, 7, 9, 12, 14, 16, 18, 19, 21, 23, 25, 27, 30, 32, 34, 36 };

bool P1 = false;
bool P2 = false;
int bet1;
int bet2;
///
MFRC522 rfid(SDA_PIN, RST_PIN);

void setup()
{

    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);    
    pinMode(6, INPUT);        
    pinMode (7, OUTPUT); 
           
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);   

    Serial.begin(9600);
    SPI.begin();
    rfid.PCD_Init();
}

/////ROLL/////
void loop()
{
  int reading = digitalRead(7);
  int reading1 = digitalRead(6);
 
  if(reading == LOW) 
  {
    digitalWrite(5, HIGH);}

    if(reading1 == LOW) 
  {
    
    digitalWrite(5, HIGH);}

    else
    digitalWrite(5, LOW);

    
  
   
   

    if (P1 == true && P2 == true) {
   

        Serial.println("|||||||ROLUJEM|||||||");
        delay(5000);
        int rnd_cislo = random(0, 36);
        Serial.print(rnd_cislo);

        for (int i = 0; i < 18; i++) {

            if (rnd_cislo == cierna[i] && rnd_cislo != 0) {
                Serial.print("__CIERNA");
                digitalWrite(3, HIGH);
            }

            if (rnd_cislo == cervena[i] && rnd_cislo != 0) {
                Serial.print("__CERVENA");
                digitalWrite(4, HIGH);
            }
        }

        //GREEN
        if (rnd_cislo == 0) {
            Serial.print("__ZELENA");
            digitalWrite(2, HIGH);
        }

        //IDK
        if (rnd_cislo % 2 == 1) {
            Serial.println("_NEPARNA");
        }

        if (rnd_cislo % 2 == 0) {
            Serial.println("_PARNA");
        }

        delay(5000);

        digitalWrite(2, LOW);
        digitalWrite(3, LOW);
        digitalWrite(4, LOW);
        /////ROLL/////
    }

    if (!rfid.PICC_IsNewCardPresent())
        return;
	
    if (!rfid.PICC_ReadCardSerial())
        return;
	
    MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
    Serial.print("RFID tag typu: ");
    Serial.println(rfid.PICC_GetTypeName(piccType));
	
    Serial.print("Adresa RFID tagu: ");
    vypisHex(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();

    if (rfid.uid.uidByte[0] == 0x4C & rfid.uid.uidByte[1] == 0x7E & rfid.uid.uidByte[2] == 0xB6 & rfid.uid.uidByte[3] == 0xB2) {
        Serial.println("Hráč číslo 1");
        P1suma = +100;
        Serial.print("Suma na karte: ");
        Serial.print(P1suma);
        Serial.println("€");
        P1 = true;
    }
    else if (rfid.uid.uidByte[0] == 0x5C & rfid.uid.uidByte[1] == 0x11 & rfid.uid.uidByte[2] == 0x76 & rfid.uid.uidByte[3] == 0xAB) {
        Serial.println("Hráč číslo 2");
        P2suma = +100;
        Serial.print("Suma na karte: ");
        Serial.print(P2suma);
        Serial.print("€");
        P2 = true;
    }
  
    else {
        Serial.println("Choj do pice");
    }
    Serial.println();
	
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
}

void vypisHex(byte* buffer, byte bufferSize)
{
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
	
}
