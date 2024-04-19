// In this file you configure your Payload 
// In Main Payload you write each line of your exploit or whatever you use it for
//  Example: PRESS A , Presses Shift + a Once
//  Example: HOLD A, Holds a until RELEASE A
//  EXAMPLE:  

//########## CONFIG ##############

// if 0 then script will immediately execute else the board button has to be pressed
#include <stdint.h>
const uint8_t BOARD_BUTTON_CONFIRMATION_NEEDED = 1;

// IMPORTANT PUT THE LINE,COUNT OF YOUR PAYLOAD HERE
#define MAIN_PAYLOAD_LEN_DEF 3

// if 0 Script runs once, else it repeatedly executes it
const uint8_t REPEAT_DUCKY_SCRIPT = 1;

// Set this according to the PC that it will be attached to
// a sensible default delay would be 100ms
const uint16_t KEYPRESS_DELAY_MS = 80; 
//######## CONFIG ##################
const int MAIN_PAYLOAD_LEN=MAIN_PAYLOAD_LEN_DEF;


const char* MAIN_PAYLOAD[MAIN_PAYLOAD_LEN_DEF]={
"P neof","P etch","P ENTER"
};

