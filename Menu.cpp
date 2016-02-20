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

#include "Menu.h"



// Strings stored in Program space
prog_char s_MENU_0000[] PROGMEM = "Exit This Menu" ;
prog_char s_MENU_1000[] PROGMEM = "Main Menu" ;   
prog_char s_MENU_1010[] PROGMEM = "Memory Report" ;   
prog_char s_MENU_1020[] PROGMEM = "Sample Main Menu Action I" ;
prog_char s_MENU_1030[] PROGMEM = "Sub-Menu Alpha" ;
prog_char s_MENU_1040[] PROGMEM = "Sub-Menu Beta" ;
prog_char s_MENU_1050[] PROGMEM = "Sample Main Menu Action II" ;

prog_char s_MENU_2000[] PROGMEM = "Sub-Menu Alpha" ;   
prog_char s_MENU_2010[] PROGMEM = "Change A Setting" ;   
prog_char s_MENU_2020[] PROGMEM = "Enter a String" ;

prog_char s_MENU_3000[] PROGMEM = "Sub-Menu Beta" ;   
prog_char s_MENU_3010[] PROGMEM = "Enter a Sample Value" ;   
prog_char s_MENU_3030[] PROGMEM = "Inactive Item" ;

prog_char s_MENU_PROMPT_1000[] PROGMEM = "Select from choices:" ;

prog_char s_RESPONSE_PROMPT_1000[] PROGMEM = "New Value [" ;

prog_char s_MESSAGE_1000[] PROGMEM = " is invalid.  Digits 0-9 only";


PROGMEM const char *WManagement_string_table[] = 	   
{   
  s_MENU_0000,
  s_MENU_1000,
  s_MENU_1010,
  s_MENU_1020,
  s_MENU_1030,
  s_MENU_1040,
  s_MENU_1050,
  s_MENU_2000,  
  s_MENU_2010,
  s_MENU_2020,
  s_MENU_3000, 
  s_MENU_3010,
  s_MENU_3030,
  
  s_MENU_PROMPT_1000,  
  
  s_RESPONSE_PROMPT_1000, 

  s_MESSAGE_1000,   
};

// ID's for menus
#define MENU_MAIN      1
#define MENU_ALPHA    2
#define MENU_BETA    4

// Properties of Menu array
#define MENU_ID            0
#define MENU_STRING_IDX    1
#define MENU_ACTION        2

// size of the menu control array
#define N_MENU_RECORDS  18
#define N_MENU_ELEMENTS 3
// Menu Properties
// Column  Purpose
//  0      Menu index
//  1      Menu String index
//  2      ActionID


// ID's for menu actions.
// These are just ID's (meaningless numbers)
// Keep each unique.

#define   MENU_ACTION_NONE                0
#define   MENU_ACTION_EXIT                1
#define   MENU_ACTION_MEMORY_REPORT       2
#define   MENU_ACTION_SAMPLE_I            3
#define   MENU_ACTION_SUBMENU_ALPHA       4
#define   MENU_ACTION_SUBMENU_BETA        5
#define   MENU_ACTION_SAMPLE_II           6
#define   MENU_ACTION_CHG_SETTING         7
#define   MENU_ACTION_STRING_ENTRY        8
#define   MENU_ACTION_SAMPLE_VALUE        9
#define   MENU_ACTION_INACTIVE            10
#define   MENU_ACTION_EXIT_ALPHA_MENU     11
#define   MENU_ACTION_EXIT_BETA_MENU      12

/*
Menu Properties - Control array for menu display and actions.

Three properties:
Which Menu      - ID for the current menu being displayed of a multi-level menu
What to display - StringID of Flash-stored string to display for a menu item
What to do      - ID for the action to take if this menu is selected.


*/
PROGMEM const byte MenuProperties[N_MENU_RECORDS][N_MENU_ELEMENTS] = 
{ 
  {MENU_MAIN, STI_MENU_1000    , MENU_ACTION_NONE },
  {MENU_MAIN, STI_MENU_0000,     MENU_ACTION_EXIT },
  {MENU_MAIN, STI_MENU_1000 + 1, MENU_ACTION_MEMORY_REPORT },
  {MENU_MAIN, STI_MENU_1000 + 2, MENU_ACTION_SAMPLE_I },
  {MENU_MAIN, STI_MENU_1000 + 3, MENU_ACTION_SUBMENU_ALPHA },
  {MENU_MAIN, STI_MENU_1000 + 4, MENU_ACTION_SUBMENU_BETA },
  {MENU_MAIN, STI_MENU_1000 + 5, MENU_ACTION_SAMPLE_II },
  {MENU_ALPHA, STI_MENU_2000,   MENU_ACTION_NONE },
  {MENU_ALPHA, STI_MENU_0000,   MENU_ACTION_EXIT_ALPHA_MENU },
  {MENU_ALPHA, STI_MENU_2000 + 1, MENU_ACTION_CHG_SETTING },
  {MENU_ALPHA, STI_MENU_2000 + 2, MENU_ACTION_STRING_ENTRY },
  {MENU_ALPHA, STI_MENU_1000 + 1, MENU_ACTION_MEMORY_REPORT },
  {MENU_BETA, STI_MENU_3000  ,   MENU_ACTION_NONE },  
  {MENU_BETA, STI_MENU_0000,     MENU_ACTION_EXIT_BETA_MENU },
  {MENU_BETA, STI_MENU_3000 + 1,   MENU_ACTION_SAMPLE_VALUE },  
  {MENU_BETA, STI_MENU_3000 + 2,   MENU_ACTION_INACTIVE },  
  {MENU_BETA, STI_MENU_1000 + 1,   MENU_ACTION_MEMORY_REPORT }  
  
};



