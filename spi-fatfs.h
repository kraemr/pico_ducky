
#ifndef FATFS_UTILS_H
#define FATFS_UTILS_H

#include "pico/stdlib.h"
#include "sd_card.h"
#include "ff.h"
#include <stdio.h>
#include "rtc.h"
#include "hardware/spi.h"
#include "hardware/dma.h"

typedef struct FatFsState {
    bool fs_initialized;
    bool file_is_open;
    FRESULT fr;
    FATFS fs;
    FIL fil;
}FatFsState;

int initFs(const char* volume, FatFsState* state);
int openFile(const char* name, unsigned int len, FatFsState* state);
int unmountFs(const char* volume, FatFsState* state);
int closeFile(FatFsState* state);
int read_script(void* out, unsigned int* out_len, FatFsState* state);
#endif