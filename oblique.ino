/*
  LiquidCrystal Library - Autoscroll

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

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

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 modified 7 Nov 2016
 by Arturo Guadalupi

*/

// include the library code:
#include <LiquidCrystal.h>
#include "strategies.hpp"
//String strategies[] = {	"Do we need holes?",
// 	"You are an engineer",
// 	"Reverse",
// };

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int gPromptCounter = 0;
int gScrollCounter = 0;
const int gPromptMillis = 60*60*1000;
const int gScrollCounterMax = 200;

int gCurrentPromptLength;
int gFirstCharTop = 0;
int gLastCharTop;
int gFirstCharBottom;
int gLastCharBottom;
int gLCD_Length = 16;
int gNumOfLines = 0;
int gOffsetLines = 0;



unsigned int gCurrentIdx = 0;

void setup() {

  // set up the LCD's number of columns and rows:
  lcd.begin(gLCD_Length, 2);
}

void loop() {
  gCurrentPromptLength = strategies[gCurrentIdx].length();
  gNumOfLines = (gCurrentPromptLength / gLCD_Length) + 1;

  // set first line

  if (gNumOfLines == 1) {
    gFirstCharTop = gOffsetLines * gLCD_Length;
    gLastCharTop = gCurrentPromptLength;
    gFirstCharBottom = -1;
    gLastCharBottom = -1;
  } else if (gNumOfLines == 2) {  // if current prompt longer than top line
    gFirstCharTop = gOffsetLines * gLCD_Length;
    gLastCharTop = gOffsetLines * gLCD_Length + gLCD_Length;
    gFirstCharBottom = gOffsetLines * gLCD_Length + gLCD_Length;
    gLastCharBottom = gOffsetLines * gLCD_Length + gCurrentPromptLength;
  } else if (gNumOfLines > 2) {
    gFirstCharTop = gOffsetLines * gLCD_Length;
    gLastCharTop = gOffsetLines * gLCD_Length + gLCD_Length;
    gFirstCharBottom = gOffsetLines * gLCD_Length + gLCD_Length;
    gLastCharBottom = gOffsetLines * gLCD_Length + 2 * gLCD_Length;
  }


  // set the cursor to (0,0):

  lcd.setCursor(0, 0);
  lcd.print(strategies[gCurrentIdx].substring(gFirstCharTop, gLastCharTop));
  lcd.setCursor(0, 1);
  lcd.print(strategies[gCurrentIdx].substring(gFirstCharBottom, gLastCharBottom));


  if (++gScrollCounter >= gScrollCounterMax) {
    gScrollCounter = 0;
    gOffsetLines += 2;
    lcd.clear();
    if (gOffsetLines >= gNumOfLines) {
      gOffsetLines = 0;
    }
  }

  if (millis() > (gPromptCounter * gPromptMillis)) {
    gPromptCounter++;
    lcd.clear();
    gCurrentIdx = (gCurrentIdx + 1) % (sizeof(strategies)/sizeof(strategies[0]));
  }
}