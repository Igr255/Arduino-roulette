char mystr = '1';
int button = 3;


int buttonstate = 1;
long cas = 0;

volatile int buttonclick;
int tuk;

void setup() {
  pinMode(button, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(button),ISR_button, LOW);
  
  Serial.begin(9600);
}

void loop() { } 
 
void ISR_button()
{
  Serial.write('3');
  }
