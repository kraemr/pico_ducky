#include <stdio.h>
#include <stdint.h>

int main(){
    uint16_t key = 0b1111111110000000;
    uint8_t keymod;
    uint8_t actual_key;
    keymod = key & 255;
    actual_key = key >> 8;
    printf("%d %d",keymod,actual_key);
}
