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


#ifdef PARSE
    extern const char* MAIN_PAYLOAD[];
#endif

#ifdef NO_PARSE
    extern uint16_t MAIN_PAYLOAD[];
#endif

extern const int MAIN_PAYLOAD_LEN;
extern uint32_t keypress_delay_ms;
extern uint8_t send_hid_keyboard_report(uint8_t keycode[6],uint8_t key_mod);
extern const uint8_t REPEAT_DUCKY_SCRIPT;
static int current_line=0;
#define NONE 0
#define PRESS_KEY 1
#define HOLD_KEY 2
#define RELEASE_KEY 3
// ####### KEYS #######

const char* SPACE_KEY_COMMAND = "SPACE";
const char* ENTER_KEY_COMMAND = "ENTER";
const char* ESCAPE_KEY_COMMAND = "ESC";
const char* TAB_KEY_COMMAND = "TAB";
const char* MINUS_KEY_COMMAND = "MINUS";
const char* EQUAL_KEY_COMMAND = "EQ";
const char* LEFT_BRACKET_COMMAND = "LBRACKET";
const char* RIGHT_BRACKET_COMMAND = "RBRACKET";
const char* BACKSLASH_COMMAND = "BSLASH";
const char* COMMA_COMMAND = "COMMA";
const char* PERIOD_COMMAND = "PERIOD";
const char* SLASH_COMMAND = "SLASH";
const char* CAPSLOCK_COMMAND = "CAPSLOCK";

const char* F1_COMMAND = "F1";
const char* F2_COMMAND = "F2";
const char* F3_COMMAND = "F3";
const char* F4_COMMAND = "F4";
const char* F5_COMMAND = "F5";
const char* F6_COMMAND = "F6";
const char* F7_COMMAND = "F7";
const char* F8_COMMAND = "F8";
const char* F9_COMMAND = "F9";
const char* F10_COMMAND = "F10";
const char* F11_COMMAND = "F11";
const char* F12_COMMAND = "F12";
const char* F13_COMMAND = "F13";
const char* F14_COMMAND = "F14";
const char* F15_COMMAND = "F15";
const char* F16_COMMAND = "F16";
const char* F17_COMMAND = "F17";
const char* F18_COMMAND = "F18";
const char* F19_COMMAND = "F19";
const char* F20_COMMAND = "F20";
const char* F21_COMMAND = "F21";
const char* F22_COMMAND = "F22";
const char* F23_COMMAND = "F23";
const char* F24_COMMAND = "F24";

const char* PRINT_SCREEN_COMMAND = "PRINT_SCREEN";
const char* SCROLL_LOCK_COMMAND = "SCROLL_LOCK";
const char* PAUSE_COMMAND = "PAUSE";
const char* INSERT_COMMAND = "INSERT";
const char* HOME_COMMAND = "HOME";

const char* PAGEUP_COMMAND = "PGUP";
const char* PAGEDOWN_COMMAND = "PGDN";
const char* DELETE_COMMAND = "DEL";
const char* END_COMMAND = "END";

const char* RIGHT_ARROW_COMMAND = "RIGHT";
const char* LEFT_ARROW_COMMAND = "LEFT";
const char* UP_ARROW_COMMAND = "UP";
const char* DOWN_ARROW_COMMAND = "DOWN";
// ####### KEYS #######

// ####### MODIFIERS #######
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


const char* LANG1_COMMAND = "LANG1";
const char* LANG2_COMMAND = "LANG2";
const char* LANG3_COMMAND = "LANG3";
const char* LANG4_COMMAND = "LANG4";
const char* LANG5_COMMAND = "LANG5";
const char* LANG6_COMMAND = "LANG6";
const char* LANG7_COMMAND = "LANG7";
const char* LANG8_COMMAND = "LANG8";
const char* LANG9_COMMAND = "LANG9";

const char* GRAVE_COMMAND = "GRAVE";
const char* SEMICOLON_COMMAND = "SEMICOLON";
const char* APOSTROPHE_COMMAND = "APOSTROPHE";

