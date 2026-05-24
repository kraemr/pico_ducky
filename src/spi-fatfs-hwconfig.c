/* hw_config.c
Copyright 2021 Carl John Kugler III

Licensed under the Apache License, Version 2.0 (the License); you may not use
this file except in compliance with the License. You may obtain a copy of the
License at

   http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an AS IS BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

* * --- MODIFICATIONS ---
 * Extracted from no-OS-FatFS-SD-SPI-RPi-Pico and modified by kraemr in 2026.
 * Changes: 
 * Adapted Config to be configurable with Defines
 * Also simplified setup to one sd card slot
*/
/*

This file should be tailored to match the hardware design.

There should be one element of the spi[] array for each hardware SPI used.

There should be one element of the sd_cards[] array for each SD card slot.
The name is should correspond to the FatFs "logical drive" identifier.
(See http://elm-chan.org/fsw/ff/doc/filename.html#vol)
In general, this should correspond to the (zero origin) array index.
The rest of the constants will depend on the type of
socket, which SPI it is driven by, and how it is wired.

*/

#include <assert.h>
#include <string.h>
#include "hw_config.h"
#include "ff.h" /* Obtains integer types */
#include "diskio.h" /* Declarations of disk functions */

// --- Configuration Defaults ---
// (You can define these in your global config or build system to override them)
// These are known good defaults for what i use: RP2350-LCD-1.47-A
// I Used this board due to coming with a sd card reader already
// Finding out your Pinout should be much easier than in my case ...
// In future these defaults should be replaced by more commonly used sdcard reader pinouts
#ifndef SD_SPI_INSTANCE
#define SD_SPI_INSTANCE      spi1
#endif

#ifndef SD_SCK_GPIO
#define SD_SCK_GPIO          10
#endif

#ifndef SD_MOSI_GPIO
#define SD_MOSI_GPIO         11
#endif

#ifndef SD_MISO_GPIO
#define SD_MISO_GPIO         12
#endif

#ifndef SD_BAUD_RATE
// .baud_rate = 25 * 1000 * 1000 // Actual frequency: 20833333.
#define SD_BAUD_RATE         (12500 * 1000)
#endif

#ifndef SD_SS_GPIO
#define SD_SS_GPIO           15
#endif

#ifndef SD_USE_CARD_DETECT
#define SD_USE_CARD_DETECT   0
#endif

#ifndef SD_CARD_DETECT_GPIO
#define SD_CARD_DETECT_GPIO  22
#endif

#ifndef SD_CARD_DETECT_TRUE
#define SD_CARD_DETECT_TRUE  1
#endif
// Hardware Configuration of SPI "objects"
// Note: multiple SD cards can be driven by one SPI if they use different slave
// selects.
static spi_t spis[] = {
    {
        .hw_inst = SD_SPI_INSTANCE,  // SPI component
        .sck_gpio = SD_SCK_GPIO,    // GPIO number (not Pico pin number)
        .mosi_gpio = SD_MOSI_GPIO,
        .miso_gpio = SD_MISO_GPIO,
        .baud_rate = SD_BAUD_RATE,
    }};

// Hardware Configuration of the SD Card "objects"
static sd_card_t sd_cards[] = {  // One for each SD card
    {
        .pcName = "0:",   // Name used to mount device
        .spi = &spis[0],  // Pointer to the SPI driving this card
        .ss_gpio = SD_SS_GPIO,    // The SPI slave select GPIO for this SD card
        .use_card_detect = SD_USE_CARD_DETECT,
        .card_detect_gpio = SD_CARD_DETECT_GPIO,  // Card detect
        .card_detected_true = SD_CARD_DETECT_TRUE  // What the GPIO read returns when a card is present.
    }};

/* ********************************************************************** */
size_t sd_get_num() { return count_of(sd_cards); }
sd_card_t *sd_get_by_num(size_t num) {
    assert(num <= sd_get_num());
    if (num <= sd_get_num()) {
        return &sd_cards[num];
    } else {
        return NULL;
    }
}
size_t spi_get_num() { return count_of(spis); }
spi_t *spi_get_by_num(size_t num) {
    assert(num <= spi_get_num());
    if (num <= spi_get_num()) {
        return &spis[num];
    } else {
        return NULL;
    }
}