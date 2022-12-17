/*
 * Test 2X stepper motors using direct access via DDRx,PORTx and PINx registers
 *
 * https://docs.arduino.cc/hacking/software/PortManipulation
 * https://idyl.io/arduino/how-to/manipulate-arduino-ports/  <-- INPUT_PULLUP using DDRx
 * https://playground.arduino.cc/Code/BitMath/
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

const static unsigned int delays[] = { 
	2250, // wave 
	2100, // full
	1050  // half
}; 

// step tracking
int targetSteps = 2048;
int currentStep = targetSteps; // to start-over and get status display

// driving mode
int currentMode = 2; // to start-over and get status display
unsigned int stepDelay = delays[currentMode];

// timing
unsigned long timen=0, timef = 0;


/*
 *   S U P P O R T   F U N C T I O N S
 */

void initCoilsPins()
{
	// PORTB maps to Arduino digital pins 8 to 13. The two high bits (6 & 7) map to the crystal pins and are not usable.
	DDRB = DDRB | B00001111;  // pins x,x,x,x,11,10,9,8 as OUTPUT - Right motor
	// PORTD maps to Arduino digital pins 0 to 7. Pins 0 and 1 are TX and RX, manipulate with care.
	DDRD = DDRD | B11110000;  // pins 7,6,5,4,x,x,x,x as OUTPUT - Left motor

}

void setCoils(uint8_t stateR, uint8_t stateL)
{
	// PORTB maps to Arduino digital pins 8 to 13 The two high bits (6 & 7) map to the crystal pins and are not usable
	// RightMotor - pins 11,10,9,8 -> PORTB bits[3-0] = stateR bits[3-0]
	// (a & ~mask) | (b & mask)
	// a -> PINB  b->stateR  mask->B00001111
	PORTB = (PORTB & B11110000) | (stateR & B00001111);

	// PORTD maps to Arduino digital pins 0 to 7
	// LeftMotor - pins 7,6,5,4 -> PORTD bits[7-4] = stateL bits[3-0]
	// (a & ~mask) | (b & mask)
	// a -> PIND  b->(stateL << 4)  mask->B11110000
	PORTD = (PORTD & B00001111) | (stateL << 4); // implicit mask in second part
}


/*
 *   S E T U P   &   L O O P
 */

void setup() {
	Serial.begin(115200);
	Serial.println("Stepper motors (ULN2003 + 28BYJ-48) driving using PORTx registers");
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
	delayMicroseconds(stepDelay);
}
