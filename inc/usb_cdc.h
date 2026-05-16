#include <stdint.h>
uint8_t send_hid_mouse_report(uint8_t deltaX,uint8_t deltaY, uint8_t buttons);
uint8_t send_hid_consumer_control_report(uint16_t key);
uint8_t send_hid_keyboard_report(uint8_t keycode[6],uint8_t key_mod);
