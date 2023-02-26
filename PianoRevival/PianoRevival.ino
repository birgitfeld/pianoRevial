// Piano Revival
// -------------
// Abfrage der Matrix-verschalteten Klaviatur des PX-103

char tastenAktiv[180];    // State Vector with pressed Keys
bool keyPressed = false;  // Flag indicating whether at the current scan a keay was found pressed

void setup() 
{
    // LED als Ausgang
    pinMode(13, OUTPUT);

    // 8 Input Pins (KF0, KS0 to KF3, KS3)
    pinMode(2, INPUT);
    pinMode(3, INPUT);
    pinMode(4, INPUT); 
    pinMode(5, INPUT);
    pinMode(6, INPUT);
    pinMode(7, INPUT);
    pinMode(8, INPUT);
    pinMode(9, INPUT);
    
    // 22 Output Pins KB00 to KB21
    pinMode(22, OUTPUT);
    pinMode(23, OUTPUT);
    pinMode(24, OUTPUT);
    pinMode(25, OUTPUT);
    pinMode(26, OUTPUT);

    pinMode(27, OUTPUT);
    pinMode(28, OUTPUT);
    pinMode(29, OUTPUT);
    pinMode(30, OUTPUT);
    pinMode(31, OUTPUT);

    pinMode(32, OUTPUT);
    pinMode(33, OUTPUT);
    pinMode(34, OUTPUT);
    pinMode(35, OUTPUT);
    pinMode(36, OUTPUT);

    pinMode(37, OUTPUT);
    pinMode(38, OUTPUT);
    pinMode(39, OUTPUT);
    pinMode(40, OUTPUT);
    pinMode(41, OUTPUT);

    pinMode(42, OUTPUT);
    pinMode(43, OUTPUT);

    // Serielle Schnittstelle
    Serial.begin(115200);
    Serial.println("Start");
}

void loop() {

  keyPressed = false;
  
  // Iterate over all Output Pins
  for(int i=22; i<44; i++)
  {
    // Spannung an 8 Spalten der Matrix anlegen
    digitalWrite(i, HIGH);
//    delayMicroseconds(10); // Let Voltage settle (ToDo: Minimize!)
//    delay(100);
    
    // Pruefe die 8 Zeilen der Matrix auf Pegel
    for(int j=2; j<10; j++)
    {
      // s steht fuer Schalter und laeuft von 0 bis 175 (=2x88)
      int s = (i-22)*8+(j-2);
      

        if(digitalRead(j) == HIGH)
        {
          digitalWrite(13, HIGH);
          keyPressed = true;
          // Taste als aktiv markieren
          tastenAktiv[s]='*';
        }
        else
        {
          tastenAktiv[s]='-';
        }
    }
    // Spalten wieder spannungsfrei schalten
    digitalWrite(i, LOW);
//    delay(100);
//      delayMicroseconds(10); // Let Voltage settle (ToDo: Minimize!)
//    Serial.println(tastenAktiv);
//    Serial.write(tastenAktiv, 8);
//    Serial.println("");
  }
  if(keyPressed == false)
  {
    digitalWrite(13,LOW);
  }
  
}
