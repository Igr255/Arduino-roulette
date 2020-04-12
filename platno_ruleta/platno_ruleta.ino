// RFID čtečka - ukázka čtení adres tagů

// připojení knihoven SPI a MFRC522
#include <SPI.h>
#include <MFRC522.h>

// definování pinů pro SDA a RST
#define SDA_PIN 10
#define RST_PIN 9
int inMin = 2;
int inMax = 50;
//int jedno_cislo = 0;
//int druhe_cislo = 1;
int zvoleneCislo1 = -1;
int zvoleneCislo2 = -1;
boolean on = true;
int zmena = 0;
unsigned char buffer[64];       // buffer array for data receive over serial port
int count = 4;        

// vytvoření instance RFID čtečky z knihovny
MFRC522 rfid(SDA_PIN, RST_PIN);

void setup() {
  for (int i = inMin; i <= inMax; i++) {
    pinMode(i, INPUT);

  }


  for (int i = inMin; i <= inMax; i++) {
    digitalWrite(i, HIGH);

  }


  pinMode (53, OUTPUT);


  // komunikace přes sériovou linku rychlostí 9600 baud
  Serial.begin(9600);
  Serial1.begin(9600);
  // inicializace komunikace přes SPI
  SPI.begin();
  // inicializace komunikace s RFID čtečkou
  rfid.PCD_Init();


}

void loop() {
  
  if (Serial.available()) {
    char dat = Serial.read();
    
    while (dat == '1')
    {
      karta();
      if (rfid.uid.uidByte[0] == 0xC5 & rfid.uid.uidByte[1] == 0x00 & rfid.uid.uidByte[2] == 0xE9 & rfid.uid.uidByte[3] == 0x2B)
      {
        
        Serial.print("KRISTOF");
        clearBufferArray();
        
        return;
      }
      else if (rfid.uid.uidByte[0] == 0x42 & rfid.uid.uidByte[1] == 0x47 & rfid.uid.uidByte[2] == 0xEA & rfid.uid.uidByte[3] == 0xE2)
      {
        
        Serial.print("RUDLA");
        clearBufferArray();
        
        return;
      }
      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1(); 
    }
    
    while (dat == '2') //Chcu cislo z platna
    {
      rfid.PCD_Init();
      /*   for (int i = inMin; i <= inMax; i++) {

        if(jedno_cislo<1){
        if(digitalRead(i) == LOW){
           // digitalWrite(53, HIGH);
             Serial.print(i);
             jedno_cislo++;
             druhe_cislo=0;
             delay(5000);    // ako dlho moze byt polozeny magnet na doske
             }
            Serial.print(digitalRead(i));

        if(digitalRead(i) == HIGH){
            digitalWrite(53, LOW);
            }
         }
        if(druhe_cislo<1){
        if(digitalRead(i) == LOW){
           // digitalWrite(53, HIGH);
             Serial.print(i);
             druhe_cislo++;
             }
            Serial.print(digitalRead(i));

        if(digitalRead(i) == HIGH){
            digitalWrite(53, LOW);
            }
         }
        }*/
      for (int i = inMin; i <= inMax; i++) {


        if (digitalRead(i) == LOW && i != zvoleneCislo1 && on) {
          if (i != zvoleneCislo2) {
            //digitalWrite(53, HIGH);
            Serial.print(i);
            //Serial.println(digitalRead(i));
            zvoleneCislo1 = i;
            on = false;
            zmena ++;
          }
          continue;
          //break;
        }
        if (digitalRead(i) == LOW && i != zvoleneCislo2 && !on) {
          if (i != zvoleneCislo1 ) {
            //digitalWrite(53, HIGH);
            Serial.print(i);
            //Serial.println(digitalRead(i));
            zvoleneCislo2 = i;
            on = true;
            zmena++;
          }
          continue;
          //break;
          
        }
        
      }
      if(zmena >=2){
     dat = Serial.read();
      }
      
    }
      
  }

  // kontrola RFID tagů v okolí modulu,
  // pokud není žádný tag v okolí, volá se loop funkce od začátku,
  // celá část s RFID by tedy měla být na konci programu


  
 //while(Serial.available()) Serial.read(); 
// return;
}

void karta(){
  if ( ! rfid.PICC_IsNewCardPresent())
    return;
  // kontrola správného přečtení RFID tagu
  if ( ! rfid.PICC_ReadCardSerial())
    return;
  // výpis informace o verzi RFID tagu
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  //Serial.print("RFID tag typu: ");
  //Serial.println(rfid.PICC_GetTypeName(piccType));

  // kontrola podporovaných typů RFID tagu
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println("Tento RFID tag neni podporovany (typ MIFARE Classic).");
    return;
  }
  
}

  /* for (int i = inMin; i <= inMax; i++) {


      if(digitalRead(i) == LOW){
          digitalWrite(53, HIGH);
           Serial.println(i);
          Serial.println(digitalRead(i));        }


     if(digitalRead(i) == HIGH){
          digitalWrite(53, LOW);
          }

    }*/


  // výpis adresy RFID tagu v hexa formátu
  //  Serial.print("Adresa RFID tagu: ");
  //  vypisHex(rfid.uid.uidByte, rfid.uid.size);
  //  Serial.println();
  /*
    if(rfid.uid.uidByte[0] == 0xC5 & rfid.uid.uidByte[1] == 0x00 & rfid.uid.uidByte[2] == 0xE9 & rfid.uid.uidByte[3] == 0x2B) {
      Serial.print("k");
    }
    else if(rfid.uid.uidByte[0] == 0x42 & rfid.uid.uidByte[1] == 0x47 & rfid.uid.uidByte[2] == 0xEA & rfid.uid.uidByte[3] == 0xE2) {
      Serial.print("c");
    }
    /* ukázka přidání dalšího tagu
    else if(rfid.uid.uidByte[0] == 0x?? & rfid.uid.uidByte[1] == 0x?? & rfid.uid.uidByte[2] == 0x?? & rfid.uid.uidByte[3] == 0x??) {
      Serial.println("Detekovan novy tag ..!");
    }

    else {
      Serial.println("Detekovan neznamy RFID tag!");
    }*/
  //Serial.println();
  // ukončení komunikace a jejího zabezpečení
  
 
// podprogram pro výpis adresy RFID tagu v hexa formátu
void vypisHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

void clearBufferArray()                 // function to clear buffer array
{
    // clear all index of array with command NULL
    for (int i=0; i<count; i++)
    {
        //buffer[i]=NULL;
        rfid.uid.uidByte[i] = NULL;
    }                 
}
