# Project 2 Bubble Display

## Abstract
This paper is about the process of developing embedded system that function as stopwatch with additional features for further development. The program involves study of SevSeg Library by Dean Reading and understand how to use it. The design circuitry and connection of the system mainly for 4-digit 7-segment LED display with N-channel enhancement mosfet switches connected on its common cathodes. Also, the need of limiting current resistor for power efficiency of the system.

## 1. Introduction
The project design an embedded system of a stop watch with accuracy count in decisecond. The project show how programming proceed and understand importance charateristic of the circuit. The program involves display of timer in 100ms, debounce of push button switches, and use of external interuppt. The circuit comprises of Microcontroller, a 4-Digits 7-Segments LED display, few power mosfets, several resistors, couple of LEDs and push button switches. The circuit design require to identify value of resistors for limiting current to anode of LED display and triggering voltage for mosfet switches. Futhermore, the order connection of these resistors to microcontroller output pins be decided. The difficulty in this project is to program push button switches operate in various state, example alternate start and stop, and hold press state to be use to other features.



## 2. Method

### a. Project Development

![alt text](https://github.com/jvnsep/Project2BubbleDisplay/blob/master/result/flow.png "Development Flow Chart")

### b. Circuit Design
Circuit Diagram: 
![alt text](https://github.com/jvnsep/Project2BubbleDisplay/blob/master/result/circuit.png "Circuit Diagram")

Materials:
1. Teensy 3.2 Microcontroller
2. QDSP-6064 4-Digits 7-Segments LED display
3. 4pcs IRLU8743PbF Power MOSFETs
2. 8pcs. 390Ω resistors connected from μC pins to anodes LED display
3. 4pcs. 1kΩ resistors conneted to drain of mosfets
4. 2pcs push button switches connected from μC pins to ground 

### c. Program Design

![alt text](https://github.com/jvnsep/Project2BubbleDisplay/blob/master/result/program.png "Program Flow Chart")

## 3. Results

### a. Pins Assigment

| Display Pin No. | Function    | Others | Teensy Pin No. |
| :-------------: |:------------|:------:|:--------------:|
|1	|Cathode 1	|Mosfet 1	|2|
|2	|Anode e	|	|8|
|3	|Anode c	|	|7|
|4	|Cathode 3|	Mosfet 3|	4|
|5	|Anode dp|	|	6|
|6	|Cathode 4	|Mosfet 4|	5|
|7	|Anode g|	|	18|
|8	|Anode d|	|	17|
|9	|Anode f|	|	16|
|10	|Cathode 2|	Mosfet 2|	3|
|11	|Anode b|	|	15|
|12	|Anode a|	|	14|
|		||Switch 1|	20|
|		||Switch 2|	19|

### b. Limiting Current Resistor

1. At Anode pins of LED Display
    
    Vf = 1.6 V; If = 5mA
   
    Vr = 3.3 - 1.6 = 1.7 V 
        
    R = 1.7 V / 0.005 A = 340 ohm
    
    so: Resistor is 390 ohm

2. At series to gate of Mosfet
   
   Vgs is positive value to turn "ON" and zero will turn "OFF"
   
   1kohm resistor in series with gate provide limit current from capacitive discharge of mosfet gate source junction
     
### c. SevSeg Library

1. At function sevseg.begin

    set:    byte hardwareConfig = N_TRANSISTORS;      - option of using n-type mosfet switch for anodes of LED display
    
2. At sevseg.setBrightness

    set:    100      - set the brightness of LED display 1 to 100
    
### d. Timer Accuracy

Link: [Timer Accuracy Video](https://github.com/jvnsep/Project2BubbleDisplay/blob/master/result/timer.mp4)
   
## 4. Discussions

### a. Circuit

From circuit diagram above, the main component aside from teensy microcontroller, is QDSP-6064 4-Digits 7-Segments LED display. This LED display have 12 pins, for 7 segment anodes (a to g) and a decimal point anode, the other pins are for 4 digit common cathodes. Each anode are connected to 390 ohms resistors to provide limiting current. On the other hand, the common cathodes are driven by n-channel enhance mosfet switches that will connect to ground. This complete the circuit of segment anodes and cpmmon cathodes of the LED display. To turn ON the mosfet switch, a positve voltage supply by microcontroller to gate that will control Voltage gate to source Vgs. However, a 1k ohm resistor in between gate and microcontroller output pin to function as a limiting current. So to fire the LED digit this microcontroller pin must be high or positive voltage. The two push buttons connected to input pin will be use as Start/Stop and Reset button for the stop watch control. The actual circuit is shown below image. There additional two LEDs it will be use for other design option.

Actual Circuit: 
![alt text](https://github.com/jvnsep/Project2BubbleDisplay/blob/master/result/picture.jpg "Circuit Picture")

### b. Program

First, include SegSev Library and its object show below. This library comprises of two files to be include in lib folder, they are SevSeg.cpp and SevSeg.h.    
        
        #include "SevSeg.h"
        
        SevSeg sevseg;   

Assign pins for controller and LED display, also declare constants and variables to be use in the program.

        const uint8_t RESETPIN = 20;                // input pin for the RESET push button
        const uint8_t STARTOPPIN = 19;              // input pin for the START/STOP push button
        const uint8_t LEDPIN1 = 9;                  // output pin for single LED 1
        const uint8_t LEDPIN2 = 10;                 // output pin for single LED 2

        uint8_t pushPin;                            // variable identify the pin of push button that being pressed
        bool stopstate = true;                      // variable if state for start or stop counter
        byte buttonType = 1;                        // variable which push button was pressed, START/STOP is 1 or RESETis 0
        int deciSeconds = 0;                        // variable to count in desiseconds
        int randNumber;                

Initialize Sevseg object by assign pins and configure compatible to the circuit. This will setup the LED display.

        byte numDigits = 4;                       // number of digits of LED display 
        byte digitPins[] = {2, 3, 4, 5};          // μC pins link to cathodes of LED display
        byte segmentPins[] = {14, 15, 7, 17, 8, 16, 18, 6}; // μC pins link to anodes of LED display
        byte hardwareConfig = N_TRANSISTORS;      // option of using n-type mosfet switch for anodes of LED display
        bool resistorsOnSegments = false;         // 'false' means resistors are on digit pins
        bool updateWithDelays = false;            // Default 'false' is Recommended
        bool leadingZeros = false;                // Use 'true' if you'd like to keep the leading zeros
        bool disableDecPoint = false;             // Use 'true' if your decimal point doesn't exist or isn't connected

        // intialize the setting of SevSeg object
        sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
        updateWithDelays, leadingZeros, disableDecPoint); // define the pins and setting
        sevseg.setBrightness(100);                 // set the brightness of LED display 1 to 100
        
Also, include in the set up and initialization the pin mode and attach interrupt of push buttons.

        pinMode(RESETPIN, INPUT_PULLUP);          // declare the RESETPIN as an INPUT PULLUP
        pinMode(STARTOPPIN, INPUT_PULLUP);        // declare the STARTOPPIN as an INPUT PULLUP
        attachInterrupt(digitalPinToInterrupt(RESETPIN), resetISR, FALLING); // reset button external interrupts
        attachInterrupt(digitalPinToInterrupt(STARTOPPIN), startISR, FALLING); // start stop button external interrupts
        pinMode(LEDPIN1, OUTPUT);                 // declare the LEDPIN1 as an OUTPUT
        pinMode(LEDPIN2, OUTPUT);                 // declare the LEDPIN2 as an OUTPUT
        
Counter function count in deciseconds and calculate time difference for every count. The time difference will carry over to the next count. This means that the time difference will not accumulates and increases. 

        void countNow(){
          static unsigned long timer = millis();    // set initial timer time to current time
          static int difference = 0;                // set 0 initially time differnce after count
           difference = millis() - timer;           // calculation difference, it maybe greater than 100
            if (difference >= 100) {                // count if time decisecond 
              timer += difference;                  // accumulate time with the exact difference not 100
              deciSeconds ++;                       // 100 milliSeconds is equal to 1 deciSecond
              if (deciSeconds == 10000) {           // Reset to 0 after counting for 1000 seconds.
                deciSeconds=0;
              }
            }
        }

Set up each push button its ISR. This way this interrupt function can call common push button function shown below.

        // ISR function for attachinterrupt Reset push button
        void resetISR(){
          pushPin = RESETPIN;                       // set variable pushPin to RESETPIN constant
          pushbutton();                             // call function for debounce and execute reset time to 0
        }

        // ISR function for attachinterrupt start/stop push button
        void startISR(){
          pushPin = STARTOPPIN;                     // set variable pushPin to RESETPIN constant
          pushbutton();                             // call function for debounce and execute stop watch
        }

Common debounce function can be reuse for any push button with the program. It can identify which push button was press and switch to case on the code below.

        void pushbutton(){
          const uint32_t DEBOUNCEDELAY = 500;       // constant debouce delay of 100ms 
          static uint32_t previousTime_ms = 0;      // first call variable 
          static bool lastbuttonstate = LOW;        // previous state from push button

          // If state is due to noise or press
          if (digitalRead(pushPin) != lastbuttonstate){ 
            previousTime_ms = millis();             // reset the previous time
          }
          // if elapse time is equal or greater than DEBOUNCEDELAY
          if((millis() - previousTime_ms) >= DEBOUNCEDELAY){ 
            switch (pushPin) {                      // select case by push button pin number
              case STARTOPPIN:                      // start stop push button was pressed
                stopstate = !stopstate;             // toggle from start to stop and vice versa
                previousTime_ms = millis();         // reset the previous time
                break;                              // exit case
              case RESETPIN:                        // reset push button was pressed
                if (stopstate == 1) {               // if counter is stop
                  deciSeconds = 0;                  // reset counter to 0
                  sevseg.setNumber(deciSeconds, 1); // decimal
                  sevseg.refreshDisplay();          // run display refresh repeatedly
                }
                break;                              // exit case
            }
            previousTime_ms = millis();             // reset the previous time
          }  
          lastbuttonstate = digitalRead(pushPin);   // save the reading latest button state
        }

### c. Problem

### d. Future Improvement
