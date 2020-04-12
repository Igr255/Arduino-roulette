

#include <UTFT.h>
#include <URTouch.h>
#include <UTFT_Buttons.h>

#include <SPI.h>
#include <MFRC522.h>

#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define PIN 8
#define SDA_PIN 10
#define RST_PIN 9


int pole[3] = {0, 0, 0};
int buttonstate = 0;
int x;
int prvky_pola = 2;
int cervena[] = {32, 19, 21, 25, 34, 27, 36, 30, 23, 5, 16, 1, 14, 9, 18, 7, 12, 3};
int cierna[] = {15, 4, 2, 17, 6, 13, 11, 8, 10, 24, 33, 20, 31, 22, 29, 28, 35, 26};
int current = 0;
int player = 0;
int sirka_p = 480;
int vyska_p = 28;
int rnd_cislo;
int rnd;
//int rnd1;
//int rnd2;
//int rnd3;
int value = 0;
int buzzer = 11;
int NUMPIXELS = 37;
int delayval = 20;
int konto_hraca_1 = 200;
int konto_hraca_2 = 300;
int cislo_hraca1 = -1;
int cislo_hraca2 = -1;
int pocet_hracov = 0;
int pocet_cisel = 0;
int stav = 0;
String hrac_1 = "";
String hrac_2 = "";
String momentalny_hrac = "";
boolean GAME = false;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
UTFT    myGLCD(SSD1963_800ALT, 38, 39, 40, 41);
URTouch  myTouch( 6, 5, 4, 3, 2);
UTFT_Buttons myButtons(&myGLCD, &myTouch);


extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];
word colorlist[] = {VGA_WHITE, VGA_BLACK, VGA_RED, VGA_BLUE, VGA_GREEN, VGA_FUCHSIA, VGA_YELLOW, VGA_AQUA};







void setup() {

    pinMode(buzzer, OUTPUT);
    //  pinMode(3, INPUT);

    //  attachInterrupt(digitalPinToInterrupt(3),ISR_button,CHANGE);
    randomSeed(analogRead(0));
    strip.begin();
    Serial1.begin(9600);

    //NASTAVENIE SILY STLACENIA A FONTU
    myTouch.setPrecision(PREC_MEDIUM);
    myButtons.setTextFont(SmallFont);
    //ZAPNUTIE DISPLEJU A DOTYKU

    myTouch.InitTouch();
    myGLCD.InitLCD();

}

void loop() {

    menu();

}

void vypis() {

    switch (prvky_pola) {

    case 0:
        for (int i = 0; i < 3; i++) {
            myGLCD.print(String(pole[i]), sirka_p + x, vyska_p);
            x = x - 30;
        }
        break;

    case 1:
        myGLCD.print(String(pole[1]), sirka_p + x, vyska_p);
        x = x - 30;
        myGLCD.print(String(pole[2]), sirka_p + x, vyska_p);
        x = x - 30;
        myGLCD.print(String(pole[0]), sirka_p + x, vyska_p);
        x = x - 30;
        break;

    case 2:
        for (int i = 2; i >= 0; i--) {
            myGLCD.print(String(pole[i]), sirka_p + x, vyska_p);
            x = x - 30;
        }
        break;

    default:
        break;
    }
}
int upravCislo() {
    int pomCislo = 0;
    for (int i = 0, j = 2 - prvky_pola; i < (3 - prvky_pola), j >= 0; i++, j--) {
        pomCislo += pole[2 - i] * (int)(pow(10, j) + .5);

    }

    return pomCislo;
}

