#define UNDOCUMENTED

//#define RAM_SIZE 1536 // Uno, mini, 328P
#define RAM_SIZE 6144 //Mega 2560
//#define RAM_SIZE 32768 //Due


#define NMI_pin 3
#define IRQ_pin 4
#define RESET_pin 5
#define INSTRUCTIONS_PER_LOOP 100

//#define USE_TIMING 1
//#define SHOW_SPEED 1

uint8_t curkey = 0;
unsigned long starttime, endtime, counttime;

extern "C" {
  uint16_t getpc();
  uint8_t getop();
  void exec6502(int32_t tickcount);
  void reset6502();
  void nmi6502();
  void irq6502();

  void serout(uint8_t val) {
    if (val == 10) {
      Serial.println();
      }
    else {
      Serial.write(val);
      }
  }


  uint8_t getkey() {
    return(curkey);
  }

  void clearkey() {
    curkey = 0;
  }

}

void setup () {
  Serial.begin (9600);
  Serial.println();

  pinMode(NMI_pin, INPUT_PULLUP);
  pinMode(IRQ_pin, INPUT_PULLUP);
  pinMode(RESET_pin, INPUT_PULLUP);
  counttime = 0;
  reset6502();
}

void loop () {
#ifdef SHOW_SPEED
  if(counttime == 0) starttime = millis();
  counttime++;
  if(counttime == 100000 ) {
    endtime = millis() - starttime;
    float ips = ((counttime * INSTRUCTIONS_PER_LOOP) / endtime ) * 1000;
    Serial.print(ips);
#ifdef USE_TIMING
    Serial.println(" cycles per second");
#else
    Serial.println(" instructions per second");
#endif
    counttime = 0;
  }
#endif

  if(!digitalRead(NMI_pin) ) {
    while(!digitalRead(NMI_pin)) { delay(10); }
    nmi6502();
  }
  if(!digitalRead(IRQ_pin)) {
    while(!digitalRead(IRQ_pin)) { delay(10); }
    irq6502();
  }


  if(!digitalRead(RESET_pin)) {
    while(!digitalRead(RESET_pin)) { delay(10); }
    reset6502();
  }

  exec6502(INSTRUCTIONS_PER_LOOP);

  if (Serial.available()) {
    curkey = Serial.read() & 0x7F;
  }
}