Menu::Menu( ) {
  
  iCurrentMenu = MENU_MAIN;
}

// Destructor
Menu::~Menu() {

}

void Menu::begin() {

};


// GetBuffer_P
// Returns pointer to a supplied Buffer, from PROGMEM based on StringIndex provided.
// based on example from http://arduino.cc/en/Reference/PROGMEM

char* Menu::GetBuffer_P(const int StringIndex, char* pBuffer, int bufSize) {

  strncpy_P(pBuffer, (char*)pgm_read_word(&(WManagement_string_table[StringIndex])), bufSize);

  return pBuffer; 

}
// GetMenuProperties
// Returns pointer to a supplied destination byte array, from PROGMEM based on iMenu index provided.
// based on example from http://arduino.cc/en/Reference/PROGMEM
// Bytes copied is establieshed above with N_MENU_ELEMENTS 
//
// Parameters:
// iSetting      zero-based setting to retrieve
// paSetting     destination byte array to copy setting into.

byte* Menu::GetMenuProperties(const int iMenu, byte* paMenu) {
 
    memcpy_P(paMenu, &(MenuProperties[iMenu]),  N_MENU_ELEMENTS );
//  strncpy_P(pBuffer, (char*)pgm_read_byte(&(SettingsProperties[StringIndex])), bufSize);
  
  return paMenu; 

}

// GenerateMenu
// Forms a menu for display on terminal or web at CurrentMenu level.
//
//  Menu format:
//  Menu Title
//  0. Return to prior menu
//  1. Menu Item 1.
//
// Parameters:
// pMenuBuffer      Buffer for menu
// iBufSize         size of buffer
// bTerminal        true = terminal, false = html
//
// Returns          true on success, false on failure
boolean Menu::GenerateMenu(char* pMenuBuffer, int iBufSize, boolean bTerminal) {
 
  int iMenuCounter = 0;
  byte aCurrentMenuProperties[N_MENU_ELEMENTS];
  int iMenuRecord = 0;
  boolean bDone = false;
  boolean bCollecting = false;
  memset (pMenuBuffer, '\0', iBufSize);
  
  while ( (iMenuRecord < N_MENU_RECORDS) && (bDone == false) ) {
    GetMenuProperties( iMenuRecord, aCurrentMenuProperties );
    
    // gone past last entry for desired menu?
    if ( bCollecting && (aCurrentMenuProperties[MENU_ID] != iCurrentMenu) ) {
      bDone = true;
    }
    if (  aCurrentMenuProperties[MENU_ID] == iCurrentMenu ) {
      
      // Get menu text
      GetBuffer_P( aCurrentMenuProperties[MENU_STRING_IDX], gBuffer, ENTRY_FIELD_SIZE);
      
      // check for Menu Title
      if ( aCurrentMenuProperties[MENU_ACTION] == MENU_ACTION_NONE ) {
        strcat ( pMenuBuffer, "   ");
      } else {
      // if web add html etc
        itoa( iMenuCounter, strchr(pMenuBuffer, '\0'), 10);
        strcat (pMenuBuffer, ".\t");
        paMenuActions[iMenuCounter] = aCurrentMenuProperties[MENU_ACTION];
        iMenuCounter++;
      }        
      // add text
      strcat ( pMenuBuffer, gBuffer );
      strcat ( pMenuBuffer, "\r\n");
      bCollecting = true;
      
      
    }  // if a menu we want
    iMenuRecord++;  
  } // while
 
 return true; 
}


// GetMenuResponse
// Prompt for menu response: a number less than maxOptions.
// waits for single character, checks for validity.
char Menu::GetMenuResponse(  char* Prompt, const byte maxOptions) {


  boolean bDone = false;
  char Response;
  char inChar[6];

  // Show prompt
  Serial.print(Prompt);
  memset(inChar,'\0',6);

  // read a single character line
  while (!bDone) {
    if (Serial.available()) {      
      inChar[0] = (char)Serial.read();
      Serial.print(inChar);
      // Accept only digits at this time
      if (inChar[0] <= (char) ( maxOptions + (byte) '0') && inChar[0] >='0' ) {
        Response = inChar[0];
        bDone = true;
      } 
      else {
        //beep       
        Serial.println(GetBuffer_P(STI_MESSAGE_1000, gBuffer, ENTRY_FIELD_SIZE));
      }

    } 
    else {
      delay (1000);
    }
  } // if not done
  Serial.println();
  // clear out anything else
  Serial.flush();
  return Response;
}