void vklad() {

    int butt1, butt2, butt3, butt4, butt5, butt6, butt7, butt8, butt9, butt0, Press, Clear, Vloz;

    myGLCD.fillScr(VGA_GREEN);
    myGLCD.setColor(VGA_BLACK);
    myGLCD.setBackColor(VGA_GREEN);
    myButtons.setButtonColors(VGA_RED, VGA_GRAY, VGA_RED, VGA_GREEN, VGA_BLACK);
    myGLCD.setFont(BigFont);
    myButtons.setTextFont(BigFont);

    butt7 = myButtons.addButton(10, 10, 100, 80, "7");
    butt8 = myButtons.addButton(140, 10, 100, 80, "8");
    butt9 = myButtons.addButton(270, 10, 100, 80, "9");
    butt4 = myButtons.addButton(10, 100, 100, 80, "4");
    butt5 = myButtons.addButton(140, 100, 100, 80, "5");
    butt6 = myButtons.addButton(270, 100, 100, 80, "6");
    butt1 = myButtons.addButton(10, 190, 100, 80, "1");
    butt2 = myButtons.addButton(140, 190, 100, 80, "2");
    butt3 = myButtons.addButton(270, 190, 100, 80, "3");
    butt0 = myButtons.addButton(10, 290, 230, 80, "0");
    Clear = myButtons.addButton(270, 290, 100, 80, "Clear");
    Vloz = myButtons.addButton(10, 390, 360, 80, "Vloz");

    myGLCD.setColor(VGA_BLACK);
    myGLCD.fillRoundRect(474, 10, 696, 96);
    myGLCD.setColor(VGA_WHITE);
    myGLCD.fillRoundRect(480, 16, 690, 90);
    myGLCD.setColor(VGA_RED);
    myGLCD.fillRoundRect(486, 22, 684, 84);
    myGLCD.setColor(VGA_WHITE);
    myGLCD.setBackColor(VGA_RED);

    myGLCD.setColor(VGA_WHITE);
    myGLCD.print(momentalny_hrac, 480, 350);

    myButtons.drawButtons();

    x = 120;
    myGLCD.setFont(SevenSegNumFont);
    int vlozena_suma = 0;
    while (1) {

        // Press = myButtons.checkButtons();

        if (myTouch.dataAvailable() == true) {
            Press = myButtons.checkButtons();
            if (Press == butt7 && current < 3) {
                pole[prvky_pola] = 7;
                vypis();
                x = 120;
                vlozena_suma = upravCislo();
                prvky_pola--;
                current++;
            }
            if (Press == butt8 && current < 3) {
                pole[prvky_pola] = 8;
                vypis();
                x = 120;
                vlozena_suma = upravCislo();
                prvky_pola--;
                //vlozena_suma = upravCislo();
                current++;
            }
            if (Press == butt9 && current < 3) {
                pole[prvky_pola] = 9;
                vypis();
                x = 120;
                vlozena_suma = upravCislo();
                prvky_pola--;
                // vlozena_suma = upravCislo();
                current++;
            }
            if (Press == butt4 && current < 3) {
                pole[prvky_pola] = 4;
                vypis();
                x = 120;
                vlozena_suma = upravCislo();
                prvky_pola--;
                //vlozena_suma = upravCislo();
                current++;
            }
            if (Press == butt5 && current < 3) {
                pole[prvky_pola] = 5;
                vypis();
                x = 120;
                vlozena_suma = upravCislo();
                prvky_pola--;
                // vlozena_suma = upravCislo();
                current++;
            }
            if (Press == butt6 && current < 3) {
                pole[prvky_pola] = 6;
                vypis();
                x = 120;
                vlozena_suma = upravCislo();
                prvky_pola--;
                // vlozena_suma = upravCislo();
                current++;
            }
            if (Press == butt3 && current < 3) {
                pole[prvky_pola] = 3;
                vypis();
                x = 120;
                vlozena_suma = upravCislo();
                prvky_pola--;
                //vlozena_suma = upravCislo();
                current++;
            }
            if (Press == butt2 && current < 3) {
                pole[prvky_pola] = 2;
                vypis();
                x = 120;
                vlozena_suma = upravCislo();
                prvky_pola--;
                //vlozena_suma = upravCislo();
                current++;
            }
            if (Press == butt1 && current < 3) {
                pole[prvky_pola] = 1;
                vypis();
                x = 120;
                vlozena_suma = upravCislo();
                prvky_pola--;
                //vlozena_suma = upravCislo();
                current++;

            }
            if (Press == butt0 && current < 3 && current != 0) {
                pole[prvky_pola] = 0;
                vypis();
                x = 120;
                vlozena_suma = upravCislo();
                prvky_pola--;
                //vlozena_suma = upravCislo();
                current++;
                // myGLCD.print(String(pole[1]), CENTER, 220);
            }
            if (Press == Clear) {

                //myGLCD.print("000", sirka_p+28, vyska_p);
                // myGLCD.print("                   ",480,220);
                for (int i = 0; i < 3; i++) {
                    pole[i] = 0;
                }
                vlozena_suma = 0;
                current = 0;
                prvky_pola = 2;
                vypis();
                x = 120;

            }
            if (Press == Vloz && player != 2 && vlozena_suma > 0) {
                myGLCD.setFont(BigFont);
                for (int i = 0; i < 3; i++) {
                    pole[i] = 0;
                }
                switch (player) {
                case 0:
                    if (konto_hraca_1 >= vlozena_suma) {
                        myGLCD.setBackColor(VGA_GREEN);
                        myGLCD.print(hrac_1, 450, 220);
                        myGLCD.print("VLOZIL:", 480, 220);
                        myGLCD.print(String(vlozena_suma), 680, 220);
                        momentalny_hrac = hrac_2;
                        player++;
                    } else {
                        myGLCD.print(hrac_1, 450, 220);
                        myGLCD.print("VLOZIL VYSOKU SUMU", 480, 220);
                        myGLCD.print("NA TVOJOM KONTE JE", 480, 240);
                        myGLCD.printNumI(konto_hraca_1, 480, 260);

                    }
                    break;
                case 1:
                    if (konto_hraca_2 >= vlozena_suma) {
                        myGLCD.setBackColor(VGA_GREEN);
                        myGLCD.print(hrac_2, 450, 220);
                        myGLCD.print("VLOZIL:", 480, 220);
                        myGLCD.print(String(vlozena_suma), 680, 220);
                        delay(1000);
                        myButtons.deleteButton(butt1);
                        myButtons.deleteButton(butt2);
                        myButtons.deleteButton(butt3);
                        myButtons.deleteButton(butt4);
                        myButtons.deleteButton(butt5);
                        myButtons.deleteButton(butt6);
                        myButtons.deleteButton(butt7);
                        myButtons.deleteButton(butt8);
                        myButtons.deleteButton(butt9);
                        myButtons.deleteButton(butt0);
                        myButtons.deleteButton(Clear);
                        myButtons.deleteButton(Vloz);
                        player = 0;
                        current = 0;
                        prvky_pola = 2;
                        roztoc_r();

                    } else {
                        myGLCD.print(hrac_2, 450, 220);
                        myGLCD.print("VLOZIL VYSOKU SUMU", 480, 220);
                        myGLCD.print("NA TVOJOM KONTE JE", 480, 240);
                        myGLCD.printNumI(konto_hraca_2, 480, 260);
                    }
                    break;
                default:
                    break;
                }

                current = 0;
                prvky_pola = 2;
                myButtons.deleteButton(butt1);
                myButtons.deleteButton(butt2);
                myButtons.deleteButton(butt3);
                myButtons.deleteButton(butt4);
                myButtons.deleteButton(butt5);
                myButtons.deleteButton(butt6);
                myButtons.deleteButton(butt7);
                myButtons.deleteButton(butt8);
                myButtons.deleteButton(butt9);
                myButtons.deleteButton(butt0);
                myButtons.deleteButton(Clear);
                myButtons.deleteButton(Vloz);

                delay(1000);
                vklad();

            }
            /*
                  if (player == 2)
                  {
                    delay(1000);
                    roztoc_r();
                  }*/

        }

    }

}

