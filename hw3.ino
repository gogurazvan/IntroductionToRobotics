#define A 0
#define B 1
#define C 2
#define D 3
#define E 4
#define F 5
#define G 6
#define DP 7
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define NEUTRAL 5
#define STAGE_1 0
#define SEG_SIZE  8
#define MIN_TRESHHOLD 200
#define MAX_TRESHHOLD 800

// declare all the segments pins
const int pinA = 4;
const int pinB = 5;
const int pinC = 6;
const int pinD = 7;
const int pinE = 8;
const int pinF = 9;
const int pinG = 10;
const int pinDP = 11;

const int pinSW = 2; // digital pin connected to switch output
const int pinX = A0; // A0 - analog pin connected to X output
const int pinY = A1; // A1 - analog pin connected to Y output

const unsigned long blinkInterval = 500;
const unsigned long debounceInterval = 200;
const unsigned long resetTime = 5000;

int xValue = 0;
int yValue = 0;

int segments[SEG_SIZE] = { 
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

byte segLightStates[SEG_SIZE] = {
  LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW
};

int moveMatrix[SEG_SIZE][4] = {  //moves are in order UP, DOWN, LEFT, RIGHT
  {A, G, F, B},   //A
  {A, G, F, B},   //B
  {G, D, E, DP},   //C
  {G, D, E, C},   //D
  {G, D, E, C},   //E
  {A, G, F, B},   //F
  {A, D, G, G},   //G
  {DP, DP, C, DP},   //DP
};

byte stage = STAGE_1;
int segIndex = DP;
byte isNeutral = true;

byte blinkState = LOW;
unsigned long lastBlink = 0;

volatile byte buttonPressed = false;
volatile byte buttonReleased = false;
volatile unsigned long lastPressDebounce = 0;
volatile unsigned long lastReleaseDebounce = 0;
volatile unsigned long pressStart = 0;

void setup() {
  for (int i = 0; i < SEG_SIZE; i++) {
    pinMode(segments[i], OUTPUT);
  }
  pinMode(pinSW, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(pinSW), buttonPress, CHANGE);

  Serial.begin(9600);
}



void loop() {
  if (buttonPressed){
    if (buttonReleased){
      stage = !stage;
      buttonPressed = false;
    }else if ((millis() - pressStart > resetTime) && stage == STAGE_1){
      buttonPressed = false;
      for (int i = 0; i < SEG_SIZE; ++i)
        segLightStates[i] = LOW;
      segIndex = DP;
    }
  }
  if (stage == STAGE_1){
    blink(blinkState, lastBlink, blinkInterval);
    segIndex = moveSegment(segIndex, isNeutral);
  }else{
    setSegment(segIndex);
  }
  lightSegments(segIndex);
}


void lightSegments(int segmentOmit){
  for (int i = 0; i < SEG_SIZE; ++i){
    if (i != segmentOmit)
      digitalWrite(segments[i], segLightStates[i]);    
  }    
}

void blink(byte &blinkState, unsigned long &lastBlink, unsigned long blinkInterval){
  if (millis() - lastBlink > blinkInterval){
    digitalWrite(segments[segIndex], blinkState);
    lastBlink = millis();
    blinkState = !blinkState;   
  }
}

int moveSegment(int currentSegment, byte &wasNeutral){
  int xVal = analogRead(pinX);
  int yVal = analogRead(pinY);
  int direction = NEUTRAL;
  if (xVal > MAX_TRESHHOLD)
    direction = LEFT;
  if (xVal < MIN_TRESHHOLD)
    direction = RIGHT;
  if (yVal > MAX_TRESHHOLD)
    direction = DOWN;
  if (yVal < MIN_TRESHHOLD)
    direction = UP;

  if (direction == NEUTRAL)
    wasNeutral = true;
  else if (wasNeutral){
    wasNeutral = false;
    return moveMatrix[currentSegment][direction]; 
  }

  return currentSegment; 
}

void setSegment(int currentSegment){
  int yVal = analogRead(pinY);
  if (yVal > MAX_TRESHHOLD)
    segLightStates[currentSegment] = LOW;
  if (yVal < MIN_TRESHHOLD)
    segLightStates[currentSegment] = HIGH;
  digitalWrite(segments[currentSegment], segLightStates[currentSegment]);
}

void buttonPress(){
  byte buttonRead = digitalRead(pinSW);
  byte buttonOn = !buttonRead;
  if (buttonOn){
    if (millis() - lastPressDebounce > debounceInterval){    
      buttonPressed = true;
      buttonReleased = false; 
      pressStart = millis();
      Serial.println("p");         
    }
    lastPressDebounce = millis();   
  }else{
    if (millis() - lastReleaseDebounce > debounceInterval){   
      buttonReleased = true;
      Serial.println("r");
    }
    lastReleaseDebounce = millis();
  }
}


