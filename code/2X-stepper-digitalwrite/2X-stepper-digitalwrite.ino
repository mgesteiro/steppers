/*
 * Test 2X stepper motors using digitalWrite()
 * 
 * https://web.archive.org/web/20211130201930/https://www.arduino.cc/en/Reference/PortManipulation
 * https://web.archive.org/web/20211121130756/https://playground.arduino.cc/Code/BitMath/
 * http://graphics.stanford.edu/~seander/bithacks.html
 * 
 */

const static uint8_t states_sequences[][8] = {
    { B0001, B0010, B0100, B1000, B0001, B0010, B0100, B1000 }, // wave drive - repeated 2x
    { B0011, B0110, B1100, B1001, B0011, B0110, B1100, B1001 }, // full drive - repeated 2x
    { B0001, B0011, B0010, B0110, B0100, B1100, B1000, B1001 }  // half drive
};
const static char states_sequences_names[][5] = { 
  "WAVE\0", 
  "FULL\0", 
  "HALF\0"
};

//unsigned int delays[] = {500, 500, 500}; // <-- use this to visualize drivers' LEDs
const static unsigned int delays[] = { 
  2250, // wave 
  2100, // full
  1050  // half
};

// step tracking
//int targetSteps = 32; // <-- use this to visualize drivers' LEDs
int targetSteps = 2048;
int currentStep = targetSteps; // to start-over and get status display

// driving mode
int currentMode = 2; // to start-over and get status display
unsigned int stepDelay = delays[currentMode];


/*
 *   S U P P O R T   F U N C T I O N S
 */
void initCoilsPins()
{
    for(int pin = 4; pin <= 11; pin++)
        pinMode(pin, OUTPUT);
}

void setCoils(uint8_t stateR, uint8_t stateL)
{
    // RightMotor - pins 4,5,6,7
    for(int pin = 4; pin <= 7; pin++) {
        digitalWrite(pin, bitRead(stateR, pin-4));
    }
    // LeftMotor  - pins 8,9,10,11
    for(int pin = 8; pin <= 11; pin++) {
        digitalWrite(pin, bitRead(stateR, pin-8));
    }
}

/*
 *   S E T U P   &   L O O P
 */
unsigned long timen=0, timef = 0;
void setup() {
  Serial.begin(115200);
  Serial.println("Stepper motors (ULN2003 + 28BYJ-48) driving using digitalWrite()");
  initCoilsPins(); 
  setCoils(0, 0);  // disable coils
}

void loop() {
  // tracking the current step we are on
  currentStep++;
  if(currentStep >= targetSteps) {
    setCoils(0, 0);  // disable coils
    currentStep = 0;
    ++currentMode %= 3; // change mode
    stepDelay = delays[currentMode];
    // status display
    Serial.print("Maximun setCoils() time: ");
    Serial.println(timef);
    Serial.print("Mode: ");
    Serial.print(states_sequences_names[currentMode]);
    Serial.print("  Delay: ");
    Serial.println(stepDelay);
    delay(1000);
  }
  // action
  timen=micros();
  setCoils(
    states_sequences[currentMode][currentStep % 8],
    states_sequences[currentMode][currentStep % 8]
  );
  timen = micros() - timen;   
  if (timen>timef) timef=timen;
  //delay(stepDelay); // <-- use this to visualize drivers' LEDs
  delayMicroseconds(stepDelay);
}
