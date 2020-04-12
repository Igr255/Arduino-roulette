void setup()
{
  Serial.begin(9600);
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  pinMode(A4, INPUT_PULLUP);
  pinMode(A5, INPUT_PULLUP);
}

void loop()
{
  if (Serial.available())
  {
    char dat = Serial.read();
    while (dat == '1')
    {
      if (digitalRead(A5) == LOW)
      {
        Serial.print("hrac1001");
        break;
      }
      else if (digitalRead(A4) == LOW)
      {
        Serial.print("hrac2001");
        break;
      }
    }
    while (dat == '2')
    {
      if (digitalRead(A0) == LOW)
      {
        Serial.print('1');
        break;
      }
      else if (digitalRead(A1) == LOW)
      {
        Serial.print('2');
        break;
      }
      else if (digitalRead(A2) == LOW)
      {
        Serial.print('3');
        break;
      }
      else if (digitalRead(A3) == LOW)
      {
        Serial.print('4');
        break;
      }
    }

    while(Serial.available()) Serial.read(); // vycistenie prijimacieho buffera
    if(dat != '1' &&   dat != '2') Serial.print("ERROR"); // test chybne prijateho znaku
  }
}
