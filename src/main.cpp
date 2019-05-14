#include "SevSeg.h"
/* 
  ***************************************************************************************************
  ** Project 2: Bubble Display                                                                     **
  ***************************************************************************************************
  This is an embedded system project with 4 digit display, as counter that counts up which show deciseconds.
  This project function as a stop watch and reaction tester.
  This program uses the SevSeg library by Dean Reading.

  The circuit:
    1. Teensy 3.1/3.2 Microcontroller
    2. a 4-Digits 7-Segments LED display
    3. 4pcs power mosfet act as switch triggering the 4 cathodes of LED display connect to ground
    4. 8pcs. 390Ω resistors connected from μC pins to anodes,7 segments and decimal point of LED display
    5. 4pcs. 1kΩ resistors conneted to drain of mosfets
    6. 2pcs push button switches connected from μC pins to ground 

  Created: 14 May 2019 by Joven Sepulveda 
            Weltec School of Engineering
            MG7013 Embedded Systems
*/

SevSeg sevseg;                            // Open object for a seven segment controller from SevSeg library

// constant set pin numbers:
const uint8_t RESETPIN = 20;              // input pin for the RESET push button
const uint8_t STARTOPPIN = 19;            // input pin for the START/STOP push button
const uint8_t LEDPIN1 = 9;                // output pin for single LED 1
const uint8_t LEDPIN2 = 10;               // output pin for single LED 2

uint8_t pushPin;                          // variable identify the pin of push button that being pressed
bool startbuttonState = false;            // variable if state of start = false or stop = true
bool optionState = true;                  // variable if state of reaction tester = false or stop watch = true
int deciSeconds = 0;                      // variable to count in desiseconds
int randNumber;                           // variable for variable number

// forward declaration of function
void resetbutton();                       // ISR function for reset push button
void startstopbutton();                   // ISR function for start or stop push button
void pushbutton();                        // debouce function for both push button 
void optionAStopWatch();                  // stop watch function for option 1
void pressedbutton();                     // function to know what button was pressed, START/STOP or RESET push button
void timerNow();                          // function to calculate the time in count of decisecond

// setting sevseg object, pin modes and interrupt function
void setup() {
  byte numDigits = 4;                     // number of digits of LED display 
  byte digitPins[] = {1, 3, 4, 5};        // μC pins link to cathodes of LED display
  byte segmentPins[] = {14, 15, 7, 17, 8, 16, 18, 6}; // μC pins link to anodes of LED display
  byte hardwareConfig = N_TRANSISTORS;    // option of using n-type mosfet switch for anodes of LED display
  bool resistorsOnSegments = false;       // 'false' means resistors are on digit pins
  bool updateWithDelays = false;          // Default 'false' is Recommended
  bool leadingZeros = false;              // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = false;           // Use 'true' if your decimal point doesn't exist or isn't connected
  
  // intialize the setting of SevSeg object
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
  updateWithDelays, leadingZeros, disableDecPoint); // define the pins and setting
  sevseg.setBrightness(90);               // set the brightness of LED display 1 to 100

  pinMode(RESETPIN, INPUT_PULLUP);        // declare the RESETPIN as an INPUT PULLUP
  pinMode(STARTOPPIN, INPUT_PULLUP);      // declare the STARTOPPIN as an INPUT PULLUP
  attachInterrupt(digitalPinToInterrupt(RESETPIN), resetbutton, FALLING); // reset button external interrupts
  attachInterrupt(digitalPinToInterrupt(STARTOPPIN), startstopbutton, FALLING); // start stop button external interrupts
  pinMode(LEDPIN1, OUTPUT);               // declare the LEDPIN1 as an OUTPUT
  pinMode(LEDPIN2, OUTPUT);               // declare the LEDPIN2 as an OUTPUT
}

