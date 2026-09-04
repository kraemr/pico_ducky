#ifndef MOCK_PICO
  #include <sys/types.h>
  #include "bsp/board.h"
  #include "tusb.h"
  #include "../inc/tusb_config.h"
  #include "../inc/usb_cdc.h"
  #include "usb_descriptors.h"
  #include "pico/stdlib.h"

  void tud_mount_cb(void){}
  void tud_umount_cb(void){}
  void tud_suspend_cb(bool remote_wakeup_en){(void) remote_wakeup_en;}
  void tud_resume_cb(void){}

  uint8_t send_hid_mouse_report(uint8_t deltaX,uint8_t deltaY, uint8_t buttons) {
    if ( !tud_hid_ready() ) return 0;
    tud_hid_mouse_report(REPORT_ID_MOUSE,buttons,deltaX,deltaY,0,0);
    return 1;
  }

  uint8_t send_abs_hid_mouse_report(int16_t x,int16_t y, uint8_t buttons) {
    if ( !tud_hid_ready() ) return 0;
    tud_hid_abs_mouse_report(REPORT_ID_STYLUS_PEN,buttons,x,y,0,0);
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
  }
  // Invoked when sent REPORT successfully to host
  void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint16_t len)
  {
    (void) instance;
    (void) len;
  }

  // Invoked when received GET_REPORT control request
  uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
  {
    (void) instance;
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) reqlen;
    return 0;
  }

  // Invoked when received SET_REPORT control request or
  void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
  {
    (void) instance;
  }
#endif