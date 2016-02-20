/*
MenuDemo

A simple menu manager for the Arduino environment.

This sample shows how to provide a menu-driven utility
with minimal use of RAM.
Strings are stored in flash memory.
Menu operation controlled through a menu manager array.

A single switch statement selects menu actions.
This may be inappropriate for large, complex menus, but works
sufficiently well for small-scale menus as on AVR-series microcontrollers.

This supports multi-level menus i.e. main menu and sub-menus
Each menu item is automatically numbered (easy to add/drop items)
Menu items are selected by the user by entering the menu item number.

Supports entry of strings and menu entries as required.



Copyright notice 2011
GPL 2.1

Tom Waldock, 2011

*/

#include <WProgram.h>
#include <avr/pgmspace.h>
#include <Streaming.h>
#include <Time.h>
#include "Menu.h"
#include "MemoryFree.h"


void setup () {
  
  Serial.begin(9600);
  
  Serial << "Starting MenuSample.  Memory="  << freeMemory() << endl; 
  
  
}


void loop () {

  Menu myMenu;
  
  strncpy(myMenu.mySetting,"Initial sample setting", ENTRY_FIELD_SIZE);

  myMenu.begin();
  Serial << "Init Menu:"  << freeMemory() << endl; 
  myMenu.TerminalMenuManager();
  
  Serial << "Completed MenuSample ...  Retrying ... " << endl;
  delay(2000);

}

