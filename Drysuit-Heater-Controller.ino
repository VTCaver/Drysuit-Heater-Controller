/*This sketch is used to control a drysuit heater using pulse width modulation.  The program allows you to set the length of the cycle, currently set at 2 seconds.
 * There are 3 settings, currently 50%, 75% and 100% duty cycles. You can change these to whatever you want.  The controller is designed to be controlled by a 
 * single toggle switch.  Every time you turn the heater on, it switches to the next heater setting in the loop Low, Med, Hi, Low Med Hi.  It includes 3 indicator 
 * LEDs; One for each power setting.  The on board LED is used to show when the heater is being cycled for testing and troubleshooting.
*/

//Initiallizes EPPROM. This is where the previous power setting is stored.
#include <EEPROM.h>
int address = 0; // You are supposed to get at least 100,000 writes to an eeprom address.  If you were worried you could add code to change the address every cycle
int Power = EEPROM.read(address) %3;  //Instead of trying to cycle the value of the temperature setting, I just divide the value by 3 and take the remainder. 0 = low 1 = medium 2 = hi

//These are the power level indicator pins.  Connect them to 3 diffent colored LEDs or an RGB LED if you want to reduce the number of parts.  They can share a resistor on the negative
//side since the LEDs should never be on at the same time.
const int LowPin =  3;
const int MedPin =  4;
const int HiPin =  2;

//Power output pin.  This is what will turn on and off depending on the power level.  Connect to Mosfet to control heater.  
//It can also be the onboard LED pin (Pin 1 for the Trinket, Pin 13 for the UNO) to help with testing.
const int LEDPin =  1;

const int cycletime = 2; // duration of the total cycle in seconds

// Percentage of the time the power is low, Med and Hi (Duty Cycle)  Play with these numbers to get different power outputs
const int LowDC = 50; // Percent
const int MedDC = 75; // Percent 
const int HiDC = 100; // Percent


int DC = 0; //During operation this value will switch between the duty cycles of the 3 power settings (LowDC, MedDC, HiDC)
unsigned long cycletimemill = cycletime*1000; // converts the cycletime to milliseconds
unsigned long ontime = 0; //initialized variable. This will be the amount of time the heater will be on per cycle in miliseconds
void setup() {
  //Sets the pins to outputs
  pinMode(LowPin, OUTPUT);
  pinMode(MedPin, OUTPUT);
  pinMode(HiPin, OUTPUT);
  pinMode(LEDPin, OUTPUT);
  
EEPROM.write(0,(Power+1)); //Changes the output each time power is cycled
}

void loop() {
  ontime = cycletimemill*DC/100;
  unsigned long t=millis() % (cycletimemill);
  digitalWrite(LEDPin, (t <= ontime));

// Switch between power settings.  Each case turns the correct indicator pin and makes DC equal the correct duty cycle for the power setting
  switch (Power) {
    case 0:    // LOW
      digitalWrite(LowPin, HIGH);
      digitalWrite(MedPin, LOW);
      digitalWrite(HiPin, LOW);
      DC=LowDC;
      break;
    case 1:    // Med
      digitalWrite(LowPin, LOW);
      digitalWrite(MedPin, HIGH);
      digitalWrite(HiPin, LOW);
      DC=MedDC;
      break;
    case 2:    // Hi
      digitalWrite(LowPin, LOW);
      digitalWrite(MedPin, LOW);
      digitalWrite(HiPin, HIGH);
      DC=HiDC;
      break;
  }
}





