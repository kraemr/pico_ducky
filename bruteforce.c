//permissable chars:
// 0 = Nothing
// 0b1000000000 = Characters
// 0b0100000000 = Numbers
// 0b0010000000 = Upper/lower case
// 0b0001000000 =  Special Characters

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bsp/board.h"
#include "tusb.h"
#include "tusb_config.h"
#include "usb_descriptors.h"

extern uint32_t keypress_delay_ms;
extern void send_hid_report(uint8_t report_id, uint32_t btn, uint8_t keycode[6],uint8_t key_mod);


#define BRUTEFORCE_CHARACTERS 0b1000000000 // abcdefg ...
#define BRUTEFORCE_NUMBERS    0b0100000000 // 0987654321
#define BRUTEFORCE_UPPERLOWER 0b0010000000 // ABCDEFG ...
#define BRUTEFORCE_SPECIAL 0b0001000000 // #,.-_| ...
#define BRUTEFORCE_UMLAUTE 0b0000100000 // Bruteforce ä ö ü ....

const uint8_t characters[] = {
    HID_KEY_A, HID_KEY_B, HID_KEY_C, HID_KEY_D, HID_KEY_E, HID_KEY_F, HID_KEY_G, HID_KEY_H, HID_KEY_I, HID_KEY_J, HID_KEY_K, HID_KEY_L, HID_KEY_M,
    HID_KEY_N, HID_KEY_O, HID_KEY_P, HID_KEY_Q, HID_KEY_R, HID_KEY_S, HID_KEY_T, HID_KEY_U, HID_KEY_V, HID_KEY_W, HID_KEY_X, HID_KEY_Y, HID_KEY_Z
};

const uint8_t numbers[] = {
    HID_KEY_1, HID_KEY_2, HID_KEY_3, HID_KEY_4, HID_KEY_5, HID_KEY_6, HID_KEY_7, HID_KEY_8, HID_KEY_9, HID_KEY_0
};

int char_index=0;
// Sends one keycode
void bruteforce_chars_single(){
    uint8_t keycode [6] = {0};
    if(char_index >= 26){
        char_index = 0;
        keycode[0] = HID_KEY_ENTER;
    }
    keycode[0] = characters[char_index]; 
    send_hid_report(REPORT_ID_KEYBOARD,1,keycode,0);
    char_index+=1;
}

void bruteforce_task (){
    int exit = 0;
    static uint32_t start_ms = 0;
    if (!keypress_delay_ms) return;
    send_hid_report(REPORT_ID_KEYBOARD ,0,(uint8_t[]){0,0,0,0,0,0},0);
    if ( board_millis() - start_ms < keypress_delay_ms) return; // not enough time
    start_ms += keypress_delay_ms;
    bruteforce_chars_single();
}
