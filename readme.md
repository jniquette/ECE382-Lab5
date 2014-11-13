ECE382-Lab5 MSP430 Interrupts and Remote Control Decoding
=========================================================

##Purpose
The purpose of this lab is to demonstrate how to decode IR remote control signals using an IR sensor connected to our MSP430's. We'll configure the MSP430 to listen for an interrupt from the IR sensor and then debug the remote control signal.

##Day 1 Activities

###Analyzing a IR Pulse
The following image is an example of an IR pulse annotated with what lines of code are executed at each part of the pulse. The code comes from test5.c
![alt text](https://github.com/jniquette/ECE382-Lab5/blob/master/img/ir_pulse.png "IR Pulse")

###Setup
This image shows our MSP430G2553 (on a TI Launchpad) connected to the IR sensor via Pin 2.6 (labeled XIN), Ground, and VCC. The MSP430 is also connected to an Algilent 1680A1 Logic Analyzer. I will be using "Remote #10" for my lab.
![alt text](https://github.com/jniquette/ECE382-Lab5/blob/master/img/setup.jpg "Setup of the Board")

###Timing of pulses
The logic analyzer was used to measure the length of time of each bit of data. The infrared remotes also emit a standardized pulse to signal the start and end of the data packet. This particular remote uses 32 bits of data in each packet when a button is pressed.

The photo below shows what a data packet looks like on the logic analyzer with the following settings:
- Trigger: Falling Edge
- Sampling Period: 500ns
![alt text](https://github.com/jniquette/ECE382-Lab5/blob/master/img/analyzer.jpg "Data Packet for Power Button")
The bits for a repeat button press are ommited because they are not used in this lab.

###Measurements and Calculations
Pulse                       |Duration (ms)  |Timer A counts |# of Half-Bit Widths   |Counts Per 1 Half-Bit
----------------------------|---------------|---------------|-----------------------|---------------------
Start logic 0 half-pulse    |9.0156         |8903           |18                     |495
Start logic 1 half-pulse    |4.4844         |4433           |9                      |493
Data 1 logic 0 half-pulse   |0.5875         |588            |1                      |588
Data 1 logic 1 half-pulse   |1.6719         |1650           |3                      |550
Data 0 logic 0 half-pulse   |0.5695         |565            |1                      |565
Data 0 logic 1 half-pulse   |0.5406         |526            |1                      |526
Stop logic 0 half-pulse     |0.5656         |562            |1                      |562
Stop logic 1 half-pulse     |39.9219        |39456          |73                     |540

The following values were calculated to find a confidence interval that would accurately catch 99.9999426697% of bits
Average Counts per bit = 539.8325
Standard Deviation = 33.84871
z* for 99.9999426697% confidence interval = 4.864648
Confidence Interval = average +/- (z* x StDev) = (375.1704227, 704.4945316)

###Button Codes
Using the above data, the code was modified to convert pulse durations into data bits. The codes retrieved for Remote #10 
Button      |Code
------------|----------
POWER       |0x2CD3750A
ZERO        |0x2CD3057A
ONE         |0x2CD32D52
TWO         |0x2CD33D42
THREE       |0x2CD33946
BIG_UP      |0x2CD36C13
BIG_DOWN    |0xACD3601F
BIG_LEFT    |0xACD3304F
BIG_RIGHT   |0xACD3205F
BIG_MIDDLE  |0xACD37807

##Code
All code for this project can be found in the following three files:
1. lab5.c - C code that contains the main method, initMSP430 subroutine, and the interrupt subroutines.
2. lab5.h - Contains several constant definitions to improve readability of code.
3. nokia.asm - Assembly code for functionality of the Nokia 1707 display.

##Functionality

###Required Functionality
This required a demonstration of using two buttons on the remote control to turn on and off two LED lights on the MSP430. I used buttons 1 and 2 for the red and green LED's respectively. This functionality was demonstrated as working in class.

###A Functionality
I was unable to get A Functionality to work due to problems seeming to stem from the nokia.asm file. Both the assembly file and lab5.c use Timer A and they seem to interfere with each other. There also seems to be some overwriting of data between the two files. After a couple hours of debugging and some EI time with the instructor leading to no success, I ran out of time working on A Functionality.

##Documentation Statement
Nothing to report.