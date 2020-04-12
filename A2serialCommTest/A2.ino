char mystr; //Initialized variable to store recieved data

void setup() {
 pinMode(13, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if(Serial.available())
  {
    mystr = Serial.read();
    
    if(mystr == '1')
    {
      digitalWrite(13, HIGH);
      
      }
      
      else{
        digitalWrite(13, LOW);
      
        }
     
  }
  

  }
