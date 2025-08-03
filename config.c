// In this file you configure your Payload
// In Main Payload you write each line of your exploit
//########## CONFIG ##############
#include "usb_descriptors.h"
#include "layouts/us_keyboard_layout.h"
#include "tinyusb/src/tusb.h"
#include <stdint.h>

/*
    The HID_KEYs as they are defined in tinyusb are for the US Layout
    IF the PC doesnt use US Layout, then you will have to lookup us layout and convert the keys
    For example on German Keyboard layout "EQ" in your Payload will lead to the Gravure Key being pressed instead of =
    For The US Layout a header file is pre-included at layouts/us_keyboard_layout.h
    Feel free to add your own layouts!
*/

#define NO_MODIFIER 0b00000000
#define L_CTRL      0b00000001
#define L_SHIFT     0b00000010
#define L_ALT       0b00000100
#define L_SUPER     0b00001000
#define R_CTRL      0b00010000
#define R_SHIFT     0b00100000
#define R_ALT       0b01000000
#define R_SUPER     0b10000000

#define NO_MOUSE     0
#define MOUSE_LEFT   1
#define MOUSE_RIGHT  2
#define MOUSE_MDIILE 3


// if 0 then script will immediately execute.
// Otherwise the board button has to be pressed
const uint8_t BOARD_BUTTON_CONFIRMATION_NEEDED = 1;

// if 0 Script runs once, else it repeatedly executes it
const uint8_t REPEAT_DUCKY_SCRIPT = 0;

// Set this according to the PC that it will be attached to
// a sensible default delay would be 125ms 
const uint16_t KEYPRESS_DELAY_MS = 125;

//######## CONFIG ##################

// IMPORTANT PUT THE LINECOUNT OF YOUR PAYLOAD HERE
#define MAIN_PAYLOAD_LEN_DEF 10
const int MAIN_PAYLOAD_LEN=MAIN_PAYLOAD_LEN_DEF;

/*
This is a pretty barebones approach, where what you put in here directly gets sent.
You can use these descriptors from usb_descriptors:
enum
{
  REPORT_ID_KEYBOARD = 1,
  REPORT_ID_MOUSE,
  REPORT_ID_CONSUMER_CONTROL,
  REPORT_ID_GAMEPAD,
  REPORT_ID_COUNT
};

*/
const uint8_t MAIN_PAYLOAD[MAIN_PAYLOAD_LEN_DEF][8]={
    {REPORT_ID_KEYBOARD,NO_MODIFIER,DUCK_KEY_0,0,0,0,0,0},
    {REPORT_ID_KEYBOARD,NO_MODIFIER,DUCK_KEY_1,0,0,0,0,0},
    {REPORT_ID_KEYBOARD,NO_MODIFIER,DUCK_KEY_2,0,0,0,0,0},
    {REPORT_ID_KEYBOARD,NO_MODIFIER,DUCK_KEY_3,0,0,0,0,0},
    {REPORT_ID_KEYBOARD,NO_MODIFIER,DUCK_KEY_4,0,0,0,0,0},
    {REPORT_ID_KEYBOARD,NO_MODIFIER,DUCK_KEY_5,0,0,0,0,0},
    {REPORT_ID_KEYBOARD,NO_MODIFIER,DUCK_KEY_6,0,0,0,0,0},
    {REPORT_ID_KEYBOARD,NO_MODIFIER,DUCK_KEY_7,0,0,0,0,0},
    {REPORT_ID_KEYBOARD,NO_MODIFIER,DUCK_KEY_8,0,0,0,0,0},
    {REPORT_ID_KEYBOARD,NO_MODIFIER,DUCK_KEY_9,0,0,0,0,0},
};


