const unsigned int KEYPRESS_DELAY_MS = 50;
const unsigned int DISK_BLOCK_SIZE = 512;
const char* MSC_VID = "pico_ducky";
const char* MSC_PID = "Mass Storage";
const char* MSC_REV = "1.0";

// Set to a GPIO digital input that determines if to exec commands
// Useful if you dont want to execute your script and edit it
const unsigned char GPIO_CMD_EXEC = 24;
// 0: Dont exec if GPIO_DONT_EXEC is pulled UP
// 1: Dont exec if GPIO_DONT_EXEC is pulled UP
const unsigned char GPIO_CMD_EXEC_IF_UP = 0;