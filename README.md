# pico_ducky

This software is distributed under GPLv3. You are allowed to use this software for an open-source project with a compatible license.
The Pi pico acts as a "rubberducky" essentially acting as a Usb-Stick,Mouse,Keyboard and Stylus at the same time with the ability to script mouse movements,keyboard presses,delays etc.

The Pi pico reads script.txt in its sdcards FAT Filesystem and executes it.

NOTE:
Clone with --recurse-submodules

## Hardware Configuration (Sdcards)
You will have to adapt the hw_config.c to fit your pi pico board/spi adapter.
If your board has a built-in sdcard reader you might be lucky and the vendor might already have an example with a config you can just copy.

Otherwise you have to look at how you wired your adapter and adapt the config.
In the future there will be some examples for more common setups.

## Building
```
  1. run build.sh
  2. copy .uf2 to pico
  3. Usb-disk pops up, create a script.txt file with a valid script
  4. plug in again -> it executes the keypresses/mouse movements
```

## Temporarily Disabling Scripts
You can temporarily prevent scripts from running by specifying a GPIO in app_config.c which when it returns 1 disables scripts.
```c
...
// CHANGE THIS
const unsigned char GPIO_CMD_EXEC = 24;
```

## Scripting Commands
The language and current commands available are intentionally kept small and simple. 
There will be an extension for n-key rollover and some other improvements.

These are the currently implemented commands:
```c
// hold key 0 and 1 and release 0
hold 0,1;
release 0;

// press key 0,1,2,3,4,5
press 0,1,2,3,4,5; 

// move mouse pointer to x:100 y:101
move_abs 100,101;

// move mouse pointer +10 in x direction and +10 in y direction
move_rel 10,10;

// hold right mouse button
mouse_hold mouse_right;

// release right mouse button
mouse_release mouse_right;

// press all mouse buttons
mouse_press mouse_left,mouse_right,mouse_middle;

// Wait 1000 milliseconds
delay 1000;
```
