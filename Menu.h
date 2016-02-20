#ifndef __MENU_H__
#define __MENU_H__

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

This Menu.h file describes the Menu class.


This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

Copyright (c) GPL 2.1  Tom Waldock, 2011

*/

#include "MemoryFree.h"
#include <Streaming.h>
#include <Time.h>
#include <avr/pgmspace.h>


// String Table Index STI_
// Positions of strings in PROGMEM space.

// Menu entries
#define STI_MENU_0000 0
#define STI_MENU_1000 1
#define STI_MENU_2000 STI_MENU_1000 + 6
#define STI_MENU_3000 STI_MENU_2000 + 3

// Prompts
#define STI_MENU_PROMPT_1000 STI_MENU_3000 + 3
#define STI_MENU_PROMPT_2000 STI_MENU_PROMPT_1000 
#define STI_MENU_PROMPT_4000 STI_MENU_PROMPT_1000 
#define STI_MENU_PROMPT_5000 STI_MENU_PROMPT_1000 

// Response prompts
#define STI_MENU_RESPONSE_PROMPT_1000  STI_MENU_PROMPT_1000 + 1

// Messages
#define STI_MESSAGE_1000  STI_MENU_RESPONSE_PROMPT_1000 + 1

// Buffer sizes
#define ENTRY_FIELD_SIZE   64
#define MAX_STORED_RESPONSES 6


class Menu {

  public:
    Menu();
    ~Menu();
    void begin();
    boolean ShowMenu (const int Menu, const boolean bLinks);
    char GetMenuResponse( char* Prompt, const byte maxOptions);
    char* GetPromptResponse( char* Prompt, char* buffer, int bufsize);
    
    byte* GetMenuProperties(const int iMenu, byte* paMenu);
    int TerminalMenuManager();
    boolean GenerateMenu(char* pMenuBuffer, int iBufSize, boolean bTerminal);
    
    char mySetting[ENTRY_FIELD_SIZE];
   
   
   private:
    char* GetBuffer_P(const int StringIndex, char* pBuffer, int bufSize = ENTRY_FIELD_SIZE);
    boolean PerformAction( int iActionID);
    
    char gBuffer[ENTRY_FIELD_SIZE];  // widely-used buffer spaces
    char gPrompt[ENTRY_FIELD_SIZE];  //
    
    byte  iCurrentMenu;
    char  paMenuActions[MAX_STORED_RESPONSES];
};
  
#endif 



