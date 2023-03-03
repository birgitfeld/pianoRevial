// Piano Revival
// -------------
// Abfrage der Matrix-verschalteten Klaviatur des PX-103

int dauer[88];

enum State_t
{
  NOT_PRESSED,  // 0 Taste nicht gedrückt
  WAIT,         // 1 Warten auf Schalter 2
  DETECTED,     // 2 Schalte 2 wurde soeben detektiert
  PRESSED,      // 3 Taste gedrückt
  RELEASED      // 4 Taste losgelassen 
};

State_t state[88];


void setup() 
{
    // noch ist keine Taste gedrückt
    for(int i=0; i<88; i++) dauer[i] = 0;
    for(int i=0; i<88; i++) state[i] = NOT_PRESSED;

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

void loop() 
{
  int i=0;

  // Iterate over all 22 Output Pins
  for(int s=22; s<44; s++)
  {
    delayMicroseconds(4);

    // Spannung anlegen, beginnend bei PIN 22 (bis PIN 43)
    digitalWrite(s, HIGH);

    // Pruefe jeweils 4 Tasten mit zwei Schaltern (die an PIN 2 bis 9 hängen)
    for(int j=2,k=3; j<9;j+=2,k+=2)
    {
        // Schalter 1 nicht gedrückt
        if(!(digitalRead(j) == HIGH)) 
        {
          // Taste gedrückt -> gerade losgelassen
          if(state[i]==PRESSED) state[i]=RELEASED;
        }
        // Schalter 1 gedrückt aber 2 nicht
        else if(!(digitalRead(k) == HIGH))
        {
          if(state[i]==NOT_PRESSED) state[i]=WAIT;

          // Warten auf Schalter 2, wenn wir auf dem Weg nach unten sind
          else if(state[i]==WAIT)
          {
            // Dauer hochzählen (wenn nicht schon am Maximum)
            if(dauer[i]<128) dauer[i]++;
          }
        }
        // beide Schalter gedrückt
        else
        {
          // Ton ausgeben
          if(state[i]==WAIT) state[i]=DETECTED;
        }
        i++;
    }
    // Spalten wieder spannungsfrei schalten
    digitalWrite(s, LOW);
  }

  for(i=0; i<88; i++)
  {
    if(state[i]==DETECTED)
    {
      if(dauer[i]<128) { Serial.print(i); Serial.print(","); Serial.println(dauer[i]); }
      state[i]=PRESSED;
    }
    else if(state[i]==RELEASED)
    {
      Serial.print(i); Serial.println(",o");
      state[i]=NOT_PRESSED;
    }
  }
}
