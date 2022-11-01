#define SEM_GREEN 0
#define SEM_RED 1
#define SEM_YELLOW 2
#define LIGHT_ON 1
#define LIGHT_OFF 0

#define STAGE_1 1
#define STAGE_2 2
#define STAGE_3 3
#define STAGE_4 4

const int interruptPin = 2;
const int buzzPin = 3;
const int redCarPin = 6;
const int yellowCarPin = 7;
const int greenCarPin= 8;
const int redPedestrianPin = 11;
const int greenPedestrianPin = 12;

const int buzzTone = 2000;
const int buzzTimerStage3 = 1000;
const int buzzTimerStage4 = 700;
const int buzzDuration = 200;

const int blinkInterval = 300;

const int buttonPressDelay = 8000, durationStage2 = 3000, durationStage3 = 8000, durationStage4 = 4000;

const unsigned long daBounce = 50;

volatile unsigned long lastPress = 0;

volatile byte buttonPressed = false;
int actionStage = STAGE_1;
int carSemColor = SEM_GREEN, pedSemColor = SEM_RED;

unsigned long startTimer = 0;   //for measuring the stage delays
unsigned long blinkStart = 0, buzzStart = 0;   //for the delays between the buzzer sounds and green light blinks
byte blinkGreen = 0;


void setup() {
  pinMode(interruptPin, INPUT_PULLUP);

  pinMode(redCarPin, OUTPUT);
  pinMode(yellowCarPin, OUTPUT);
  pinMode(greenCarPin, OUTPUT);
  pinMode(redPedestrianPin, OUTPUT);
  pinMode(greenPedestrianPin, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(interruptPin), startSemaphore, RISING);
}

void loop() {
  switch (actionStage) {
  case STAGE_1:

    carSemColor = SEM_GREEN; pedSemColor = SEM_RED;
    setSem(carSemColor, pedSemColor);
    if (buttonPressed){
      startTimer = millis();
      actionStage = STAGE_2;
    }

    break;    
  case STAGE_2:

    if (millis() - startTimer >= buttonPressDelay){
      carSemColor = SEM_YELLOW; pedSemColor = SEM_RED;
      setSem(carSemColor, pedSemColor);
      if (millis() - startTimer >= buttonPressDelay+durationStage2){
        startTimer = millis();
        buzzStart = millis();
        actionStage = STAGE_3;
      }      
    }
  
    break;
  case STAGE_3:

    carSemColor = SEM_RED; pedSemColor = SEM_GREEN;
    setSem(carSemColor, pedSemColor);
    if (millis() - buzzStart >= buzzTimerStage3){
      tone(buzzPin, buzzTone, buzzDuration);
      buzzStart = millis();
    }
    if (millis() - startTimer >= durationStage3){
        startTimer = millis();
        blinkGreen = LIGHT_ON;
        blinkStart = millis();
        buzzStart = millis();
        actionStage = STAGE_4;
    }  

    break;
  case STAGE_4:
  
    if (millis() - blinkStart >= blinkInterval){
      blinkGreen = !blinkGreen;
      digitalWrite(greenPedestrianPin, blinkGreen);
      blinkStart = millis();
    }
    if (millis() - buzzStart >= buzzTimerStage4){
      tone(buzzPin, buzzTone, buzzDuration);
      buzzStart = millis();
    }
    if (millis() - startTimer >= durationStage4){
      buttonPressed = false;
      actionStage = STAGE_1;
    }  

    break;
  default:
    break;
  }
}

void startSemaphore(){  //interrupt function, didn't use debounce because I am not affected by multiple presses
  if (millis() - lastPress > daBounce) {
    buttonPressed = true;
    lastPress = millis();
  }
}

void setSem(int carSem, int pedSem){  //function for coloring the semaphores
  byte carRed = LIGHT_OFF, carYellow = LIGHT_OFF, carGreen = LIGHT_OFF;
  byte pedRed = LIGHT_OFF, pedGreen = LIGHT_OFF;

  if (carSem == SEM_RED) carRed = LIGHT_ON;
  else if (carSem == SEM_YELLOW) carYellow = LIGHT_ON;
  else carGreen = LIGHT_ON;

  if (pedSem == SEM_RED) pedRed = LIGHT_ON;
  else pedGreen = LIGHT_ON;

  digitalWrite(redCarPin, carRed);
  digitalWrite(yellowCarPin, carYellow);
  digitalWrite(greenCarPin, carGreen);
  digitalWrite(redPedestrianPin, pedRed);
  digitalWrite(greenPedestrianPin, pedGreen);
}


