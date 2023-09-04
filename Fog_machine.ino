#define SENSOR 14
#define FOG    11

#define RED 8
#define GREEN 9
#define BLUE 10

#define HOLDOFF_LED RED // RED
#define FIRING_LED 17 // ORANGE
#define IDLE_LED GREEN // GREEN

#define MOTION_DETECTED_LED BLUE

#define SPRAY_DURATION_MS 2500
#define LOCKOUT_DURATION_MS 30000

enum STATE {
  IDLE,
  FIRING,
  HOLDOFF,
};

enum STATE myState = IDLE;
int pirState = LOW;
int val = 0;
unsigned long sprayTime = 0;
unsigned long lockoutTime = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(SENSOR, INPUT);
  pinMode(FOG, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(HOLDOFF_LED, OUTPUT);
  pinMode(MOTION_DETECTED_LED, OUTPUT);
  pinMode(IDLE_LED, OUTPUT);
}

void loop() {
    switch (myState){
      case IDLE:
        clear_state_led();
        turn_on_led(IDLE_LED);
        if (digitalRead(SENSOR) == HIGH) {
          myState = FIRING;
          digitalWrite(FOG, HIGH);
          sprayTime = millis();
        } 
        break;
      case FIRING:
        clear_state_led();
        turn_on_led(FIRING_LED);
        if (millis() - sprayTime > SPRAY_DURATION_MS) {
          myState = HOLDOFF;
          digitalWrite(FOG, LOW);
          lockoutTime = millis();
        }
        break;
      case HOLDOFF:
        clear_state_led();
        turn_on_led(HOLDOFF_LED);

        if (millis() - lockoutTime > LOCKOUT_DURATION_MS){
          myState = IDLE;
        }
    }
    digitalWrite(MOTION_DETECTED_LED, digitalRead(SENSOR));
}

void clear_state_led(){
  // FIRING LED doesn't need to be cleared explicitly because its both red and green turned on
  digitalWrite(HOLDOFF_LED, LOW);
  digitalWrite(IDLE_LED, LOW);
}

void turn_on_led(int led){
  if(led == FIRING_LED) {
    digitalWrite(IDLE_LED, HIGH);
    digitalWrite(HOLDOFF_LED, HIGH);
  }
  else{
    digitalWrite(led, HIGH);
  }
}
