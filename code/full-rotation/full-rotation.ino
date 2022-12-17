/*
 * Experiment with number of steps for a full rotation of a
 * 28BYJ-48 stepper motor + ULN2003 driver board
 *
 */

const uint8_t STATE_SEQUENCE[] = {B0011, B0110, B1100, B1001}; // full drive
const uint8_t MAX_STATE_SEQUENCE = sizeof(STATE_SEQUENCE) - 1;

void initCoilsPins()
{
	// PORTD maps to Arduino digital pins 0 to 7. Pins 0 and 1 are TX and RX, manipulate with care.
	DDRD = DDRD | B11110000;  // pins 7,6,5,4 as OUTPUT - Left motor
}

void setCoils(uint8_t state)
{
	// PORTD maps to Arduino digital pins 0 to 7
	// Motor - pins 4,5,6,7   -> PORTD bits[7-4] = state bits[3-0]
	// (a & ~mask) | (b & mask)
	// a -> PIND  b->(state << 4)  mask->B11110000
	PORTD = (PORTD & B00001111) | (state << 4); // implicit mask in the second part
}

void doSteps(int16_t howmany)
{
	uint8_t seq_index = 0;
	for (int i=0; i<howmany; i++)
	{
		setCoils(STATE_SEQUENCE[seq_index]);
		seq_index ++;
		if (seq_index > MAX_STATE_SEQUENCE) seq_index = 0;
		delayMicroseconds(2000);
	}
}

void setup() {
	initCoilsPins();
	delay(1000);
}

void loop() {
	// Theoretical value: 32 * 63,6840 = 2037,8864
	// Practical value: 2048 (slips, gear teeth engagement, etc.)
	doSteps(2048*4); // various rotations to appreciate the drift
	delay(2000);
}
