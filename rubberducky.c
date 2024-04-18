#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bsp/board.h"
#include "tusb.h"
#include "tusb_config.h"
#include "usb_descriptors.h"

extern const char* MAIN_PAYLOAD[];
extern const int MAIN_PAYLOAD_LEN;
extern uint32_t keypress_delay_ms;
extern uint8_t send_hid_keyboard_report(uint8_t keycode[6],uint8_t key_mod);
// executes lists of keypresses / commands
static int current_line=0;
// KEYBOARD STATE here
#define NONE 0
#define PRESS_KEY 1
#define HOLD_KEY 2
#define RELEASE_KEY 3

const char* SPACE_KEY_COMMAND = "SPACE";
const char* ENTER_KEY_COMMAND = "ENTER";
const char* SUPER_MODIFIER_COMMAND = "SUPER";
const char* SHIFT_MODIFIER_COMMNAD = "SHIFT";
const uint8_t NUMBER_KEY_OFFSET = 19;
const uint8_t LOWERCASE_CHARACTER_KEY_OFFSET = 93;

/*the 9th bit of the Return Value indicates if it is a keymodifier or a Special Key*/
uint16_t parse_special_key(const char* command_str,int* pos,int len){
    char chs[256];
    int chs_i = 0;

    while(command_str[*pos] != ' ' && command_str[*pos] != '\0' ){
        chs[chs_i] = command_str[*pos];
        (*pos) += 1;
    }

    if( strcmp(chs,SPACE_KEY_COMMAND) == 0){
        return HID_KEY_SPACE;
    }

    if(strcmp(chs, ENTER_KEY_COMMAND) == 0){
        return HID_KEY_ENTER;
    }

    if(strcmp(chs,SUPER_MODIFIER_COMMAND)){
        return HID_KEY_0;
    }
}


uint8_t parse_key(const char ascii_code){
    // In ascii 0 comes before 1
    // In USB key codes it comes after 9
    // So handle this special case first
    if(ascii_code == 48){
        return HID_KEY_0;
    }
    if(ascii_code < 58){
        return ascii_code - NUMBER_KEY_OFFSET;
    }
    if(ascii_code > 96){
        return(ascii_code - LOWERCASE_CHARACTER_KEY_OFFSET);
    }
    return 0;
    }


void execute_ducky_payload(){
    uint8_t keys[6]={0,0,0,0,0,0};
    if(current_line >= MAIN_PAYLOAD_LEN){
        current_line = 0;
        //return;
    }

    const char * command_str = MAIN_PAYLOAD[current_line];
    int len = strlen(command_str);
    uint8_t key_action=NONE;
    uint8_t keys_i = 0;
    uint8_t keymod=0;
    uint8_t is_special_key=0;
    

    if( (command_str[0] == 'P' || command_str[0] == 'p')){
            key_action = PRESS_KEY;
    }

    for(int i = 1; i < len ; i++){
        if(command_str[i] != ' ' ){
            keys[keys_i] = parse_key(command_str[i]);
            keys_i++;
        }
        else{
            continue;
        }
    }
    board_led_write(1);
    uint8_t pressed = send_hid_keyboard_report(keys,0);
    if(pressed == 0){
        return;
    }
    current_line++;
}