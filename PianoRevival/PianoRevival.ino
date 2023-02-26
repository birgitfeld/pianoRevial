// Piano Revival
// -------------
// Abfrage der Matrix-verschalteten Klaviatur des PX-103

int dauer[88];

void setup() 
{
    // noch ist keine Taste gedrückt
    for(int i=0; i<22; i++) dauer[i] = 0;

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

  // Iterate over all 22 Output Pins
  int i=0;// for(int i=0; i<22; i++) // todo wieder schleife reinnehmen
  {
    delay(1000);

    // Spannung anlegen, beginnend bei PIN 22 (bis PIN 43)
    digitalWrite(i+22, HIGH);

    // Pruefe jeweils 4 Tasten mit zwei Schaltern (die an PIN 2 bis 10 hängen)
    for(int j=0; j<1;j++) // todo wieder bis 4 laufen lassen
    {
        bool s1 = (digitalRead(2*j+2) == HIGH);
        bool s2 = (digitalRead(2*j+3) == HIGH);

        // Schalter 1 nicht gedrückt
        if(!s1) 
        {
          // Wurde die Taste gerade erst losgelassen? -> Ton zuende
          if(dauer[i]==-1)
          {
            Serial.print("Note off ");
            Serial.println(i*j);
          }
          dauer[i]=0;
        }
        // Schalter 1 gedrückt aber 2 nicht
        else if(!s2)
        {
          // Dauer hochzählen (wenn nicht schon am Anschlag)
          if(dauer[i]>-1 && dauer[i]<128) dauer[i]++;
        }
        // beide Schalter gedrückt
        else
        {
          // Ton ausgeben, falls nicht sehr langsam gedrückt wurde
          if(dauer[i]>-1 && dauer[i]<128)
          {
            Serial.print("Note on ");
            Serial.print(i*j);
            Serial.print(" ");
            Serial.println(dauer[i]);
            dauer[i]=-1;
          }
        }
    }

    // Spalten wieder spannungsfrei schalten
    digitalWrite(i, LOW);

  }
}
