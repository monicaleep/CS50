#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

int main(void){
    char* c = "abc";
    for (int x=0;x<3;x++){
        printf("%i\n",c[x]);
    }
}