// loop checks option which button press and execute its function
void loop() {
  static uint32_t previousTime_ms = 0;    // first call 0 for initial value for previousTime_ms
  static bool ledstate = false;           // ledstate for light on or off

  pressedbutton();                        // function to know which button was pressed
  if (optionState == true){               // optionstate is true, START/STOP push button was pressed
    optionAStopWatch();                   // execute stop watch function
    previousTime_ms = millis();           // reset previousTime_ms
    digitalWrite(LEDPIN1, false);         // led light 1 off
    digitalWrite(LEDPIN2, false);         // led light 2 off
  }
  else {                                  // optionstae is false, START/STOP or RESET push button
    deciSeconds = 0;                      // display set to zero
    sevseg.setNumber(deciSeconds, 1);     // set decimal
    sevseg.refreshDisplay();              // repeatedly refresh display
    if((millis() - previousTime_ms) >= 1000){ // blinking of two led every second
      previousTime_ms = millis();         // reset previousTime_ms
      ledstate = !ledstate;               // alternate ledstate for blink
    }
    digitalWrite(LEDPIN1, ledstate);      // output to led 1
    digitalWrite(LEDPIN2, ledstate);      // output to led 2
  }
}

// function to know which button was pressed and hold for 2sec, START/STOP or RESET push button
void pressedbutton(){
  static uint32_t previousTime_ms = 0;    // 0 for initial value for previousTime_ms 
  static bool lastbuttonstate = LOW;      // low for initial value for lastbuttonstate

  if (digitalRead(pushPin) != lastbuttonstate){ // if 
    previousTime_ms = millis();           // reset previousTime_ms if button press
  }

  // if pressed for 2sec and identify START/STOP or RESET push button
  if ((millis() - previousTime_ms >=2000) && (digitalRead(pushPin)==LOW)) {
    if (pushPin == STARTOPPIN) {
      optionState = true;
    }
    if (pushPin == RESETPIN) {
      optionState = false;
    }
  }
  lastbuttonstate = digitalRead(pushPin);

}
// function for attachinterrupt at instant push button, and allow debounce delay
void resetbutton(){
  pushPin = RESETPIN;
  pushbutton();
}

// function for attachinterrupt at instant push button and allow debounce delay
void startstopbutton(){
  pushPin = STARTOPPIN;
  pushbutton();
}

void timerNow(){
  static unsigned long timer = millis();
  static int differenceTime = 0;
   differenceTime = millis() - timer;
    if (differenceTime >= 100) {
      timer = timer + differenceTime; //+= 100;
      deciSeconds ++; // 100 milliSeconds is equal to 1 deciSecond
      if (deciSeconds == 10000) { // Reset to 0 after counting for 1000 seconds.
        deciSeconds=0;
      }
    }
}

void optionAStopWatch(){
    
  if (startbuttonState == true){
    timerNow();
  }
  /* else{
    timer = millis(); //+ differenceTime;
  } */
  sevseg.setNumber(deciSeconds, 1);
  sevseg.refreshDisplay(); // Must run repeatedly

}
void pushbutton(){
  const uint32_t DEBOUNCEDELAY = 500;       // constant debouce delay of 100ms 
  static uint32_t previousTime_ms = 0;      // first call variable 
  static bool lastbuttonstate = LOW;        // previous state from push button
  //static bool currentbuttonState = LOW;

  // If state is due to noise or press
  if (digitalRead(pushPin) != lastbuttonstate){ 
    previousTime_ms = millis();             // reset the previous time
  }
  // if elapse time is equal or greater than DEBOUNCEDELAY
  if((millis() - previousTime_ms) >= DEBOUNCEDELAY){ 
    // serial monitor output
    switch (pushPin) {
      case STARTOPPIN: // Common cathode
        startbuttonState = !startbuttonState;
        previousTime_ms = millis();   
        break;
      case RESETPIN: // Common anode
        if (startbuttonState == false) {
          deciSeconds = 0;
          sevseg.setNumber(deciSeconds, 1);
          sevseg.refreshDisplay(); // Must run repeatedly
        }
        break;
    }
    previousTime_ms = millis(); 
  } 
 
  lastbuttonstate = digitalRead(pushPin); // save the reading latest button state
}