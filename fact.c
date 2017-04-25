#include <stdio.h>
#include <cs50.h>

int fact(int n){
    if (n==1){
        return 1;
    }
    else{
        return n*fact(n-1);
    }
}
int main(void){
    printf("n: ");
    int n = get_int();
    printf("%i\n",fact(n));
}