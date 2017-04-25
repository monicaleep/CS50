#include <stdio.h>
#include <cs50.h>

bool checksum(long num);
void type(long num);
int main(void)
{
    long num;
    printf("Number: ");
    num = get_long_long();
    if (checksum(num)){
        type(num);
    }
    else{
        printf("INVALID\n");
    }
}


bool checksum(long num){

    long working=num/10;
    int checksum=0;
    while (working>0){
        if (2*(working%10) < 10){
            checksum = checksum + 2*(working%10);
        }
        else {
            checksum = checksum + 1 + (2*(working%10))-10;
        } 
        working = working/100;
    }
    long workinga=num;
    while (workinga>0){
        checksum = checksum + workinga%10;
        workinga = workinga/100;
    }
    if (checksum%10==0){
        return true;
    }
    else{
        return false;
    }
}

void type(long num){
    if(num/10000000000000 ==37 || num/10000000000000 ==34){
        printf("AMEX\n");
    }
    else if(num/100000000000000>50 && num/100000000000000<56){
        printf("MASTERCARD\n");
    }
    else if(num/1000000000000000==4 || num/1000000000000==4){
        printf("VISA\n");
    }
    else{
        printf("INVALID\n");
    }
}