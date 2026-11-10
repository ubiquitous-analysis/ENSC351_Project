#include "hal/fob.h"
#include "stdio.h"
#include "stdlib.h"
int main(){
    fob_init();
    while(1){
        enum fobSignal value = fob_listen();
        switch (value){
            case sigA:
                printf("Signal A\n");
                break;
            case sigB:
                printf("Signal B\n");
                break;
            case sigC:
                printf("Signal C\n");
                break;
            case sigD:
                printf("Signal D\n");
                break;
            default:
                break;
        }
    }
}