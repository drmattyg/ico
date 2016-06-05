#define OUTPUT_PIN 4 //11
#define LIMIT_LEFT 5 // 10
#define LIMIT_RIGHT 6 //9
#define LED_PIN 13
#define DIRECTION_PIN 3 // 12
#define IGNITOR_PIN 7

#define VALVE_PIN 2 // 13
#define DIR_LEFT 1
#define DIR_RIGHT 2
#define DIR_UNKNOWN 0

#define START 0
#define STOP 1
#define OPEN LOW
#define CLOSED HIGH

int speed = 255;
boolean debounceLeft = false;
boolean debounceRight = false;
int direction = DIR_UNKNOWN; 

void blink(int times) {
  for(int i = 0; i < times; i++) {
      digitalWrite(LED_PIN, HIGH);
      delay(250);             
      digitalWrite(LED_PIN, LOW);
      delay(250);   
  }
}

void valve(int state) {
  digitalWrite(VALVE_PIN, state);
}

void ignitor() {
    delay(1000);
    digitalWrite(IGNITOR_PIN, LOW);
    delay(500);
    digitalWrite(IGNITOR_PIN, HIGH);
}

void setup() {
  //Serial.begin(9600);Serial.println("Starting");
  pinMode(OUTPUT_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(LIMIT_LEFT, INPUT_PULLUP);
  pinMode(LIMIT_RIGHT, INPUT_PULLUP);
  pinMode(DIRECTION_PIN, OUTPUT);
  digitalWrite(VALVE_PIN, HIGH);
  pinMode(VALVE_PIN, OUTPUT);
  digitalWrite(IGNITOR_PIN, HIGH);
  pinMode(IGNITOR_PIN, OUTPUT);
  digitalWrite(DIRECTION_PIN, HIGH);
  blink(2);
  digitalWrite(OUTPUT_PIN, HIGH);
}

void switchDirection() {
  digitalWrite(DIRECTION_PIN, !digitalRead(DIRECTION_PIN));

}

void drive(int a) {
  if(a == START) {
    digitalWrite(OUTPUT_PIN, HIGH);
  } else {
    digitalWrite(OUTPUT_PIN, LOW);
  }
}

void loop() {
    int left = digitalRead(LIMIT_LEFT);
    int right = digitalRead(LIMIT_RIGHT);
    if(!debounceLeft && left == LOW) { // && (direction == DIR_LEFT || direction == DIR_UNKNOWN)) {
      // Serial.println("RIGHT");
      // blink(1);
      drive(STOP); 
      valve(CLOSED);
      delay(100);
      debounceLeft = true;
      switchDirection();
      drive(START);
      direction = DIR_RIGHT;
 

    }
    if(!debounceRight && right == LOW) {// && (direction == DIR_RIGHT || direction == DIR_UNKNOWN)) {
      // Serial.println("LEFT");
      // blink(2);
      drive(STOP);
      valve(OPEN);
      ignitor();
      debounceRight = true;
      switchDirection();
      drive(START);
      direction = DIR_LEFT;

    }
    if(debounceLeft && left == HIGH) {
      delay(100);
      debounceLeft = false;
    }
    if(debounceRight && right == HIGH) {
      delay(100);
      debounceRight = false;
    }
  
}
