const int BITS_PER_SEC = 9600;

const int PIN_LED_RED = 6;
const int PIN_LED_GREEN = 5;
const int PIN_LED_BLUE = 3;

const int PIN_POT_RED = A0;
const int PIN_POT_GREEN = A2;
const int PIN_POT_BLUE = A4;

const int MIN_POT_VAL = 0;
const int MAX_POT_VAL = 1023;
const int MIN_LED_VAL = 0;
const int MAX_LED_VAL = 255;

int rLedVal = 0, bLedVal = 0, gLedVal = 0;
int rPotVal = 0, bPotVal = 0, gPotVal = 0;

void setup() {
  pinMode(PIN_POT_RED, INPUT);
  pinMode(PIN_POT_GREEN, INPUT);
  pinMode(PIN_POT_BLUE, INPUT);

  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_BLUE, OUTPUT);

  Serial.begin(BITS_PER_SEC);
}

void loop() {
  readPotRGB(rPotVal, gPotVal, bPotVal);

  rLedVal = mapPotToLed(rPotVal);
  gLedVal = mapPotToLed(gPotVal);
  bLedVal = mapPotToLed(bPotVal);

  writeLedRGB(rLedVal, gLedVal, bLedVal);
}

void readPotRGB(int &red, int &green, int &blue){
  red = analogRead(PIN_POT_RED);
  green = analogRead(PIN_POT_GREEN);
  blue = analogRead(PIN_POT_BLUE);
}

void writeLedRGB(int red, int green, int blue){
  analogWrite(PIN_LED_RED, red);
  analogWrite(PIN_LED_GREEN, green);
  analogWrite(PIN_LED_BLUE, blue);  
}

int mapPotToLed(int potentiometerValue){
  int ledValue = map(potentiometerValue, MIN_POT_VAL, MAX_POT_VAL, MIN_LED_VAL, MAX_LED_VAL);
  return ledValue;
}