void ruleta() {

    // else if (GAME == true && digitalRead(3) == 1) {
    rnd = random(37);
    //   Serial.print(rnd);
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
            } else if (value % 2 == 1) {
                if (value != 1)
                    strip.setPixelColor(j - 1, strip.Color(50, 50, 50));
            } else if (value % 2 == 0) {
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

    /*  if (rnd == 0) {
          Serial.print("|||");
          Serial.print(rnd);
      }
    */
    if (rnd % 2 == 1 && rnd != 0) {
        cervena[(rnd - 1) / 2];
        //     Serial.print("|||");
        //     Serial.print(cervena[(rnd - 1) / 2]);
    } else if (rnd % 2 == 0 && rnd != 0) {
        cierna[(rnd - 1) / 2];
        //    Serial.print("|||");
        //    Serial.print(cierna[(rnd - 1) / 2]);
    }

    win();
    delay(1000);
    delayval = 20;
    //   GAME = false;
    // }

    koniec_r();

}

void menu() {

    int start, quit, control;
    start = myButtons.addButton(320, 220, 80, 60, "START");
    quit = myButtons.addButton(420, 220, 80, 60, "QUIT");

    myGLCD.fillScr(VGA_RED);
    myButtons.setButtonColors(VGA_RED, VGA_GRAY, VGA_RED, VGA_GREEN, VGA_BLACK);
    myButtons.drawButtons();
    myGLCD.setColor(VGA_BLACK);
    myGLCD.setBackColor(VGA_RED);
    myGLCD.setFont(BigFont);

    myGLCD.print("VITAJ V HRE E-RULETA", CENTER, 50);
    myGLCD.print("PRE ZACATIE HRY KLIKNI NA START", CENTER, 70);

    while (1) {
        if (myTouch.dataAvailable() == true) {
            control = myButtons.checkButtons();

            if (control == start) {
                myButtons.deleteButton(start);
                myButtons.deleteButton(quit);
                myGLCD.clrScr();

                uzivatelia();

            }
            if (control == quit) {
                myGLCD.lcdOff();
            }
        }
    }

}

