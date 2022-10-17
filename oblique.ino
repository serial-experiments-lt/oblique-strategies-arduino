/*
Oblique strategies for Arduino by serial-experiments-lt

The circuit:
	* LCD RS pin to digital pin 12
	* LCD Enable pin to digital pin 11
	* LCD D4 pin to digital pin 5
	* LCD D5 pin to digital pin 4
	* LCD D6 pin to digital pin 3
	* LCD D7 pin to digital pin 2
	* LCD R/W pin to ground
	* 10K resistor:
	* ends to +5V and ground
	* wiper to LCD VO pin (pin 3)
*/

// include the library code:
#include "strategies.hpp"
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int gPromptCounter = 0;
int gScrollCounter = 0;
const int gPromptMillis = 60 * 60 * 1000;	 // strategy refreshes every hour
const int gScrollCounterMax = 200;			 // scroll every 200ms

int gCurrentPromptLength;
int gFirstCharTop;
int gLastCharTop;
int gFirstCharBottom;
int gLastCharBottom;
int gLCD_Length = 16;
int gNumOfLines;
int gOffsetLines = 0;

unsigned int gCurrentIdx = 0;

void setup() {
	// set up the LCD's number of columns and rows:
	lcd.begin(gLCD_Length, 2);
}

void loop() {
	gCurrentPromptLength = strategies[gCurrentIdx].length();
	gNumOfLines = (gCurrentPromptLength / gLCD_Length) + 1;

	gFirstCharTop = gOffsetLines * gLCD_Length;
	if (gNumOfLines == 1) {
		// if the prompt is one line long
		gLastCharTop = gCurrentPromptLength;
		gFirstCharBottom = -1;
		gLastCharBottom = -1;
	} else if (gNumOfLines == 2) {
		// if current prompt longer than top line
		gLastCharTop = (gOffsetLines + 1) * gLCD_Length;
		gFirstCharBottom = gLastCharTop;
		gLastCharBottom = gOffsetLines * gLCD_Length + gCurrentPromptLength;
	} else if (gNumOfLines > 2) {
		// if the prompt is larger than the lcd
		gLastCharTop = (gOffsetLines + 1) * gLCD_Length;
		gFirstCharBottom = gLastCharTop;
		gLastCharBottom = (gOffsetLines + 2) * gLCD_Length;
	}

	lcd.setCursor(0, 0);
	lcd.print(strategies[gCurrentIdx].substring(gFirstCharTop, gLastCharTop));
	lcd.setCursor(0, 1);
	lcd.print(strategies[gCurrentIdx].substring(gFirstCharBottom, gLastCharBottom));

	if (++gScrollCounter >= gScrollCounterMax) {
		gScrollCounter = 0;
		gOffsetLines = gOffsetLines < (gNumOfLines - 1) ? gOffsetLines + 2 : 0;
		if (gNumOfLines > 2) {      
			lcd.clear();
		}
	}

	if (millis() > (gPromptCounter * gPromptMillis)) {
		gPromptCounter++;
		lcd.clear();
		gCurrentIdx = (gCurrentIdx + 1) % (sizeof(strategies) / sizeof(strategies[0]));
	}
}