#ifndef DEBUG
#include <bsp/board.h>
#include <stdint.h>
#include "tusb.h"
#include "../inc/tusb_config.h"
#include "usb_descriptors.h"
#include "pico/stdlib.h"
#include "spi-fatfs.h"
#include "../inc/usb_cdc.h"
#include "../inc/usb_descriptors.h"
#include "bsp/board.h"
#include "tusb.h"
#include "../inc/tusb_config.h"
#include "pico-sdk/lib/tinyusb/src/class/hid/hid.h"
#include "../usb_script/parser.h"
#define KEYPRESS_DELAY_MS 50

void execute_usb_cmd_payload(UsbCommand* cmd);
extern bool get_bootsel_button(void);
extern uint8_t send_hid_keyboard_report(uint8_t keycode[6],uint8_t key_mod);
extern uint8_t send_hid_mouse_report(uint8_t deltaX,uint8_t deltaY, uint8_t buttons);
extern uint8_t send_abs_hid_mouse_report(int16_t x, int16_t y, uint8_t buttons);
extern int get_commands(UsbState* usb_state);

int32_t run_cmds(uint32_t* prev, uint32_t now, int32_t i) {
    uint32_t delay_ms = KEYPRESS_DELAY_MS;
    uint32_t can_send_report = now >= ((*prev) + delay_ms);
    if(can_send_report) {
        execute_usb_cmd_payload(&cmds[i]);
        i++;      
        (*prev) = board_millis();
    }
    else{
        return i;
    }
    return i;
}

void main_loop(UsbState* state) {
    size_t i = 0;  
    uint32_t prev = board_millis();
    uint32_t boot_wait = board_millis();    
    while (board_millis() - boot_wait < 2000) {
        tud_task();
    }
    sleep_ms(100);
    
    /* Execute All Commands in Order*/
    while(i < cmds_len) { 
      uint32_t now = board_millis();
      i = run_cmds(&prev, now, i);
      tud_task();
    }

    while(1) {
        tud_task();
    }
}

int main(void) {
    UsbState state = {0};
    board_init();
    int32_t res = 0;
    #ifdef BOARD_CONFIRMATION_NOT_NEEDED
        sleep_ms(100); 
        while(1) {
            if(!get_bootsel_button()){
                break;
            }
            sleep_ms(1);
        }
    #endif
    while(res != 1){
      res = get_commands(&state);
    }
    tusb_init();
    main_loop(&state);
    return 0;
}
#endif

void execute_usb_cmd_payload(UsbCommand* cmd){
    if(cmd->type == KEYBOARD){
        send_hid_keyboard_report(cmd->value.keyboard_cmd.keys,cmd->value.keyboard_cmd.modifier);
    }
    else if(cmd->type == MOUSE_ABS_MOVE){
        send_abs_hid_mouse_report(cmd->value.mouse_abs_cmd.x,cmd->value.mouse_abs_cmd.y,cmd->value.mouse_abs_cmd.buttons);
    }
    else if(cmd->type == MOUSE_REL_MOVE || cmd->type == MOUSE_BUTTONS){
        send_hid_mouse_report(cmd->value.mouse_rel_cmd.delta_x,cmd->value.mouse_rel_cmd.delta_y,cmd->value.mouse_rel_cmd.buttons);
    }
    else if(cmd->type == WAIT){
        uint32_t start = board_millis();
        while (board_millis() - start < cmd->value.delay) {
            tud_task();
        }
    }
}