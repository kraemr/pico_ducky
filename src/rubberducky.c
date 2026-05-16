#include <stdint.h>
#include "../inc/usb_descriptors.h"
#include "bsp/board.h"
#include "tusb.h"
#include "../inc/tusb_config.h"
#include "pico-sdk/lib/tinyusb/src/class/hid/hid.h"
#include "../usb_script/parser.h"

extern uint8_t send_hid_keyboard_report(volatile uint8_t keycode[6],uint8_t key_mod);
#define NONE 0
//extern uint8_t send_hid_mouse_report(uint8_t deltaX,uint8_t deltaY, uint8_t buttons);
//extern uint8_t send_hid_consumer_control_report(uint16_t key);
//extern const uint8_t REPEAT_DUCKY_SCRIPT;
static int current_line=0;


void execute_usb_cmd_payload(UsbCommand* cmd){
    uint8_t key_action = NONE;
    uint8_t keys_i = 0;
    uint8_t keymod = 0;
    uint8_t report_type = cmd->value[0];
    keymod = cmd->value[1];

    if(report_type == REPORT_ID_KEYBOARD){
        uint8_t pressed = send_hid_keyboard_report(&cmd->value[2],keymod);
    }

    current_line++;
}