// GetPromptResponse
// Prompt for  response: any string of less than ~64 characters.
// waits for single character, checks for validity.
char* Menu::GetPromptResponse(  char* Prompt, char* buf, int bufsize) {

  boolean bDone = false;
  char inChar;
  int i=0;
  memset(buf, '\0', bufsize);

  // Clean out any leftover characters then show prompt
  Serial.flush();
  Serial.print(Prompt);

  // read a character line terminated by a CR
  while (!bDone) {
    if (Serial.available()) {      
      inChar = (char)Serial.read();
      // Accept up to cr/nl digits at this time
      if (inChar == '\n' || inChar =='\r' || i == ENTRY_FIELD_SIZE-1 ) {
        bDone = true;
        buf[i]='\0';
      } 
      else {
        buf[i]=inChar;
        i++;
      }

    } 
  } // if not done
  Serial.println(buf);

  return  buf;
}



// PerformAction
// Executes action sequence for given iActionID
// Add menu actions into this 'big switch'.
// 
// 
//
// Parameters:
// iActionID      ActionID as listed above
//
// SIDE EFFECTS: 
// Globals affected
// iCurrentMenu       Current menu level
// 
// Returns:
// true on success, false on failures.
boolean Menu::PerformAction( int iActionID) {
  boolean bOk = false;
  
  switch ( iActionID ) {
    case MENU_ACTION_NONE:
    // do nothing
    break;
    
    case MENU_ACTION_EXIT:
    // return to prior menu
    iCurrentMenu--;
    break;
    
    case MENU_ACTION_MEMORY_REPORT:
    // Show available system memory
   
      Serial << "Memory Available: " << freeMemory() << endl;
      break;
      
    case MENU_ACTION_SUBMENU_ALPHA:
      // Alpha sub-menu
      iCurrentMenu = MENU_ALPHA;
      break;

    case MENU_ACTION_SUBMENU_BETA:
      // Beta sub-menu
      iCurrentMenu = MENU_BETA;
      break;

    case MENU_ACTION_SAMPLE_I:
      // Sample action I
      Serial << "Sample Menu Action I" << endl;
      break;

    case MENU_ACTION_SAMPLE_II:
      // Sample action I
      Serial << "Presenting Sample Menu Action II" << endl;
      break;

     case MENU_ACTION_EXIT_ALPHA_MENU:
      // Return to main menu... or another.
      iCurrentMenu = MENU_MAIN;
      break;
      
    case MENU_ACTION_EXIT_BETA_MENU:
      // Return to main menu, or some other menu.
      iCurrentMenu = MENU_MAIN;
      break;
      
      
     case MENU_ACTION_CHG_SETTING:
      // Change a setting.
      GetBuffer_P(STI_MENU_RESPONSE_PROMPT_1000, gPrompt, ENTRY_FIELD_SIZE);
      strcat( gPrompt, mySetting);
      strcat( gPrompt, "]");
      GetPromptResponse(  gPrompt, gBuffer, ENTRY_FIELD_SIZE);
      if ( strlen(gBuffer) > 0 ) {
        strncpy( mySetting, gBuffer, ENTRY_FIELD_SIZE);
      }
      break;
      
     case MENU_ACTION_STRING_ENTRY:
      // Change a setting.
      strcpy( gPrompt, "Enter a string:");
      GetPromptResponse(  gPrompt, gBuffer, ENTRY_FIELD_SIZE);
      
      Serial << "Thanks for the string " << gBuffer << endl;

      break;
      
      
     

   default:
     Serial << "???" << endl;
    break; 
  }
  
   
  return bOk;
  
}




// TerminalMenuManager
// Control routine for operating the menu.
// 
// Parameters: None
// Generate menu
// output menu
// prompt for valid selection
// do action of selection
// until done.
int Menu::TerminalMenuManager() {
  
  char* pMenuBuffer = NULL;
  boolean bDone = false;
  int iMenuResponse, iActionID;
  while (!bDone) {

    pMenuBuffer = (char*) malloc(150);
    GenerateMenu(pMenuBuffer, 150,true);
    Serial << endl << pMenuBuffer;
    free(pMenuBuffer);
    
    gBuffer[0] = GetMenuResponse( GetBuffer_P(STI_MENU_PROMPT_1000, gBuffer, ENTRY_FIELD_SIZE) ,9); 
    gBuffer[1] = '\0';
    iMenuResponse = atoi(gBuffer);
    iActionID = paMenuActions[iMenuResponse];
    
    PerformAction(iActionID);
    if (iCurrentMenu < MENU_MAIN ) {
      bDone = true;
    }
    
    
  }
}



