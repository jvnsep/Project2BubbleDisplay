# Project 2 Bubble Display
 
## 1. Introduction
The project is to design an embedded system function like a stop watch and expandable for additional feature. The project involves programming and circuit design. The circuit comprises of Teensy 3.1/3.2 Microcontroller, a 4-Digits 7-Segments LED display, few power mosfets, several resistors, couple of LEDs and push button switches. The program involves display of counter every 100ms, debounce of push button switches, and use of external interuppt.



## 2. Method
### a. Circuit Design
Inline-style: 
![alt text](https://github.com/jvnsep/Project2BubbleDisplay/blob/master/result/circuit.png "Circuit Diagram"


4pcs power mosfet act as switch triggering the 4 cathodes of LED display connect to ground, 8pcs. 390Ω resistors connected from μC pins to anodes,7 segments and decimal point of LED display, 4pcs. 1kΩ resistors conneted to drain of mosfets, 2pcs push button switches connected from μC pins to ground 
