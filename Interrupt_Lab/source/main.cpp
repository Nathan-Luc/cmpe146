#include <cstdint> 
#include "L0_LowLevel/LPC40xx.h"
#include <project_config.hpp>

#include <cstdint>
#include <iterator>
#include "L3_Application/oled_terminal.hpp"

#include "utility/log.hpp"
#include "utility/time.hpp"
#include "LabGPIO.hpp"
   // Initialize button and led here
  LabGPIO button3(0, 29);
  LabGPIO led3(1, 18);
  
  LabGPIO button2(0, 30);
  LabGPIO led2(1, 24);
  
  LabGPIO button1(1, 15);
  LabGPIO led1(1, 26);
  
  LabGPIO button0(1, 19);
  LabGPIO led0(2, 3);
 
  //variables to reach EC
  int CB0=0;
  int CB1=0;
  int CB2=0;
  int CB3=0;
 
  // variables for EC
  int increment =0; // used for for loops
  int count=0; // used for distguishing between dots and bars
  int word_count=1; // count the amount of characters in the morse array
  int translated_count=1;// count the amount of characters in the translated  array
  char morse[1000]= {}; // used to store morse code 
  char translated[100]={};// used to store translated morse code
  OledTerminal oled_terminal; //make a variable for the Oled display

void Part0(); 
void DemoButton(); //Part 2 of Lab
void EC(); // morse code translator
void morse_translate(); // translate dots and dashes to alphabet

int main() 
{ 
  LOG_INFO("Start");
  oled_terminal.Initialize(); //turn on screen
  button0.setPulldown(); //Have button 0 pull down resistor activated
  button1.setPulldown();// Have button 1 pull down resistor activated
  bool change =true; //used to change between program

  while(true)
  {    
    if(change){ 
    // Logic to read if button has been RELEASED and if so, TOGGLE LED state;
      DemoButton();
    }
    if(CB0%2 == 1 && CB1%6 ==5 && CB2%4 ==0 && CB3%6 ==5) //condition that allows the program to go to extra credit
    {   change=false; //used to close off the previous program
          EC();
    }
    
  }
  return 0;
     
}
void Part0(){
   // Lab 0
  // 1) Find and choose an onboard LED to manipluate.
  // 2) Use the schematic to figure out which pin it is connected to
  // 3) Use FIODIR to set that pin as an output
   LPC_GPIO2->PIN |= (1 << 3);
   LPC_GPIO2->DIR |= (1 << 3);
  while (true)
  {
    // 4) use FIOCLR to set the pin LOW, turning ON the LED
    LPC_GPIO2->CLR = (1 << 3);
    LOG_INFO("Turning LED ON!");
    Delay(500);  // Delay in milliseconds
    // 5) use FIOSET to set the pin HIGH, turning OFF the LED
    LPC_GPIO2->SET = (1 << 3);
    LOG_INFO("Turning LED OFF!");
    Delay(500);
  }
}
void DemoButton()
{
   if(button0.ReadBool())
   {    
      Delay(10); //debounce
       if(button0.ReadBool() != true)
       {
        LOG_INFO("Button 0 pressed!");
        led0.toggle();
        CB0++;
       }           
   }
  
   if(button1.ReadBool())
   {     
      Delay(10); //debounce
       if(button1.ReadBool() != true)
       {
        LOG_INFO("Button 1 pressed!"); 
        led1.toggle();
        CB1++;
       }           
   }
       
   if(button2.ReadBool())
   {        
      Delay(10); //debounce
       if(button2.ReadBool() != true)
       {
        LOG_INFO("Button 2 pressed!");
        led2.toggle();
        CB2++;
       }           
   }
   if(button3.ReadBool())
   {     
      Delay(10); //debounce
       if(button3.ReadBool() != true)
       {
        LOG_INFO("Button 3 pressed!");
        led3.toggle();
        CB3++;
       }           
     
   }
}
  