void uzivatelia() {
    myGLCD.fillScr(VGA_RED);
    myGLCD.setColor(VGA_BLACK);
    myGLCD.setBackColor(VGA_RED);
    myGLCD.setFont(BigFont);
    myGLCD.print("PRIHLASENIE SA...", CENTER, 50);
    while (pocet_hracov < 2) {
        Serial1.print('1');
        while (!Serial1.available()); // zacyklenie, pokial nepridu data zo Serialu
        String id = Serial1.readString(); // nacitanie znakov
        switch (pocet_hracov) {
        case 0:
            hrac_1 = id;
            stav = 1;
            pocet_hracov++;
            myGLCD.print("PRIHLASIL SA HRAC 1: ", CENTER, 50);
            myGLCD.print(hrac_1, CENTER, 100);

            delay(2000);
            myGLCD.fillScr(VGA_RED);
            myGLCD.print("PRIHLASENIE SA... CAKAM DRUHEHO HRACA", CENTER, 50);
            while (Serial1.available()) Serial1.read();
            break;

        case 1:
            if (id == hrac_1) {
                while (Serial1.available()) Serial1.read();

                myGLCD.fillScr(VGA_RED);
                myGLCD.print("CHYBA, PRIHLASUJE SA ROVNAKY HRAC!", CENTER, 50);
                delay(2000);
                myGLCD.fillScr(VGA_RED);
                myGLCD.print("PRIHLASENIE SA... CAKAM DRUHEHO HRACA", CENTER, 50);
                break;
            }
            hrac_2 = id;
            stav = 1;
            pocet_hracov++;
            myGLCD.fillScr(VGA_RED);
            myGLCD.print("PRIHLASIL SA HRAC 2", CENTER, 50);
            myGLCD.print(hrac_2, CENTER, 100);

            delay(2000);
            myGLCD.fillScr(VGA_RED);
            break;

        default:
            break;
        }
    }
    myGLCD.print("TERAZ VLOZ VKLAD, PRVY VLOZI HRAC 1 ", CENTER, 50);
    myGLCD.print(hrac_1, CENTER, 200);
    momentalny_hrac = hrac_1;
    delay(3000);
    platno();

}

