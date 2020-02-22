#include "mbed.h"
#include "mbed.h"
#include "EFM32_SegmentLCD.h"
#include "EFM32_CapSenseSlider.h"
#include <math.h>
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
using namespace std;

DigitalOut led1(LED1);
DigitalOut led2(LED0);
 
Serial pc(USBTX, USBRX);
Serial device(PD4, PD5);
char msg[25];
int Speed;
int volatile speed1 = 0;
int Cruise;
int Cruise_Set;
int Saved_Speed;
int CAN_flag = 1;


silabs::EFM32_SegmentLCD segmentDisplay;
char* speed;

void getline() {
    while(device.getc() != 'S');    // wait for the start of a line
    for(int i=0; i<25; i++) {
        msg[i] = device.getc();
        if(msg[i] == 'R') {
            msg[i] = 0;
            return;
        }
    }
    error("Overflowed message limit");
    return;
}



/*-------------------- Main ----------------------*/
int main() {

    
    while (1) {
        
    getline();    
    pc.printf("%s\n",msg);
    sscanf(msg, "%d,%d,%d,%d,%d", &Speed, &Cruise_Set, &Cruise, &Saved_Speed, &CAN_flag);

    
    if (CAN_flag) SegmentLCD_Symbol(LCD_SYMBOL_ANT,1);
    else SegmentLCD_Symbol(LCD_SYMBOL_ANT,0);
  
    if (Cruise_Set == 1){
        SegmentLCD_Symbol(LCD_SYMBOL_PAD0,1);
        segmentDisplay.Number(Saved_Speed);
        }
    else {SegmentLCD_Symbol(LCD_SYMBOL_PAD0,0); SegmentLCD_NumberOff(); }
    
    if (Cruise == 1){
        SegmentLCD_Symbol(LCD_SYMBOL_PAD1,1);
        }
    else SegmentLCD_Symbol(LCD_SYMBOL_PAD1,0);
        
        
            ostringstream oss;
            string mystr;
            oss << Speed;
            mystr=oss.str();
                       
    
            if (Speed >= 10){
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
            }
            else{
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