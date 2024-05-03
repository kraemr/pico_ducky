#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define PARSE
// If testing is defined, then keypresses are not actually sent
// then the code can be "tested" on pc instead of the pico
#ifndef TESTING
    #include "bsp/board.h"
    #include "tusb.h"
    #include "tusb_config.h"
    #include "tinyusb/src/class/hid/hid.h"
#endif
#include "usb_descriptors.h"

extern uint16_t MAIN_PAYLOAD[];
extern const int MAIN_PAYLOAD_LEN;
extern uint32_t keypress_delay_ms;
extern uint8_t send_hid_keyboard_report(uint8_t keycode[6],uint8_t key_mod);
extern const uint8_t REPEAT_DUCKY_SCRIPT;
static int current_line=0;
#define NONE 0
#define PRESS_KEY 1
#define HOLD_KEY 2
#define RELEASE_KEY 3

// returns the keymod
uint8_t exec_keypress(uint16_t key,uint8_t keys[6]){
    uint8_t keymod;
    uint8_t actual_key;
    actual_key = key & 255;
    keymod = key >> 8;
    keys[0] = actual_key;
    return keymod;
}


void execute_ducky_payload(){
    uint8_t keys[6]={0,0,0,0,0,0};
    if(current_line >= MAIN_PAYLOAD_LEN){
        if(REPEAT_DUCKY_SCRIPT == 0){
            return;
        }else{
            current_line = 0;
            return;
        }
    }
    uint8_t key_action = NONE;
    uint8_t keys_i = 0;
    uint8_t keymod = 0;
    uint16_t mp = MAIN_PAYLOAD[current_line];
    keymod =        exec_keypress(mp,keys);

    #ifndef TESTING
        uint8_t pressed = send_hid_keyboard_report(keys,keymod);
        if(pressed == 0){
            return;
        }
    #endif

    #ifdef TESTING
        printf("current line: %d, %d %d %d %d %d %d",current_line,keys[0],keys[1],keys[2],keys[3],keys[4],keys[5]);
    #endif

    current_line++;
}
