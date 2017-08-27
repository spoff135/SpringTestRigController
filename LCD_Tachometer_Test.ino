// This #include statement was automatically added by the Particle IDE.
#include "Serial_LCD_SparkFun.h"

/*
This is a port of the wonderful library found here: http://playground.arduino.cc/Code/SerLCD
The documentation there will explain all the functions available, and I expect them all to work
EXCEPT when it says `lcd.print()` If you're copying example code, you'll need to change those to
`Serial1.print()`... and that's it!

The wiring is simple- Provide appropriate power and ground (there are 5v and 3.3v varieties
of the LCD) and connect the LCD's `RX` pin to the Core or Photon's `TX` pin!
*/

// Initialize the LCD library
Serial_LCD_SparkFun lcd = Serial_LCD_SparkFun();

String gateStatusStr = "n/a";

unsigned long flagCount = 0;
unsigned long lastFlagCount = 0;
unsigned int rpm = 0;
unsigned long timeOld = 0;
unsigned long timeNow = 0;
unsigned long timeBetween = 0;
unsigned long minFlagTime = 30; //Minimum time between flags - this means any speed over 2000 RPM won't count.
float rpmAverage = 0;


void setup() {
  // Start with a blank slate
  lcd.setBrightness(20);
  lcd.clear();

  //Interrupt 0 is digital pin 2, so that is where the IR detector is connected
  //Triggers on FALLING (change from HIGH to LOW)
  attachInterrupt(D4, flag_hit, FALLING);

}


void loop() {

  lcd.setCursor(1,1);
	Serial1.print(String(rpm) + " RPM   ");
  lcd.setCursor(2,1);
  Serial1.print(String(flagCount) + " Cycles   ");
/*
  //Don't process interrupts during calculations
  detachInterrupt(0);
  //Restart the interrupt processing
  attachInterrupt(0, flag_hit, FALLING);
*/
	delay(1000);
}

void flag_hit()
{
  timeNow = millis();
  timeBetween = timeNow - timeOld;

  if(timeBetween > minFlagTime){  // This fix ignores noise that seems to occur at high speeds
    rpm = (60*1000) / (timeBetween); // one flags per rotation
    timeOld = timeNow;
    //Update count
    flagCount++;
  }
}
