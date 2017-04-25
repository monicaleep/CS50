#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(void){
    RGBTRIPLE triple;
    printf("%lu\n",sizeof(triple));
    printf("%lu\n",(4 - (3 * sizeof(triple)) % 4) % 4);
    printf("%i",(4 - 6 % 4) % 4);
}
