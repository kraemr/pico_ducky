extern void execute_ducky_payload();
#include <stdio.h>
extern const int MAIN_PAYLOAD_LEN;

int main(){
    for(int i = 0; i < MAIN_PAYLOAD_LEN;i++){
        execute_ducky_payload();
    }
}