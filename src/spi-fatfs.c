#include "spi-fatfs.h"
#include "spi-fatfs-hwconfig.h"
#include "../usb_script/parser.h"
#include <stdint.h>

int initFs(const char* volume, FatFsState* state){
    if(!sd_init_driver()){
        printf("Could not init sd card\r\n");
        return 0;
    }
    state->fr = f_mount(&state->fs,volume,1);
    if (state->fr != FR_OK) {
        printf("Could not mount fs (%d)\r\n",state->fr);
        return 0;
    }
    state->fs_initialized = true;
    return 1;
}

int openFile(const char* name,unsigned int len, FatFsState* state) {
    state->fr = f_open(&state->fil,name,FA_READ);
    if (state->fr != FR_OK) {
        printf("Could not open file\r\n");
        return 0;
    }
    return 1;
}

int unmountFs(const char* volume,FatFsState* state){
    state->fr = f_unmount(volume);
    if (state->fr != FR_OK) {
        printf("Could not unmount fs (%d)\r\n",state->fr);
        return 0;
    }
    return 1;
}

int closeFile(FatFsState* state){
    state->fr = f_close(&state->fil);
    if (state->fr != FR_OK ){
        printf("Could not Close file \r\n");
        return 0;
    }
    return 1;
}


/*
    Reads and parses a part of the script
    FRESULT f_read (
        FIL* fp,      [IN] File object 
        void* buff,   [OUT] Buffer to store read data 
        UINT btr,     [IN] Number of bytes to read 
        UINT* br      [OUT] Number of bytes read 
    );
*/
int read_script(void* out, unsigned int* out_len, FatFsState* state)
{
    state->fr = f_read(&state->fil,out,1024,out_len);
}

FatFsState state;
const char name[10] = "script.txt";
// For now there is a limit 4KB
char buf[4096]={0};
// For now limited to 128 Commands
UsbCommand cmds[128];
uint32_t cmds_len = 0;

int get_commands() {
    KeysContext ktx={0};
    //unmountFs("0:",&state);
    // Explicitly reset the internal driver state flags
    int ret = initFs("0:",&state);
    if (ret == 0) {
        return -1;
    }
    ret = openFile(name, 10, &state);
    if(ret == 0) {
        return -2;
    }

    while(1){
        memset(buf,0,4096);
        char* s = f_gets(buf,4096,&state.fil);
        if(s == NULL) 
        {
            break;
        }
        int l = strlen(s);
        size_t position_in_buffer = 0;
        ParseResult result = {0};
        while(position_in_buffer < l) {
            ret = parse_line(buf,l,&ktx,&result,&position_in_buffer);
            cmds[cmds_len] = result.cmds[0];
            cmds_len++;
            if(ret == DONE && result.count == 2) {        
                cmds[cmds_len] = result.cmds[1];
                cmds_len++;
            }

            if(ret != DONE){
                break;
            }
        }
    }
    printf("got cmds: %u\n",cmds_len);
    ret = 0;
    ret = closeFile(&state);
    printf("closeFile retuns: %d\n",ret);
    if(ret == 0) {
        return -3;
    }
    ret = unmountFs("0:", &state);
    printf("unmountFs retuns: %d\n",ret);
    if(ret == 0) {
        return -4;
    }
    printf("ret: %d\n",ret);
    return ret;
}