void platno() {
    myGLCD.fillScr(VGA_RED);
    myGLCD.setColor(VGA_BLACK);
    myGLCD.setBackColor(VGA_RED);
    myGLCD.setFont(BigFont);
    myGLCD.print("VYBRATIE FARBY,PARNEJ/NEPARNEJ ALEBO CISLA...", CENTER, 50);
    while (pocet_cisel < 2) {
        Serial1.print('2');
        while (!Serial1.available());
        int vybrane_cislo = Serial1.parseInt();
        switch (pocet_cisel) {
        case 0:
            cislo_hraca1 = vybrane_cislo;
            stav = 2;
            pocet_cisel++;
            myGLCD.print(hrac_1, 350, 50);
            myGLCD.print("VYBRAL CISLO: ", CENTER, 50);
            myGLCD.setFont(SevenSegNumFont);
            myGLCD.printNumI(cislo_hraca1, CENTER, 100);
            myGLCD.setFont(BigFont);

            delay(2000);
            myGLCD.fillScr(VGA_RED);
            myGLCD.print("VYBERANIE CISLA...", CENTER, 50);
            myGLCD.print("CAKAM KYM VYBERIE DRUHY HRAC", CENTER, 70);
            myGLCD.print("CISLO/FARBU/PARNU/NEPARNU", CENTER, 90);
            while (Serial1.available()) Serial1.read();
            break;

        case 1:
            if (vybrane_cislo == cislo_hraca1) {

                myGLCD.fillScr(VGA_RED);
                myGLCD.print("TOTO CISLO UZ BOLO VYBRANE, VYBER INE!", CENTER, 50);
                delay(2000);
                myGLCD.fillScr(VGA_RED);
                myGLCD.print("VYBERANIE CISLA...", CENTER, 50);
                myGLCD.print("CAKAM KYM VYBERIE DRUHY HRAC", CENTER, 70);
                myGLCD.print("CISLO/FARBU/PARNU/NEPARNU", CENTER, 90);
                break;
            }
            cislo_hraca2 = vybrane_cislo;
            stav = 2;
            pocet_cisel++;
            myGLCD.fillScr(VGA_RED);
            myGLCD.print(hrac_2, 250, 100);
            myGLCD.print("VYBRAL CISLO", CENTER, 50);
            myGLCD.setFont(SevenSegNumFont);
            myGLCD.printNumI(cislo_hraca2, CENTER, 100);
            myGLCD.setFont(BigFont);

            delay(2000);
            myGLCD.fillScr(VGA_RED);
            break;

        default:
            break;
        }
    }

    myGLCD.print("TERAZ VLOZ VKLAD, PRVY VLOZI HRAC 1 ", CENTER, 50);
    myGLCD.print(hrac_1, CENTER, 200);
    delay(3000);
    vklad();
}

void roztoc_r() {

    // myGLCD.fillScr(VGA_GREEN);
    myGLCD.clrScr();
    myGLCD.setColor(VGA_BLACK);
    myGLCD.setBackColor(VGA_WHITE);
    myGLCD.setFont(BigFont);
    myGLCD.print("TERAZ ROZTOC RULETU", CENTER, 200);
    delay(5000);
    myGLCD.clrScr();

    ruleta();
}

void koniec_r() {
    myGLCD.fillScr(VGA_BLUE);
    while (1) {
        // rnd_cislo = random (0, 36);

        for (int i = 0; i < 18; i++) {
            if (rnd % 2 == 0 && rnd != 0) {
                cierna[(rnd - 1) / 2];
            }
            if (rnd == cierna[i] && rnd != 0) {
                myGLCD.setColor(VGA_BLACK);
                myGLCD.setFont(BigFont);
                myGLCD.drawRoundRect(480, 150, 550, 200);
                myGLCD.fillRoundRect(480, 150, 550, 200);

                myGLCD.setBackColor(VGA_RED);
                myGLCD.setColor(VGA_WHITE);
                myGLCD.print("VYTOCENE CISLO JE:  ", 490, 120);
                myGLCD.printNumI(rnd, 490, 150);
                if (cislo_hraca1 == rnd) {
                    myGLCD.print("VYHRAL:  ", 490, 240);
                    myGLCD.print(hrac_1, 490, 260);
                }
                if (cislo_hraca2 == rnd) {
                    myGLCD.print("VYHRAL:  ", 490, 240);
                    myGLCD.print(hrac_2, 490, 260);
                }

                delay(5000);
                // Serial.println("__CIERNA");
                // digitalWrite(3, HIGH);
            }
            if (rnd % 2 == 1 && rnd != 0) {
                cervena[(rnd - 1) / 2];
            }
            if (rnd == cervena[i] && rnd != 0) {
                myGLCD.setColor(VGA_RED);
                myGLCD.drawRoundRect(480, 150, 550, 200);
                myGLCD.fillRoundRect(480, 150, 550, 200);

                myGLCD.setBackColor(VGA_RED);
                myGLCD.setColor(VGA_WHITE);
                myGLCD.print("VYTOCENE CISLO JE:  ", 470, 120);
                myGLCD.printNumI(rnd, 490, 150);
                delay(5000);

                //  Serial.println("__CERVENA");
                //  digitalWrite(4, HIGH);
            }

        }

        vklad();
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

void ISR_button() {
    GAME = true;
}
