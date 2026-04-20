#include "spi-fatfs.h"
#include "spi-fatfs-hwconfig.h"
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
char buf[512]={0};
volatile UsbCommand cmds[128];
volatile uint32_t cmds_len = 0;

int get_commands() {
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
    uint32_t i = 0;
    while(1){
        memset(buf,0,512);
        char* s = f_gets(buf,512,&state.fil);
        if(s == NULL) 
        {
            break;
        }
        int l = strlen(s);
        ret = parse_line(buf,l,&cmds[i]);
        if(ret <= 0){
            break;
        }
        printf("%u %u %u %u %u %u\n",cmds[i].value[0],cmds[i].value[1],cmds[i].value[2],cmds[i].value[3],cmds[i].value[4],cmds[i].value[5]);
        i++;
    }
    cmds_len = i;
    printf("got cmds: %ld\n",cmds_len);
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

/*
extern void put_rgb(uint8_t red, uint8_t green, uint8_t blue);
extern void init_rgb();

int main(){
    stdio_init_all();
    sleep_ms(4000);

    int res = get_commands();
    printf("res returned from get_commands %d\n",res);
    init_rgb();

    if( res == -1 ) {
      put_rgb(255,0,0);
    }
    else if(res == -2){
      put_rgb(255, 255, 0);
    }
    else if(res == -3) {
      put_rgb(0, 255, 255);
    }
    else if(res == -4) {
      put_rgb(0, 0, 255);
    }
    else {
      put_rgb(255, 0,0);
    }
}*/