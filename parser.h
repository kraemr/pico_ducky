#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum USB_COMMAND {
	SET_LANG,
	SWITCH_HID_MODE, // Switches Hid mode from keyboard to controller for example
	PRESS, // Press a button and instantly release it after
	HOLD, // Hold a button
	RELEASE,// Release a button that is held
	RESTART, // When put at the end, program gets run again after finished, good for commands that need to be sent repeatedly 
}USB_COMMAND;

typedef enum PARSING_STATE {
	START,
	FOUND_COMMAND, // Press,Hold,Relase etc
	FOUND_END,
	ERROR,
}PARSING_STATE;

typedef struct UsbCommand {
	USB_COMMAND command;
	unsigned char value[8];
}UsbCommand;

typedef struct {
    const char* key;
    unsigned char val;
} KeyPair;


extern int parse_line(const char* input,unsigned short input_len,UsbCommand* cmd);