const char* KEYPAD_1_COMMAND = "KEYPAD1";
const char* KEYPAD_2_COMMAND = "KEYPAD2";
const char* KEYPAD_3_COMMAND = "KEYPAD3";
const char* KEYPAD_4_COMMAND = "KEYPAD4";
const char* KEYPAD_5_COMMAND = "KEYPAD5";
const char* KEYPAD_6_COMMAND = "KEYPAD6";
const char* KEYPAD_7_COMMAND = "KEYPAD7";
const char* KEYPAD_8_COMMAND = "KEYPAD8";
const char* KEYPAD_9_COMMAND = "KEYPAD9";
// ####### MODIFIERS #######

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

    // ##### Special Keys #####

    set_special_key(&special_key,chs,SPACE_KEY_COMMAND,HID_KEY_SPACE,0,chs_i);
    set_special_key(&special_key,chs,ENTER_KEY_COMMAND,HID_KEY_ENTER,0,chs_i);
    set_special_key(&special_key,chs,ESCAPE_KEY_COMMAND,HID_KEY_ESCAPE,0,chs_i);
    set_special_key(&special_key,chs,TAB_KEY_COMMAND,HID_KEY_TAB,0,chs_i);
    set_special_key(&special_key,chs,MINUS_KEY_COMMAND,HID_KEY_MINUS,0,chs_i);
    set_special_key(&special_key,chs,EQUAL_KEY_COMMAND,HID_KEY_EQUAL,0,chs_i);
    set_special_key(&special_key,chs,LEFT_BRACKET_COMMAND,HID_KEY_BRACKET_LEFT,0,chs_i);
    set_special_key(&special_key,chs,RIGHT_BRACKET_COMMAND,HID_KEY_BRACKET_RIGHT,0,chs_i);
    set_special_key(&special_key,chs,BACKSLASH_COMMAND,HID_KEY_BACKSLASH,0,chs_i);
    set_special_key(&special_key,chs,SLASH_COMMAND,HID_KEY_SLASH,0,chs_i);

    set_special_key(&special_key,chs,COMMA_COMMAND,HID_KEY_COMMA,0,chs_i);
    set_special_key(&special_key,chs,PERIOD_COMMAND,HID_KEY_PERIOD,0,chs_i);
    set_special_key(&special_key,chs,CAPSLOCK_COMMAND,HID_KEY_CAPS_LOCK,0,chs_i);

    set_special_key(&special_key,chs,SEMICOLON_COMMAND,HID_KEY_SEMICOLON,0,chs_i);
    set_special_key(&special_key,chs,APOSTROPHE_COMMAND,HID_KEY_APOSTROPHE,0,chs_i);
    set_special_key(&special_key,chs,GRAVE_COMMAND,HID_KEY_GRAVE,0,chs_i);

    set_special_key(&special_key,chs,LANG1_COMMAND,HID_KEY_LANG1,0,chs_i);
    set_special_key(&special_key,chs,LANG2_COMMAND,HID_KEY_LANG2,0,chs_i);
    set_special_key(&special_key,chs,LANG3_COMMAND,HID_KEY_LANG3,0,chs_i);
    set_special_key(&special_key,chs,LANG4_COMMAND,HID_KEY_LANG4,0,chs_i);
    set_special_key(&special_key,chs,LANG5_COMMAND,HID_KEY_LANG5,0,chs_i);
    set_special_key(&special_key,chs,LANG6_COMMAND,HID_KEY_LANG6,0,chs_i);
    set_special_key(&special_key,chs,LANG7_COMMAND,HID_KEY_LANG7,0,chs_i);
    set_special_key(&special_key,chs,LANG8_COMMAND,HID_KEY_LANG8,0,chs_i);
    set_special_key(&special_key,chs,LANG9_COMMAND,HID_KEY_LANG9,0,chs_i);

    set_special_key(&special_key,chs,KEYPAD_1_COMMAND,HID_KEY_KEYPAD_1,0,chs_i);
    set_special_key(&special_key,chs,KEYPAD_2_COMMAND,HID_KEY_KEYPAD_2,0,chs_i);
    set_special_key(&special_key,chs,KEYPAD_3_COMMAND,HID_KEY_KEYPAD_3,0,chs_i);
    set_special_key(&special_key,chs,KEYPAD_4_COMMAND,HID_KEY_KEYPAD_4,0,chs_i);
    set_special_key(&special_key,chs,KEYPAD_5_COMMAND,HID_KEY_KEYPAD_5,0,chs_i);
    set_special_key(&special_key,chs,KEYPAD_6_COMMAND,HID_KEY_KEYPAD_6,0,chs_i);
    set_special_key(&special_key,chs,KEYPAD_7_COMMAND,HID_KEY_KEYPAD_7,0,chs_i);
    set_special_key(&special_key,chs,KEYPAD_8_COMMAND,HID_KEY_KEYPAD_8,0,chs_i);
    set_special_key(&special_key,chs,KEYPAD_9_COMMAND,HID_KEY_KEYPAD_9,0,chs_i);



    set_special_key(&special_key,chs,F1_COMMAND,HID_KEY_F1,0,chs_i);
    set_special_key(&special_key,chs,F2_COMMAND,HID_KEY_F2,0,chs_i);
    set_special_key(&special_key,chs,F3_COMMAND,HID_KEY_F3,0,chs_i);
    set_special_key(&special_key,chs,F4_COMMAND,HID_KEY_F4,0,chs_i);
    set_special_key(&special_key,chs,F5_COMMAND,HID_KEY_F5,0,chs_i);
    set_special_key(&special_key,chs,F6_COMMAND,HID_KEY_F6,0,chs_i);
    set_special_key(&special_key,chs,F7_COMMAND,HID_KEY_F7,0,chs_i);

    // ##### Special Keys #####
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


uint8_t parse_and_exec(const char* command_str,int len,uint8_t keys[6]){
    uint8_t key_action = NONE;
    uint8_t keys_i = 0;
    uint8_t keymod = 0;
    if( (command_str[0] == 'P' || command_str[0] == 'p')){
                key_action = PRESS_KEY;
        }
    for(int i = 1; i < len ; i++){
        if((command_str[i] >= 'a' && command_str[i] <= 'z') || ( command_str[i] >= '0' && command_str[i] <= '9') ){
            keys[keys_i] = parse_key(command_str[i]);
            keys_i++;
        }
        else if(command_str[i] != ' '){
            struct Special_key sp_key = parse_special_key(command_str,&i,len);
            keymod |= sp_key.keymod;
            if(sp_key.key != 0){
                keys[keys_i] = sp_key.key;
                keys_i++;
            }
        }
        else{
            continue;
        }
    }
    return keymod;
}

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

    #ifdef PARSE
        const char * command_str = MAIN_PAYLOAD[current_line];
        int len = strlen(command_str);
        keymod = parse_and_exec(command_str,len,keys);
    #endif

    #ifdef NO_PARSE
        uint16_t mp = MAIN_PAYLOAD[current_line];
        keymod =        exec_keypress(mp,keys);
    #endif

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
