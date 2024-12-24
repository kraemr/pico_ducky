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

#include "bsp/board.h"
#include "tusb.h"
#include "tusb_config.h"
#include "usb_descriptors.h"
#include "pico/stdlib.h"
void led_blinking_task(void);
void hid_task(void);
extern void bruteforce_task();
extern int32_t execute_ducky_payload();
extern const uint16_t KEYPRESS_DELAY_MS;
uint8_t send_hid_keyboard_report(uint8_t keycode[6],uint8_t key_mod);
uint8_t send_hid_mouse_report(uint8_t deltaX,uint8_t deltaY, uint8_t buttons);

static void main_loop(void){
  uint32_t now=0;
  uint32_t prev=0;
  uint32_t should_reset_keys = 0;
  uint8_t res = 0;
  uint8_t t = 0;
  uint8_t k[6]={0x04,0,0,0,0,0};
  uint8_t report_id = REPORT_ID_KEYBOARD;
  prev = board_millis();
  now = board_millis();

  while(!t){
    now = board_millis();
    if(now >= (prev + KEYPRESS_DELAY_MS) && should_reset_keys == 0){
      res = send_hid_keyboard_report(k,0);
      prev = board_millis();
      should_reset_keys = 1;
    }
    else if(now >= (prev + KEYPRESS_DELAY_MS) && should_reset_keys == 1){
      k[0] = 0x0;
      send_hid_keyboard_report(k,0);
      prev = board_millis();
      if(res){
        t = 1;
      }
      should_reset_keys = 0;
    }    
    tud_task(); // tinyusb device task
  }

  now = 0;
  prev = 0;
  int32_t ducky_res = 0;
  uint16_t reset_key = 0;
  while (1)
  {

    now = board_millis();
    // i figured out why it fails to print the same character two times 
    //  sending {0x4,0x4,0,0,0} would not press the a key two times instead it presses once and ignores the other one
    // sending {0x4} and directly after {0x4} will fail, the solution is to send {0,0,0,0,0,0} everytime after a key is pressed
    /*https://wiki.osdev.org/USB_Human_Interface_Devices*/
    
    if(now >= (prev + KEYPRESS_DELAY_MS) && should_reset_keys == 0) {
      ducky_res = execute_ducky_payload();
      prev = board_millis();
      should_reset_keys = (ducky_res == REPORT_ID_KEYBOARD) ^ 
                          ((ducky_res == REPORT_ID_MOUSE) << 2) ^ 
                          ((ducky_res == REPORT_ID_CONSUMER_CONTROL) << 3);
 
    }
    else if(now >= (prev + KEYPRESS_DELAY_MS) && should_reset_keys == 1 ) {
      send_hid_keyboard_report((uint8_t[]){0,0,0,0,0,0},0);
      prev = board_millis();
      should_reset_keys = 0;
    }
    else if(now >= (prev + KEYPRESS_DELAY_MS) && should_reset_keys == 4) {
      send_hid_mouse_report(0, 0, 0);
      prev = board_millis();
      should_reset_keys = 0;
    }
    else if(now >= (prev + KEYPRESS_DELAY_MS) && should_reset_keys == 8) {
      tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &reset_key, 2);
      prev = board_millis();
      board_led_write(1);
      should_reset_keys = 0;
    }
    tud_task(); // tinyusb device task
  }
}

/*------------- MAIN -------------*/
volatile int main(void)
{
  board_init();
  tusb_init();
  // This makes sure that the "ducky" script only starts when the button is pressed
  while(board_button_read() == 0){
  }
  main_loop();
}
//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void)
{
  //blink_interval_ms = KEYPRESS_DELAY;
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
 // blink_interval_ms = BLINK_NOT_MOUNTED;
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
  (void) remote_wakeup_en;
  //blink_interval_ms = BLINK_SUSPENDED;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
  //blink_interval_ms = KEYPRESS_DELAY;
}


uint8_t send_hid_mouse_report(uint8_t deltaX,uint8_t deltaY, uint8_t buttons) {
  if ( !tud_hid_ready() ) return 0;
  tud_hid_mouse_report(REPORT_ID_MOUSE,buttons,deltaX,deltaY,0,0);
  return 1;
}

uint8_t send_hid_consumer_control_report(uint16_t key) {
  if ( !tud_hid_ready() ) return 0;
  tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &key, 2);
  return 1;
}

uint8_t send_hid_keyboard_report(uint8_t keycode[6],uint8_t key_mod)
{
  // skip if hid is not ready yet
  if ( !tud_hid_ready() ) return 0;
  tud_hid_keyboard_report(REPORT_ID_KEYBOARD, key_mod, keycode);
  return 1;

/*
    case REPORT_ID_MOUSE:
    {
      int8_t const delta = 5;

      // no button, right + down, no scroll, no pan
      tud_hid_mouse_report(REPORT_ID_MOUSE, 0x00, delta, delta, 0, 0);
    }
    break;

    case REPORT_ID_CONSUMER_CONTROL:
    {
      // use to avoid send multiple consecutive zero report
      static bool has_consumer_key = false;

      if ( btn )
      {
        // volume down
        uint16_t volume_down = HID_USAGE_CONSUMER_VOLUME_DECREMENT;
        tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &volume_down, 2);
        has_consumer_key = true;
      }else
      {
        // send empty key report (release key) if previously has key pressed
        uint16_t empty_key = 0;
        if (has_consumer_key) tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &empty_key, 2);
        has_consumer_key = false;
      }
    }
    break;
*/
}

// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
// Note: For composite reports, report[0] is report ID
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint16_t len)
{
  (void) instance;
  (void) len;

  uint8_t next_report_id = report[0] + 1;

  if (next_report_id < REPORT_ID_COUNT)
  {
    //send_hid_report(next_report_id, board_button_read());
  }
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
  // TODO not Implemented
  (void) instance;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen;

  return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
  (void) instance;

  if (report_type == HID_REPORT_TYPE_OUTPUT)
  {
    // Set keyboard LED e.g Capslock, Numlock etc...
    if (report_id == REPORT_ID_KEYBOARD)
    {
      // bufsize should be (at least) 1
      if ( bufsize < 1 ) return;

      uint8_t const kbd_leds = buffer[0];

      if (kbd_leds & KEYBOARD_LED_CAPSLOCK)
      {
        board_led_write(true);
      }else
      {
        // Caplocks Off: back to normal blink
        board_led_write(false);
      }
    }
  }
}