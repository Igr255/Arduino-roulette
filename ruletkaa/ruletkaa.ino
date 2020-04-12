#include <Adafruit_NeoPixel.h > 

#define PIN 6
int buttonstate = 0;
int cervena[] = {32,19,21,25,34,27,36,30,23,5,16,1,14,9,18,7,12,3};
int cierna[] = {15,4,2,17,6,13,11,8,10,24,33,20,31,22,29,28,35,26};
int rnd;
int rnd1;
int rnd2;
int rnd3;
int value = 0;
int buzzer = 11;
int NUMPIXELS = 37;
int delayval = 20;
boolean GAME = false;



Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
void setup() {
    pinMode(buzzer, OUTPUT);
    
    pinMode(3, INPUT); 
    digitalWrite(3, HIGH);
    
    attachInterrupt(digitalPinToInterrupt(3),ISR_button,CHANGE);
    randomSeed(analogRead(0));
    strip.begin();
    Serial.begin(9600);
}   


void loop() {
  Serial.print(digitalRead(3));
  if(GAME == false)
{  
  
 
    for (int i = 1; i <= 1; i++) {
      rnd1 = random(100,150);
      rnd2 = random(0,100);
      rnd3 = random(0,150);
      Serial.print("|||");
      Serial.print(rnd1);
      Serial.print("|||");
      Serial.print(rnd2);
      Serial.print("|||");
      Serial.print(rnd3);

        for (int j = 0; j <= NUMPIXELS; j++) {            
            value = j;
            strip.setPixelColor(j, strip.Color(rnd1, rnd2, rnd3));

            if (value == 1) {
                strip.setPixelColor(j - 1, strip.Color(rnd2,rnd1, rnd3));
            }

            else if (value % 2 == 1) {
                if (value != 1)
                    strip.setPixelColor(j - 1, strip.Color(rnd3, rnd1, rnd2));
            }

           else if (value % 2 == 0) {
                strip.setPixelColor(j - 1, strip.Color(rnd3, rnd2, rnd1));            }
            
            strip.show();
            delay(100);

        }

    }

    
  
  }
  


  

   else if(GAME == true && digitalRead(3)== 1){
    rnd = random(37);
    Serial.print(rnd);      
    for (int i = 0; i < NUMPIXELS; i++) {
        strip.setPixelColor(0, strip.Color(0, 150, 0));
        if (i % 2 == 1) {
            strip.setPixelColor(i, strip.Color(150, 0, 0));
        }
        if (i % 2 == 0) {
            strip.setPixelColor(i, strip.Color(50, 50, 50));
        }
        strip.show();
    }

    for (int i = 1; i <= 5; i++) {

        for (int j = 0; j <= NUMPIXELS; j++) {
            tuk();
            value = j;
            strip.setPixelColor(j, strip.Color(150, 150, 0));

            if (value == 1) {
                strip.setPixelColor(j - 1, strip.Color(0, 150, 0));
            }

            else if (value % 2 == 1) {
                if (value != 1)
                    strip.setPixelColor(j - 1, strip.Color(50, 50, 50));
            }

           else if (value % 2 == 0) {
                strip.setPixelColor(j - 1, strip.Color(150, 0, 0));
            }

            delayval++;
            delay(delayval);
            strip.show();

        }

    }

    for (int i = 0; i <= rnd; i++) {
        tuk();
        value = i;
        strip.setPixelColor(i, strip.Color(150, 150, 0));

        if (value == 1) {
            strip.setPixelColor(i - 1, strip.Color(0, 150, 0));
        } else if (value % 2 == 1) {
            if (value != 1)
                strip.setPixelColor(i - 1, strip.Color(50, 50, 50));
        } else if (value % 2 == 0) {
            strip.setPixelColor(i - 1, strip.Color(150, 0, 0));
        }
        delayval++;
        delay(delayval);
        strip.show();

    }

    if (rnd == 0) {
        Serial.print("|||");
        Serial.print(rnd);
    }

   else if (rnd % 2 == 1 && rnd != 0) {
        cervena[(rnd - 1) / 2];
        Serial.print("|||");
        Serial.print(cervena[(rnd - 1) / 2]);
    } else if (rnd % 2 == 0 && rnd != 0) {
        cierna[(rnd - 1) / 2];
        Serial.print("|||");
        Serial.print(cierna[(rnd - 1) / 2]);
    }

    win();
    delay(1000);
    delayval = 20;
    GAME = false;
   }
   
}

void tuk() {
    digitalWrite(buzzer, HIGH);
    delay(1);
    digitalWrite(buzzer, LOW);
    delay(1);
    digitalWrite(buzzer, HIGH);
    delay(1);
    digitalWrite(buzzer, LOW);
}

void win() {
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
    delay(100);
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
    delay(100);
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
    delay(100);
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
}
void ISR_button()
{ 
  GAME = true;  
  }
