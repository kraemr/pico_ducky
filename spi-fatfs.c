#include "spi-fatfs.h"

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
    }
    return 1;
}

int closeFile(FatFsState* state){
    state->fr = f_close(&state->fil);
    if (state->fr != FR_OK ){
        printf("Could not Close file \r\n");
        return 0;
    }
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

/*
This worked 
  FILE * fp = fopen("script.txt","rb");
  char buffer[1024] = {'\0'};
  size_t bytes_read = 0; 
  while ((bret_fread(buffer,1,1024,fp,&bytes_read))){
    int offset = parse_line(buffer, bytes_read,&command) +1;
    if (offset <= 0) {
      return 1;
    } 
    //printf("Offset %d\n",offset);
    //printf("After Reading: %ld\n",ftell(fp));
    fseek(fp,offset,SEEK_SET);
    //printf("After Reading and setting offset: %ld\n",ftell(fp));
    //printf("%s\n",buffer);
    memset(buffer, 0, 1024);
    printf("%u %u %u %u %u %u %u %u\n",command.value[0],command.value[1],command.value[2],command.value[3],command.value[4],command.value[5],command.value[6],command.value[7]);
  } 
*/
#include "parser.h"
volatile int main(void) {
    stdio_init_all(); 
    sleep_ms(4000);
    
    char name[256] = "script.txt";
    char out[16384] = {0};
    unsigned int len = 0;
    FatFsState state;
    UsbCommand command;
    size_t bytes_read = 0; 
    
    int ret = initFs("0:",&state);
    ret = openFile(name, 10, &state);
    int offset = 0;

    while(1){
        ret = read_script(out,&len,&state);
        if(ret != 0) {
            printf("Read scirpt failed: %d\n",ret);
        }
        ret = parse_line(out,len,&command);
        printf("%u %u %u %u %u %u %u %u\n",command.value[0],command.value[1],command.value[2],command.value[3],command.value[4],command.value[5],command.value[6],command.value[7]);
        if (ret <= 0) {
            return 1;
        } 
        offset += ret;
        f_lseek(&state.fil, offset+1);
        memset(out, 0, 1024);
    }
    

    closeFile(&state);
    ret = unmountFs("0:", &state);
    
    out[len] = '\0';
   // printf("\nResult:%s\n",out); 
}