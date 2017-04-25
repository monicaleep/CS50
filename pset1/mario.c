#include <stdio.h>
#include <cs50.h>

void pyramid(int n);
int main(void)
{
    int n;
    do
    {
        printf("Height: ");
        n = get_int();
    }
    while (n<0 || n>23);
    pyramid(n);
}

void pyramid(int n)
{
    int hash=1;
    int space=n-1;
    while (n>0)
    {
        for (int i=space;i>0;i--){
            printf(" ");
        }
        for (int i=hash;i>0;i--){
            printf("#");
        }
        printf("  ");
        for (int i=hash;i>0;i--){
            printf("#");
        }
        hash++;
        space--;
        printf("\n");
        n--;
    }
}