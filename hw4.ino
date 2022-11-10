#define D1 0
#define D2 1
#define D3 2
#define D4 3
#define STAGE_1 0
#define MIN_TRESHHOLD 200
#define MAX_TRESHHOLD 800
#define DISPLAY_COUNT 4 //no of digit displays
#define REG_SIZE 8  //encoding length
#define MAX_DIGIT 16  //maximum no of digits

const int latchPin = 11; // STCP to 12 on Shift Register
const int clockPin = 10; // SHCP to 11 on Shift Register
const int dataPin = 12; // DS to 14 on Shift Register

// declare all the digit pins
const int segD1 = 7;
const int segD2 = 6;
const int segD3 = 5;
const int segD4 = 4;

const int pinSW = 2; // digital pin connected to switch output
const int pinX = A0; // A0 - analog pin connected to X output
const int pinY = A1; // A1 - analog pin connected to Y output

const unsigned long debounceInterval = 50;
const unsigned long resetTime = 2000; //time the button stays pressed for reset

int xValue = 0;
int yValue = 0;

//pin list
int displayDigits[DISPLAY_COUNT] = {
  segD1, segD2, segD3, segD4
};
//value for each display
int displayValues[DISPLAY_COUNT] = {
  0, 0, 0, 0
};
//encoding matrix
byte registers[MAX_DIGIT][REG_SIZE] = {
  {1, 1, 1, 1, 1, 1, 0, 0}, //0
  {0, 1, 1, 0, 0, 0, 0, 0}, //1
  {1, 1, 0, 1, 1, 0, 1, 0}, //2
  {1, 1, 1, 1, 0, 0, 1, 0}, //3
  {0, 1, 1, 0, 0, 1, 1, 0}, //4
  {1, 0, 1, 1, 0, 1, 1, 0}, //5
  {1, 0, 1, 1, 1, 1, 1, 0}, //6
  {1, 1, 1, 0, 0, 0, 0, 0}, //7
  {1, 1, 1, 1, 1, 1, 1, 0}, //8
  {1, 1, 1, 1, 0, 1, 1, 0}, //9
  {0, 0, 1, 1, 1, 0, 1, 0}, //A(10)
  {0, 0, 0, 1, 1, 0, 1, 0}, //B(11)
  {0, 0, 0, 1, 0, 1, 1, 0}, //C(12)
  {0, 1, 1, 1, 1, 0, 1, 0}, //D(13)
  {1, 0, 0, 1, 1, 1, 1, 0}, //E(14)
  {1, 0, 0, 0, 1, 1, 1, 0}, //F(15)
};

byte stage = STAGE_1;
int digitIndex = D1;

byte isNeutralX = true;
byte isNeutralY = true;

byte dpOn = false;

byte buttonPressed = false;
unsigned long lastPressDebounce = 0;
unsigned long lastReleaseDebounce = 0;
unsigned long pressStart = 0;
byte buttonRead = false;
byte lastButtonRead = false;
byte pressed = false;
byte released = true;

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  pinMode(pinSW, INPUT_PULLUP);

  for (int i = 0; i < DISPLAY_COUNT; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], HIGH); 
  }
  //Serial.begin(9600);
}

void loop() {
  buttonRead = digitalRead(pinSW);

  if (buttonRead != lastButtonRead)
    lastPressDebounce = millis();
  if (millis() - lastPressDebounce > debounceInterval) {  //classic debounce
    if (buttonPressed == buttonRead)
      buttonPressed = !buttonRead;
      
    if (buttonPressed) {
      if (released) {    //the button should not be pressed if it was not released
        pressStart = millis();
        pressed = true;
        released = false;
      }
    }else { 
      if (pressed) {  //makes sure that it doesn't change state after reset
        stage = !stage;
        pressed = false;
      }
      released = true;      
    }
  }
  lastButtonRead = buttonRead;
  

  if (stage == STAGE_1) { //first stage
    if ((millis() - pressStart > resetTime) && pressed) { //checks for long press
        pressed = false;
        for (int i = 0; i < DISPLAY_COUNT; ++i)
          displayValues[i] = 0;
        digitIndex = D1;
    }

    blink(dpOn);
    digitIndex = moveDigit(digitIndex, isNeutralX);
  }else {  //second stage
    dpOn = true;
    setDigit(digitIndex, isNeutralY);
  }
  display(digitIndex, dpOn);
}

void blink(byte &blinkState) { //blinks the selected segment
  const unsigned long blinkInterval = 500;
  static unsigned long lastBlink = 0;
  if (millis() - lastBlink > blinkInterval) {
    lastBlink = millis();
    blinkState = !blinkState;   
  }
}

int moveDigit(int currentDigit, byte &wasNeutral) { //function for stage 1 joystick digit movement
  int xVal = analogRead(pinX);
  int direction = 0;

  if (xVal > MAX_TRESHHOLD)
    direction = -1;
  if (xVal < MIN_TRESHHOLD)
    direction = 1;

  if (direction == 0)
    wasNeutral = true;
  else if (wasNeutral) {
    wasNeutral = false;
    currentDigit = currentDigit + direction;
    if (currentDigit < 0) currentDigit = 0;
    if (currentDigit > DISPLAY_COUNT - 1) currentDigit = DISPLAY_COUNT - 1;
    return currentDigit; 
  }
  return currentDigit; 
}

void setDigit(int currentDigit, byte &wasNeutral) { // function for stage 2 digit switch
    int yVal = analogRead(pinY);
    int increment = 0;
    if (yVal > MAX_TRESHHOLD)
      increment -= 1;
    if (yVal < MIN_TRESHHOLD)
      increment += 1;

    if (increment == 0)
      wasNeutral = true;
    else if (wasNeutral) {
      wasNeutral = false;
      displayValues[currentDigit] += increment;   
      if (displayValues[currentDigit] < 0) displayValues[currentDigit] = 0;  
      if (displayValues[currentDigit] > MAX_DIGIT - 1) displayValues[currentDigit] = MAX_DIGIT - 1; 
    }        
}

void display(int currentDigit, byte dpState) {
  for (int i = 0; i < DISPLAY_COUNT; ++i) {
    if (i == currentDigit) registers[displayValues[currentDigit]][REG_SIZE-1] = dpState;
    writeReg(registers[displayValues[i]]);
    registers[displayValues[currentDigit]][REG_SIZE-1] = 0;
    digitalWrite(displayDigits[i],LOW);  
    delay(1);
    digitalWrite(displayDigits[i],HIGH);  
  }
}

void writeReg(byte encoding[]) {
  digitalWrite(latchPin, LOW); 
  for (int i = 0; i < REG_SIZE; i++) { // MSBFIRST 
    digitalWrite(clockPin, LOW);
    digitalWrite(dataPin, encoding[i]);
    digitalWrite(clockPin, HIGH);
  }
  digitalWrite(latchPin, HIGH);
}




