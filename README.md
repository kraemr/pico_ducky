# pico_ducky

This project uses pico-sdk and tinyusb as submodules to act as a badusb device.
Be sure to use the following to clone:

```
git clone --recurse-submodules https://github.com/kraemr/pico_ducky
```

Or clone with SSH important part is that you add --recurse-submodules

# How to Build

use build.sh in the pico_ducky folder.
You need to have cmake and make installed as well as an arm cross-compiler.

This will generate a .uf2 file just like any other c pico project.
Just drag and drop the .uf2 into your mounted pi pico.

# How To Use

In config.c
You will have to edit MAIN_PAYLOAD_LEN to reflect the amout of keypresses you send.
In MAIN_PAYLOAD You put your keycodes you want pressed.

You can look them up online and for ease of use define them as for example DUCKY_KEY_A=0x4 or something like that.
For us keyboards tinyusb already has a bunch of defines you can use.

BOARD_BUTTON_CONFIRMATION_NEEDED=1 causes the program to NOT send ANY keypresses until the user has pressed the button on the pico board.

BOARD_BUTTON_CONFIRMATION_NEEDED=0 will cause the program to execute keypresses as soon as the pico powers on.

REPEAT_DUCKY_SCRIPT=1 will loop through your keypresses

REPEAT_DUCKY_SCRIPT=0 run keypresses once

## Single Keypress

In MAIN_PAYLOAD You Would send for example "a" ONE time like this

```c
// config.c
#define MAIN_PAYLOAD_LEN=1
const uint8_t BOARD_BUTTON_CONFIRMATION_NEEDED = 1;
const uint8_t REPEAT_DUCKY_SCRIPT = 0;
const uint8_t MAIN_PAYLOAD[MAIN_PAYLOAD_LEN_DEF][8]={
{REPORT_ID_KEYBOARD,NO_MODIFIER,DUCK_KEY_A,0,0,0,0,0},
};
```

## Multiple Keys pressed at once

In MAIN_PAYLOAD You Would send for example "a" ONE time like this

```c
// config.c
#define MAIN_PAYLOAD_LEN=1
const uint8_t BOARD_BUTTON_CONFIRMATION_NEEDED = 1;
const uint8_t REPEAT_DUCKY_SCRIPT = 0;
const uint8_t MAIN_PAYLOAD[MAIN_PAYLOAD_LEN_DEF][8]={
{REPORT_ID_KEYBOARD,NO_MODIFIER,DUCK_KEY_A,DUCK_KEY_B,0,0,0,0},
};
```

## Single Keypress With Key Modifier

The following Payload will press "A".
Basically you can OR a Modifier with the keycode.
so keycode for a 0x4 OR'd with L_SHIFT will produce "A"

```c
// config.c
#define MAIN_PAYLOAD_LEN=1
const uint8_t BOARD_BUTTON_CONFIRMATION_NEEDED = 1;
const uint8_t REPEAT_DUCKY_SCRIPT = 0;
#define L_CTRL    0b00000001
#define L_SHIFT   0b00000010
#define L_ALT     0b00000100
#define L_SUPER   0b00001000
#define R_CTRL    0b00010000
#define R_SHIFT   0b00100000
#define R_ALT     0b01000000
#define R_SUPER   0b10000000
const uint8_t MAIN_PAYLOAD[MAIN_PAYLOAD_LEN_DEF][8]={
{REPORT_ID_KEYBOARD,L_SHIFT,DUCK_KEY_A,0,0,0,0,0},
};
```

## Single Keypress With Multiple Modifiers

```c
// config.c
#define MAIN_PAYLOAD_LEN=1
const uint8_t BOARD_BUTTON_CONFIRMATION_NEEDED = 1;
const uint8_t REPEAT_DUCKY_SCRIPT = 0;
#define L_CTRL    0b00000001
#define L_SHIFT   0b00000010
#define L_ALT     0b00000100
#define L_SUPER   0b00001000
#define R_CTRL    0b00010000
#define R_SHIFT   0b00100000
#define R_ALT     0b01000000
#define R_SUPER   0b10000000
const uint8_t MAIN_PAYLOAD[MAIN_PAYLOAD_LEN_DEF][8]={
{REPORT_ID_KEYBOARD, L_SHIFT | L_ALT | R_SUPER, DUCK_KEY_A,0,0,0,0,0},
};

```

## Repeated Keypresses

The following Payload will press "A" repeatedly

```c
#define MAIN_PAYLOAD_LEN=1
const uint8_t BOARD_BUTTON_CONFIRMATION_NEEDED = 1;
const uint8_t REPEAT_DUCKY_SCRIPT = 1;
#define L_CTRL    0b00000001
#define L_SHIFT   0b00000010
#define L_ALT     0b00000100
#define L_SUPER   0b00001000
#define R_CTRL    0b00010000
#define R_SHIFT   0b00100000
#define R_ALT     0b01000000
#define R_SUPER   0b10000000
const uint8_t MAIN_PAYLOAD[MAIN_PAYLOAD_LEN_DEF][8]={
{REPORT_ID_KEYBOARD,L_SHIFT,DUCK_KEY_A,0,0,0,0,0},
};
```
