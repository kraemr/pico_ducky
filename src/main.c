/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include "bsp/board.h"
#include "tusb.h"
#include "../inc/tusb_config.h"
#include "usb_descriptors.h"
#include "pico/stdlib.h"
#include "spi-fatfs.h"
#include "../inc/usb_cdc.h"

extern void execute_usb_cmd_payload(UsbCommand* cmd);
extern bool get_bootsel_button(void);
#define KEYPRESS_DELAY_MS 50

uint8_t send_hid_keyboard_report(uint8_t keycode[6],uint8_t key_mod);
uint8_t send_hid_mouse_report(uint8_t deltaX,uint8_t deltaY, uint8_t buttons);
extern int get_commands(void);

int32_t run_cmds(uint32_t* prev, uint32_t now, int32_t i) {
    uint32_t should_reset_keys = 0;
    uint16_t reset_key = 0;
    uint32_t can_send_report = now >= ((*prev) + KEYPRESS_DELAY_MS);
    if(i < cmds_len && can_send_report) {
      execute_usb_cmd_payload(&cmds[i]);
      i++;      
    }else{
      return i;
    }
    (*prev) = board_millis();
    return i;
}

volatile void main_loop() {
    size_t i = 0;  
    uint32_t prev = board_millis();
    uint32_t needs_release = 0;
    uint32_t boot_wait = board_millis();
    
    while (board_millis() - boot_wait < 2000) {
        tud_task();
    }          

    while(1) { 
      uint32_t now = board_millis();
      i = run_cmds(&prev, now, i);      
      tud_task(); 
    } 
}

int main(void) {
    board_init();      
    int32_t res = 0;
    

    #ifdef BOARD_CONFIRMATION_NEEDED
        sleep_ms(100); 
        while(1) {
            if(!get_bootsel_button()){
                break;
            }
            sleep_ms(1);
        }
    #endif

    while(res != 1){
      res = get_commands();
    }
    tusb_init();
    main_loop();
    return 0;
}