void EC(){
      translated[0]='?'; //used to make the array start at a known variable
      morse[0]='?'; // used to make the array start at a known variable
      if(button3.ReadBool()){
          Delay(150); // used to debounce and to increment the count slowly
          count++;
          if(count>2){
          oled_terminal.printf("-");
          morse[word_count]= '-';
          word_count++;
           count=-1; //used to prevent a dot to appear after releasing the button
          }
          if(button3.ReadBool() != true && (count != 0) && (count != -1))
          {
          oled_terminal.printf(".");
          morse[word_count] = '.';
          word_count++;
          count = 0; 
       
          }
      }
  if(button2.ReadBool()) // used to display on terminal
  { Delay(10);
      if(button2.ReadBool() != true){
    for(increment =1; increment<=word_count; increment++){
     printf("%c", morse[increment]);
     morse_translate();
    }
    printf("\n");
    for(increment=1; increment<=translated_count;increment++)
    {
        printf("%c", translated[increment]);
    }
    
        oled_terminal.Clear();
      }
      
  }
    if(button1.ReadBool()) //used to provide space between letters
  { Delay(10);
      if(button1.ReadBool() != true){
      oled_terminal.printf("&"); 
      morse[word_count] = '&';
          word_count++;
      }
      
  }
  if(button0.ReadBool()) //used to provide space between words
  { Delay(10);
      if(button0.ReadBool() != true){
      oled_terminal.printf("|"); 
      morse[word_count] = '|';
          word_count++;
      }
      
  }
   
}
void morse_translate(){ //translate the morse array into the alphabet
if(morse[increment] =='|')
     {
         translated[translated_count]=' ';
         translated_count++;
     }
    if((morse[increment-3] =='&' || morse[increment-3] == '|' || morse[increment-3] == '?') && morse[increment-2] =='.' && morse[increment-1] == '-' && morse[increment] == '&'){
        translated[translated_count]='a';
        translated_count++;
        }
     if((morse[increment-5] =='&' || morse[increment-5] == '|' || morse[increment-5] == '?') && morse[increment-4] =='-' && morse[increment-3] == '.' && morse[increment-2] == '.' && morse[increment-1] == '.'  && morse[increment] == '&'){
        translated[translated_count]='b';
        translated_count++;
        }
      if((morse[increment-5] =='&' || morse[increment-5] == '|' || morse[increment-5] == '?') && morse[increment-4] =='-' && morse[increment-3] == '.' && morse[increment-2] == '-' && morse[increment-1] == '.'  && morse[increment] == '&'){
        translated[translated_count]='c';
        translated_count++;
        }
       if((morse[increment-4] =='&' || morse[increment-4] == '|' || morse[increment-4] == '?') &&  morse[increment-3] == '-' && morse[increment-2] == '.' && morse[increment-1] == '.'  && morse[increment] == '&'){
        translated[translated_count]='d';
        translated_count++;
        }
        if((morse[increment-2] =='&' || morse[increment-2] == '?' || morse[increment-2] == '|') && morse[increment-1] =='.'&& morse[increment] =='&')
        {
         translated[translated_count]='e';
         translated_count++;
        }
        if((morse[increment-5] =='&' || morse[increment-5] == '|' || morse[increment-5] == '?') && morse[increment-4] =='.' && morse[increment-3] == '.' && morse[increment-2] == '-' && morse[increment-1] == '.'  && morse[increment] == '&'){
        translated[translated_count]='f';
        translated_count++;
        }
       if((morse[increment-5] =='&' || morse[increment-5] == '|' || morse[increment-5] == '?') && morse[increment-4] =='.' && morse[increment-3] == '.' && morse[increment-2] == '.' && morse[increment-1] == '.'  && morse[increment] == '&'){
        translated[translated_count]='h';
        translated_count++;
        }
       if((morse[increment-3] =='&' || morse[increment-3] == '|' || morse[increment-3] == '?') && morse[increment-2] =='.' && morse[increment-1] == '.' && morse[increment] == '&'){
        translated[translated_count]='i';
        translated_count++;
        }
        if((morse[increment-5] =='&' || morse[increment-5] == '|' || morse[increment-5] == '?') && morse[increment-4] =='.' && morse[increment-3] == '-' && morse[increment-2] == '-' && morse[increment-1] == '-'  && morse[increment] == '&'){
        translated[translated_count]='j';
        translated_count++;
        }
        if( (morse[increment-4] =='&' || morse[increment-4] == '|' || morse[increment-4] == '?') && morse[increment-3] == '-' && morse[increment-2] == '.' && morse[increment-1] == '-'  && morse[increment] == '&'){
        translated[translated_count]='k';
        translated_count++;
        }
        if((morse[increment-5] =='&' || morse[increment-5] == '|' || morse[increment-5] == '?') && morse[increment-4] =='.' && morse[increment-3] == '-' && morse[increment-2] == '.' && morse[increment-1] == '.'  && morse[increment] == '&'){
        translated[translated_count]='l';
        translated_count++;
        }
        if((morse[increment-3] =='&' || morse[increment-3] == '|' || morse[increment-3] == '?') && morse[increment-2] =='-' && morse[increment-1] == '-' && morse[increment] == '&'){
        translated[translated_count]='m';
        translated_count++;
        }
        if((morse[increment-3] =='&' || morse[increment-3] == '|' || morse[increment-3] == '?') && morse[increment-2] =='-' && morse[increment-1] == '.' && morse[increment] == '&'){
        translated[translated_count]='n';
        translated_count++;
        }
        if((morse[increment-4] =='&' || morse[increment-4] == '|' || morse[increment-4] == '?') && morse[increment-3] == '-' && morse[increment-2] == '-' && morse[increment-1] == '-'  && morse[increment] == '&'){
        translated[translated_count]='o';
        translated_count++;
        }
           if((morse[increment-5] =='&' || morse[increment-5] == '|' || morse[increment-5] == '?') && morse[increment-4] =='.' && morse[increment-3] == '-' && morse[increment-2] == '-' && morse[increment-1] == '.'  && morse[increment] == '&'){
        translated[translated_count]='p';
        translated_count++;
        }
          if((morse[increment-5] =='&' || morse[increment-5] == '|' || morse[increment-5] == '?') && morse[increment-4] =='-' && morse[increment-3] == '-' && morse[increment-2] == '.' && morse[increment-1] == '-'  && morse[increment] == '&'){
        translated[translated_count]='q';
        translated_count++;
        }
         if((morse[increment-4] =='&' || morse[increment-4] == '|' || morse[increment-4] == '?') && morse[increment-3] == '.' && morse[increment-2] == '-' && morse[increment-1] == '.'  && morse[increment] == '&'){
        translated[translated_count]='r';
        translated_count++;
        }
         if((morse[increment-4] =='&' || morse[increment-4] == '|' || morse[increment-4] == '?') && morse[increment-3] == '.' && morse[increment-2] == '.' && morse[increment-1] == '.'  && morse[increment] == '&'){
        translated[translated_count]='s';
        translated_count++;
        }
        if((morse[increment-2] =='&' || morse[increment-2] == '?' || morse[increment-2] == '|') && morse[increment-1] =='-'&& morse[increment] =='&')
        {
         translated[translated_count]='t';
         translated_count++;
        }
         if((morse[increment-4] =='&' || morse[increment-4] == '|' || morse[increment-4] == '?') && morse[increment-3] == '.' && morse[increment-2] == '.' && morse[increment-1] == '-'  && morse[increment] == '&'){
        translated[translated_count]='u';
        translated_count++;
        }
            if((morse[increment-5] =='&' || morse[increment-5] == '|' || morse[increment-5] == '?') && morse[increment-4] =='.' && morse[increment-3] == '.' && morse[increment-2] == '.' && morse[increment-1] == '-'  && morse[increment] == '&'){
        translated[translated_count]='v';
        translated_count++;
        }
         if((morse[increment-4] =='&' || morse[increment-4] == '|' || morse[increment-4] == '?') && morse[increment-3] == '.' && morse[increment-2] == '-' && morse[increment-1] == '-'  && morse[increment] == '&'){
        translated[translated_count]='w';
        translated_count++;
        }
            if((morse[increment-5] =='&' || morse[increment-5] == '|' || morse[increment-5] == '?') && morse[increment-4] =='-' && morse[increment-3] == '.' && morse[increment-2] == '.' && morse[increment-1] == '-'  && morse[increment] == '&'){
        translated[translated_count]='x';
        translated_count++;
        }
            if((morse[increment-5] =='&' || morse[increment-5] == '|' || morse[increment-5] == '?') && morse[increment-4] =='-' && morse[increment-3] == '.' && morse[increment-2] == '-' && morse[increment-1] == '-'  && morse[increment] == '&'){
        translated[translated_count]='y';
        translated_count++;
        }
            if((morse[increment-5] =='&' || morse[increment-5] == '|' || morse[increment-5] == '?') && morse[increment-4] =='-' && morse[increment-3] == '-' && morse[increment-2] == '.' && morse[increment-1] == '.'  && morse[increment] == '&'){
        translated[translated_count]='z';
        translated_count++;
        }
}
 