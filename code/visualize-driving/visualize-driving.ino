/*
 * Visualize the different driving modes of a
 * 28BYJ-48 stepper motor + ULN2003 driver board
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

// step tracking
int targetSteps = 32;  // 4 iterations of states_sequences
int currentStep = targetSteps; // to start-over and get status display

// driving mode
int currentMode = 2; // to start-over and get status display
unsigned int stepDelay = 500; // half a second, so we can visualize


/*
 *   S U P P O R T   F U N C T I O N S
 */

void initCoilsPins()
// 4 pins total: from 4 to 7
{
	for(int pin = 4; pin <= 7; pin++)
		pinMode(pin, OUTPUT);
}

void setCoils(uint8_t state)
{
	for(int pin = 4; pin <= 7; pin++) {
		digitalWrite(pin, bitRead(state, pin-4));
	}
}


/*
 *   S E T U P   &   L O O P
 */

void setup() {
	Serial.begin(115200);
	Serial.println("Stepper motor (28BYJ-48 + ULN2003) driving visualization experiment");
	initCoilsPins(); 
	setCoils(0);  // disable coils
}

void loop() {
	// tracking the current step we are on
	currentStep++;
	if(currentStep >= targetSteps) {
		setCoils(0);  // disable coils
		currentStep = 0;
		++currentMode %= 3; // change mode
		// status display
		Serial.print("Mode: ");
		Serial.print(states_sequences_names[currentMode]);
		Serial.print("  Delay: ");
		Serial.println(stepDelay);
		delay(1000);
	}
	// action
	setCoils( states_sequences[currentMode][currentStep % 8]);
	delay(stepDelay);
}
