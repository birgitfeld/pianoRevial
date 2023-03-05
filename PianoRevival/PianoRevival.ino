// Piano Revival
// -------------
// Abfrage der Matrix-verschalteten Klaviatur des PX-103 von Technics.
// Jede Klaviertaste hat zwei Schalter (s1 und s2), die beim Tastendruck zeitlich kurz nacheinander geschlossen werden.
// Aus dieser Dauer wird die Anschlagstärke (in 128 Stufen) ermittelt.
// Im Zustand TONE_ON wird das midi Kommando zum Starten des Tons übermittelt. Bei TONE_OFF entsprechend beendet.
//
//                 O
//                 │
//                 │
//          ┌──────▼──────────────┐
// ┌────────►   NOT_PRESSED       ◄──────────────┐
// │        └──────┬──────────────┘              │
// │               │                             │!s1
// │               │ s1                          │
// │               │                             │
// │        ┌──────▼──────────────┐        ┌─────┴────────┐
// │    ┌───►   COUNTING          ├────────►   TOO_SLOW   │
// │    │   └─┬────┬──────────────┘ >max   └──────────────┘
// │    │     │    │
// │    └─────┘    │ s2
// │               │
// │        ┌──────▼──────────────┐
// │        │   TONE_ON           │
// │        └──────┬──────────────┘
// │               │
// │               │ tick
// │               │
// │        ┌──────▼──────────────┐
// │        │  HOLDING            │
// │        └──────┬──────────────┘
// │               │
// │tick           │ !s1
// │               │
// │       ┌───────▼─────────────┐
// └───────┤    TONE_OFF         │
//         └─────────────────────┘

#include <digitalWriteFast.h>

int dauer[88];
const int maxDur = 128;

enum State_t
{
  NOT_PRESSED,
  COUNTING,
  TOO_SLOW,
  TONE_ON,
  HOLDING,
  TONE_OFF 
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

  // Iterate over all 22 Output Pins, beginnend bei PIN 22 (bis PIN 43)
  for(int s=22; s<44; s++)
  {
    // Spannung anlegen
    digitalWrite(s, HIGH);
    delayMicroseconds(8);

    // Pruefe jeweils 4 Tasten mit zwei Schaltern (die an PIN 2 bis 9 hängen)
    for(int j=2,k=3; j<9;j+=2,k+=2)
    {
      bool s1= (digitalReadFast(j) == HIGH);
      bool s2= (digitalReadFast(k) == HIGH);

      // Ascii-Diagramm zur Statemachine: siehe oben
      switch(state[i])
      {
        case NOT_PRESSED:
          if(s1) state[i]=COUNTING;
          break;
        
        case COUNTING:
          if(dauer[i]>maxDur) state[i]=TOO_SLOW;
          else if(!s1)        state[i]=NOT_PRESSED;
          else if( s2)        state[i]=TONE_ON;
          else                dauer[i]++;
          break;
        
        case TOO_SLOW:
          if(!s1) state[i]=NOT_PRESSED;
          break;
        
        case TONE_ON:
          // todo issue tone - parameter sind Tastennummer und Anschlagstärke
          Serial.println("on");
          dauer[i]=0;
          state[i]=HOLDING;
          break;
        
        case HOLDING:
          if(!s1) state[i]=TONE_OFF;
          break;
        
        case TONE_OFF:
          // todo issue tone off
          Serial.println("off");
          state[i]=NOT_PRESSED;
          break;
      }
      i++;
    }
    // Spannung an diesem PIN wieder wegnehmen
    digitalWrite(s, LOW);
  }
}
