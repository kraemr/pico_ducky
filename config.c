// In this file you configure your Payload 
// In Main Payload you write each line of your exploit or whatever you use it for
//  Example: PRESS A , Presses Shift + a Once
//  Example: HOLD A, Holds a until RELEASE A
//  EXAMPLE:  
#define BOARD_BUTTON_CONFIRMATION_NEEDED 1
const int MAIN_PAYLOAD_LEN=7;
const char* MAIN_PAYLOAD[7]={
    "P 9",
    "P 0",
    "P 1",
    "P 2",
    "P 3",
    "P 4",
    "P 5"

};