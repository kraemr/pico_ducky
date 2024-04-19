#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifndef TESTING
    #include "bsp/board.h"
    #include "tusb.h"
    #include "tusb_config.h"
    #include "tinyusb/src/class/hid/hid.h"
#endif

#include "usb_descriptors.h"


extern const char* MAIN_PAYLOAD[];
extern const int MAIN_PAYLOAD_LEN;
extern uint32_t keypress_delay_ms;
extern uint8_t send_hid_keyboard_report(uint8_t keycode[6],uint8_t key_mod);
extern const uint8_t REPEAT_DUCKY_SCRIPT;

static int current_line=0;

#define NONE 0
#define PRESS_KEY 1
#define HOLD_KEY 2
#define RELEASE_KEY 3

const char* SPACE_KEY_COMMAND = "SPACE";
const char* ENTER_KEY_COMMAND = "ENTER";

const char* LEFT_SUPER_MODIFIER_COMMAND = "LSUPER";
const char* RIGHT_SUPER_MODIFIER_COMMAND = "RSUPER";

const char* LSHIFT_MODIFIER_COMMAND = "LSHIFT";
const char* RSHIFT_MODIFIER_COMMAND = "RSHIFT";

const char* LEFT_ALT_MODIFIER_COMMAND = "LALT";
const char* RIGHT_ALT_MODIFIER_COMMAND = "RALT";

const char* LEFT_CTRL_MODIFIER_COMMAND = "LCTRL";
const char* RIGHT_CTRL_MODIFIER_COMMAND = "RCTRL";

const uint8_t NUMBER_KEY_OFFSET = 19;
const uint8_t LOWERCASE_CHARACTER_KEY_OFFSET = 93;

struct Special_key{
    uint8_t key;
    uint8_t keymod;
}Special_key;


void set_special_key(struct Special_key* sp_key_ref,const char chs[256],const char* command,const uint8_t hid_key,const uint8_t hid_key_mod,const uint8_t chs_len){
    if(strncmp(chs,command,chs_len) == 0){
        sp_key_ref->key = hid_key;
        sp_key_ref->keymod = hid_key_mod;
    }
}

struct Special_key parse_special_key(const char* command_str,int* pos,int len){
    char chs[256];
    int chs_i = 0;

    struct Special_key special_key;
    special_key.key = 0;
    special_key.keymod = 0;
    
    // get the rest of the string
    while(command_str[ (*pos) ] != ' ' && command_str[ (*pos) ] != '\0' && (*pos) <  len){
        chs[chs_i] = command_str[ (*pos) ];
        (*pos) += 1;
        chs_i += 1;
    }

    // Special Keys
    set_special_key(&special_key,chs,SPACE_KEY_COMMAND,HID_KEY_SPACE,0,chs_i);
    set_special_key(&special_key,chs,ENTER_KEY_COMMAND,HID_KEY_ENTER,0,chs_i);


    // MODIFIFERS
    set_special_key(&special_key,chs,LEFT_SUPER_MODIFIER_COMMAND,0,KEYBOARD_MODIFIER_LEFTGUI,chs_i);
    set_special_key(&special_key,chs,RIGHT_SUPER_MODIFIER_COMMAND,0,KEYBOARD_MODIFIER_RIGHTGUI,chs_i);

    set_special_key(&special_key,chs,LSHIFT_MODIFIER_COMMAND,0,KEYBOARD_MODIFIER_LEFTSHIFT,chs_i);
    set_special_key(&special_key,chs,RSHIFT_MODIFIER_COMMAND,0,KEYBOARD_MODIFIER_RIGHTSHIFT,chs_i);
    
    set_special_key(&special_key,chs,LEFT_ALT_MODIFIER_COMMAND,0,KEYBOARD_MODIFIER_LEFTALT,chs_i);
    set_special_key(&special_key,chs,RIGHT_ALT_MODIFIER_COMMAND,0,KEYBOARD_MODIFIER_RIGHTALT,chs_i);
    
    set_special_key(&special_key,chs,LEFT_CTRL_MODIFIER_COMMAND,0,KEYBOARD_MODIFIER_LEFTCTRL,chs_i);
    set_special_key(&special_key,chs,RIGHT_CTRL_MODIFIER_COMMAND,0,KEYBOARD_MODIFIER_RIGHTCTRL,chs_i);
    
    return special_key;
}

uint8_t parse_key(const char ascii_code){
    // In ascii 0 comes before 1
    // In USB key codes it comes after 9
    // So handle this special case first
    if(ascii_code == 48){
        return 0x27;
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
        if(REPEAT_DUCKY_SCRIPT == 0){
            return;
        }else{
            current_line = 0;
            return;
        }
    }

    const char * command_str = MAIN_PAYLOAD[current_line];
    int len = strlen(command_str);
    uint8_t key_action = NONE;
    uint8_t keys_i = 0; 
    uint8_t keymod = 0;
    

    if( (command_str[0] == 'P' || command_str[0] == 'p')){
            key_action = PRESS_KEY;
    }

    for(int i = 1; i < len ; i++){
        // 0-9 a-z
        if((command_str[i] >= 'a') && (command_str[i] <= 'z') || ( command_str[i] >= '0' && command_str[i] <= '9') ){
            keys[keys_i] = parse_key(command_str[i]);
            keys_i++;
        }
        // Special Keys like Numpad0-9,Enter ...
        else if(command_str[i] != ' '){
            struct Special_key sp_key = parse_special_key(command_str,&i,len);
            keymod |= sp_key.keymod;
            printf("key: %d\n", sp_key.key);
            if(sp_key.key != 0){
                keys[keys_i] = sp_key.key;
                keys_i++;
            }
        }
        else{
            continue;
        }
    }
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