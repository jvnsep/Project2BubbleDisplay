# Project 2 Bubble Display

## Abstract
This paper is about the process of developing embedded system that function as stopwatch with additional features for further development. The program involves study of SevSeg Library by Dean Reading and understand how to use it. The design circuitry and connection of the system mainly for 4-digit 7-segment LED display with N-channel enhancement mosfet switches connected on its common cathodes. Also, the need of limiting current resistor for power efficiency of the system.

## 1. Introduction
The project design an embedded system of a stop watch with accuracy count in decisecond. The project show how programming proceed and understand importance charateristic of the circuit. The program involves display of timer in 100ms, debounce of push button switches, and use of external interuppt. The circuit comprises of Microcontroller, a 4-Digits 7-Segments LED display, few power mosfets, several resistors, couple of LEDs and push button switches. The circuit design require to identify value of resistors for limiting current to anode of LED display and triggering voltage for mosfet switches. Futhermore, the order connection of these resistors to microcontroller output pins be decided. The difficulty in this project is to program push button switches operate in various state, example alternate start and stop, and hold press state to be use to other features.



## 2. Method

### a. Project Development

![alt text](https://github.com/jvnsep/Project2BubbleDisplay/blob/master/result/flow.png "Development Flow Chart")

### b. Circuit Design
Diagram: 
![alt text](https://github.com/jvnsep/Project2BubbleDisplay/blob/master/result/circuit.png "Circuit Diagram")

Image: 
![alt text](https://github.com/jvnsep/Project2BubbleDisplay/blob/master/result/picture.jpg "Circuit Picture")

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


