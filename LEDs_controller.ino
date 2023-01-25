#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
    #include <avr/power.h> // Required for 16 MHz Adafruit Trinket.
#endif

/* ---------------------- LEDs variables and constants ---------------------- */
#define NUM_LEDS  10
#define UNITS_PIN  6
#define TENS_PIN   5

int unitsCounter = 0;
int tensCounter  = 0;
int totalCounter = 0;
Adafruit_NeoPixel units_strip(NUM_LEDS, UNITS_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel tens_strip(NUM_LEDS ,  TENS_PIN, NEO_GRB + NEO_KHZ800);

const uint32_t CLEAR      = units_strip.Color(0  ,   0,   0);
const uint32_t RED        = units_strip.Color(255,   0,   0);
const uint32_t ORANGE     = units_strip.Color(255,  50,   0);
const uint32_t YELLOW     = units_strip.Color(255, 127,   0);
const uint32_t GREEN      = units_strip.Color(0  , 255,   0);
const uint32_t LIGHT_BLUE = units_strip.Color(0  , 255, 255);
const uint32_t MED_BLUE   = units_strip.Color(0  , 127, 255);
const uint32_t NAVY_BLUE  = units_strip.Color(0  ,   0, 255);
const uint32_t PURPLE     = units_strip.Color(127,   0, 255);
const uint32_t PINK       = units_strip.Color(255,   0, 127);
const uint32_t WHITE      = units_strip.Color(255, 255, 255);
/* -------------------------------------------------------------------------- */

/* -------------------- Buttons variables and constants --------------------- */
#define TEN_INCR_PIN 9
#define INCR_PIN     8
#define DECR_PIN     7
#define VALID_PIN    4

#define DELAYVAL 500

boolean incrWasUp		= true;
boolean tenIncrWasUp	= true;
boolean decrWasUp		= true;

int isOn = 0;
String incomingByte; 
boolean validateWasUp = true;
boolean ledEnabled = false;
/* -------------------------------------------------------------------------- */

const uint32_t tensToColour(int tens)
{

	switch (tens) {
		case 0:
			return RED;
		case 1:
			return NAVY_BLUE;
		case 2:
			return YELLOW;
		case 3:
			return GREEN;
		case 4:
			return LIGHT_BLUE;
		case 5:
			return PINK;
		case 6:
			return ORANGE;
		case 7:
			return PURPLE;
		case 8:
			return MED_BLUE;
		case 9:
			return WHITE;
		default:
			return CLEAR;      
	}

}

void clearUnits()  //attention clearUnits met donc unitsCounter à -1;
{

	while (unitsCounter >= 0) {
		units_strip.setPixelColor(unitsCounter, CLEAR);
		units_strip.show();
		delay(150);
		unitsCounter--;
	}

}

void fastFillUnits(int nb)
{

  while (unitsCounter < nb) {
    units_strip.setPixelColor(unitsCounter, tensToColour(tensCounter));
    units_strip.show();
    unitsCounter++;
  }

}

void fillUnits(int nb)
{

	while (unitsCounter < nb) {
		units_strip.setPixelColor(unitsCounter, tensToColour(tensCounter));
		units_strip.show();
		delay(150);
		unitsCounter++;
	}

}

void clearTens()
{

	while (tensCounter >= 0) {
		tens_strip.setPixelColor(tensCounter, CLEAR);
		tens_strip.show();
		delay(150);
		tensCounter--;
	}

}

void setup()
{

	// These lines are specifically to support the Adafruit Trinket 5V
	// 16 MHz. Any other board, you can remove this part (but no harm
	// leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
	clock_prescale_set(clock_div_1);
#endif
	// END of Trinket-specific code.

	Serial.begin(9600);
	while (!Serial);
	
	pinMode(TENS_PIN, OUTPUT);
	pinMode(UNITS_PIN, OUTPUT);

	pinMode(TEN_INCR_PIN, INPUT_PULLUP);
	pinMode(INCR_PIN, INPUT_PULLUP);
	pinMode(DECR_PIN, INPUT_PULLUP);
	pinMode(VALID_PIN, INPUT_PULLUP);

	units_strip.begin();
	tens_strip.begin();
	units_strip.setBrightness(25);
	tens_strip.setBrightness(25);
}

void loop()
{

  if (Serial.available()>0){
    incomingByte = Serial.readString();
    if (incomingByte == "r"){
      clearTens();  
      clearUnits();
      unitsCounter = 0;
      tensCounter = 0;
      totalCounter = 0;
    }  
  }


	boolean tenIncrIsUp, incrIsUp, decrIsUp;
	boolean validateIsUp;

	tenIncrIsUp = digitalRead(TEN_INCR_PIN);
	incrIsUp 	= digitalRead(INCR_PIN);
	decrIsUp 	= digitalRead(DECR_PIN);

	if (tenIncrWasUp && !tenIncrIsUp) {
		delay(10);
		tenIncrIsUp = digitalRead(TEN_INCR_PIN);
		if (!tenIncrIsUp && totalCounter <= 90) { // (tensCounter * 10 + unitsCounter) 
			int nbUnits = totalCounter%10; //int nbUnits = unitsCounter;
     	if (nbUnits == 0){
				fillUnits(10);
        delay(750);
        tens_strip.setPixelColor(tensCounter, tensToColour(tensCounter));
        tens_strip.show();
        clearUnits();
        unitsCounter = 0;
        tensCounter++;
     	}
			else {
				fillUnits(10);
        delay(750);
        tens_strip.setPixelColor(tensCounter, tensToColour(tensCounter));
        tens_strip.show();
				clearUnits();
        tensCounter++;
				fillUnits(nbUnits);
			}
      totalCounter += 10;
		}
	}


  
	if (incrWasUp && !incrIsUp) {
		delay(10);
		incrIsUp = digitalRead(INCR_PIN);
    if (!incrIsUp && totalCounter <100){
  		if (totalCounter%10 >= 0 && totalCounter%10 < 9) { //unitsCounter >= -1 && unitsCounter < 9
  			units_strip.setPixelColor(unitsCounter, tensToColour(tensCounter));
  			unitsCounter++;
  		} else if (totalCounter%10 == 9) { //on passe à 10  -> (unitsCounter == 9)
        units_strip.setPixelColor(unitsCounter, tensToColour(tensCounter)); 
        units_strip.show();
        delay(750);
        tens_strip.setPixelColor(tensCounter, tensToColour(tensCounter));
        tens_strip.show();
  			clearUnits();
        unitsCounter = 0;
        tensCounter++;
  		} 
      totalCounter++;
    }
	}

  
	if (decrWasUp && !decrIsUp) {
		delay(10);
		decrIsUp = digitalRead(DECR_PIN);
    if (!decrIsUp && totalCounter > 0){
  		if (totalCounter%10 > 0) { //1
  			unitsCounter--;
  			units_strip.setPixelColor(unitsCounter, CLEAR);
  		} else if (totalCounter%10 == 0) { //1
  			tensCounter--;
  			tens_strip.setPixelColor(tensCounter, CLEAR);
        fastFillUnits(10);
        tens_strip.show();
  			delay(1500);
        unitsCounter--;
  			units_strip.setPixelColor(unitsCounter, CLEAR);
  		}
      totalCounter--;
      if (totalCounter == 0){
        unitsCounter = 0;
        tensCounter = 0;
      }
    }
	}



	units_strip.show();
	tens_strip.show();
	tenIncrWasUp = tenIncrIsUp;
	incrWasUp = incrIsUp;
	decrWasUp = decrIsUp;

  validateIsUp = digitalRead(VALID_PIN);
	if (validateWasUp && !validateIsUp) {
		delay(10);
		validateIsUp = digitalRead(VALID_PIN);
		if (!validateIsUp) {
      Serial.println(totalCounter);
		}
	}
	validateWasUp = validateIsUp;

}
