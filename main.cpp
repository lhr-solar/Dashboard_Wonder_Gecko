/**
 * main.cpp
 * @authors:    Matthew Yu
 *              William Blount
 * @last_modified: 2/22/20
 * @description:
 *      EFM32(WG-STK3800) Wonder Gecko Board Program. This program displays dashboard data onto its 8x20 Segment LCD.
 * @notes: Projected display configuration (see https://os.mbed.com/teams/SiliconLabs/code/EFM32_SegmentLCD//file/114aa75da77b/segmentlcd.h/):
 *      current speed   - display first 2 digits (we're never going above 50mph, let's face it), then "mph" on 3, 4, 5, 6, 7 of the LCDs
 * 
 *      cruise control enable
 *          disabled    - PAD0 off, PAD1 off
 *          enabled     - PAD0 on
 *      cruise control set - only on when cruise control enabled
 *          unset       - PAD0 on, PAD1 off
 *          set         - PAD0 on, PAD1 on
 *      regen enable    - GEK on
 *      CAN signal      - ANT on
 */

#include "mbed.h"
#include "EFM32_SegmentLCD.h"
#include "EFM32_CapSenseSlider.h"
#include <math.h>
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
using namespace std;

#define MAX_CHARS 25 // maximum amount of characters that the gecko should check before exiting
#define OFF 0
#define ON 1

DigitalOut led1(LED1);
DigitalOut led2(LED0);
 
Serial pc(USBTX, USBRX);
Serial device(PD4, PD5); // TX, RX
char msg[25] = {0}; // initialize to null

// data grabbed from serial line
int speed;
int cruiseControlEnabled;
int cruiseControlSet;
int regenEnabled;
int CANError;

silabs::EFM32_SegmentLCD segmentDisplay;

// get a new line from the serial connection to the dashboard
void getline() {
    // wait for readable line
    while(!device.readable());

    // read proceeding characters
    for(int i = 0; i < MAX_CHARS; i++) {
        // clear entry first before reading
        msg[i] = '\0'; 
        // grab value from line
        msg[i] = device.getc();
        if(msg[i] == '\n') {
            msg[i] = '\0';
            return;
        }
    }
    error("Overflowed message limit\n");
    return;
}

int main() {
    while (1) {   
        // get update from dashboard serial     
        getline();    
        pc.printf("%s\n",msg);

        // grab data
        sscanf(msg, "%d,%d,%d,%d,%d", 
            &speed, 
            &cruiseControlEnabled, 
            &cruiseControlSet, 
            &regenEnabled, 
            &CANError);

        // can flag display
        if(CAN_flag) {
            SegmentLCD_Symbol(LCD_SYMBOL_ANT, ON);
        }else {
            SegmentLCD_Symbol(LCD_SYMBOL_ANT, OFF);
        }

        // regen flag display
        if(regenEnabled) {
            SegmentLCD_Symbol(LCD_SYMBOL_GEK, ON);
        }else {
            SegmentLCD_Symbol(LCD_SYMBOL_GEK, OFF);
        }

        // cruise control display
        if(cruiseControlEnabled) {
            SegmentLCD_Symbol(LCD_SYMBOL_PAD0, ON;
            if(cruiseControlSet) {
                SegmentLCD_Symbol(LCD_SYMBOL_PAD1, ON);
            }else {
                SegmentLCD_Symbol(LCD_SYMBOL_PAD1, OFF);
            }
        }else {
            SegmentLCD_Symbol(LCD_SYMBOL_PAD0, OFF); 
            SegmentLCD_Symbol(LCD_SYMBOL_PAD1, OFF); 
        }

        ostringstream oss;
        string mystr;
        oss << speed;
        mystr=oss.str();

        if (Speed >= 10) {
            char *speed_str = new char[3 + 5];
            speed_str[0] = ' ';
            speed_str[1] = mystr.c_str()[0];
            speed_str[2] = mystr.c_str()[1];
            speed_str[3] = ' ';
            speed_str[4] = 'm';
            speed_str[5] = 'p';
            speed_str[6] = 'h';
            speed_str[7] = 0;
            segmentDisplay.Write(speed_str);
            delete[] speed_str;
        }else {
            char *speed_str = new char[2 + 6];
            speed_str[0] = ' ';
            speed_str[1] = mystr.c_str()[0];
            speed_str[2] = ' ';
            speed_str[3] = ' ';
            speed_str[4] = 'm';
            speed_str[5] = 'p';
            speed_str[6] = 'h';
            speed_str[7] = 0; 
            segmentDisplay.Write(speed_str);
            delete[] speed_str;
        }
    }
}