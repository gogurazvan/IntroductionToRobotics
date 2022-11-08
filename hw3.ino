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
const unsigned long debounceInterval = 50;
const unsigned long resetTime = 2000; //time the button stays pressed for reset

int xValue = 0;
int yValue = 0;

int segments[SEG_SIZE] = {            //segment pins array
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

byte segLightStates[SEG_SIZE] = {   //array that save the on/off state for each LED
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

byte buttonPressed = false;
unsigned long lastPressDebounce = 0;
unsigned long lastReleaseDebounce = 0;
unsigned long pressStart = 0;
byte buttonRead = false;
byte lastButtonRead = false;
byte pressed = false;
byte released = true;

void setup() {
  for (int i = 0; i < SEG_SIZE; i++) {
    pinMode(segments[i], OUTPUT);
  }
  pinMode(pinSW, INPUT_PULLUP);

//  attachInterrupt(digitalPinToInterrupt(pinSW), buttonPress, CHANGE);
}

void loop() {
  buttonRead = digitalRead(pinSW);
  if (buttonRead != lastButtonRead)
    lastPressDebounce = millis();
  if (millis() - lastPressDebounce > debounceInterval){  //classic debounce
    if (buttonPressed == buttonRead)
      buttonPressed = !buttonRead;
      
    if (buttonPressed){
      if (released){    //the button should not be pressed if it was not released
        pressStart = millis();
        pressed = true;
        released = false;
      }
    }else{ 
      if (pressed){  //makes sure that it doesn't change state after reset
        stage = !stage;
        pressed = false;
      }
      released = true;      
    }
  }
  lastButtonRead = buttonRead;

  if (stage == STAGE_1){ //first stage
    if ((millis() - pressStart > resetTime) && pressed){ //checks for long press
        pressed = false;
        for (int i = 0; i < SEG_SIZE; ++i)
          segLightStates[i] = LOW;
        segIndex = DP;
    }
    blink(blinkState, lastBlink, blinkInterval);
    segIndex = moveSegment(segIndex, isNeutral);
  }else{  //second stage
    setSegment(segIndex);
  }
  lightSegments(segIndex);
}


void lightSegments(int segmentOmit){ //lights the segments
  for (int i = 0; i < SEG_SIZE; ++i){
    if (i != segmentOmit)
      digitalWrite(segments[i], segLightStates[i]);    
  }    
}

void blink(byte &blinkState, unsigned long &lastBlink, unsigned long blinkInterval){ //blinks the selected segment
  if (millis() - lastBlink > blinkInterval){
    digitalWrite(segments[segIndex], blinkState);
    lastBlink = millis();
    blinkState = !blinkState;   
  }
}

int moveSegment(int currentSegment, byte &wasNeutral){ //function for stage 1 joystick segment movent
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

void setSegment(int currentSegment){ // function for stage 2 on/off switch
  int yVal = analogRead(pinY);
  if (yVal > MAX_TRESHHOLD)
    segLightStates[currentSegment] = LOW;
  if (yVal < MIN_TRESHHOLD)
    segLightStates[currentSegment] = HIGH;
  digitalWrite(segments[currentSegment], segLightStates[currentSegment]);
}

// void buttonPress(){
//   byte buttonRead = digitalRead(pinSW);
//   byte buttonOn = !buttonRead;
//   if (buttonOn){
//     if (millis() - lastPressDebounce > debounceInterval){    
//       buttonPressed = true;
//       buttonReleased = false; 
//       pressStart = millis();
//       Serial.println("p");         
//     }
//     lastPressDebounce = millis();   
//   }else{
//     if (millis() - lastReleaseDebounce > debounceInterval){   
//       buttonReleased = true;
//       Serial.println("r");
//     }
//     lastReleaseDebounce = millis();
//   }
// }


