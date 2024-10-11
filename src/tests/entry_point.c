// cc -fPIE -o entry_point entry_point.c
#include <stdio.h>

int main(void){
    printf("main is at %p\n", &main);
}
