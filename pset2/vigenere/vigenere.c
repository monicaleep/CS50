#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>

int main(int argc, string argv[])
{
    if (argc !=2){
        printf("Usage: ./vigenere k\n");
        return 1;
    }
    int len =strlen(argv[1]);
    int cipher[len];
    //make cipher of integers 0-25;
    for (int i=0;i<len;i++){
        if (!isalpha(argv[1][i])){
            printf("error\n");
            return 1;
        }
        if (isupper(argv[1][i])){
            cipher[i] = argv[1][i] - 65;
        }
        if (islower(argv[1][i])){
            cipher[i] = argv[1][i] - 97;
        }
    }
    //request plaintext from user
   printf("plaintext: ");
    string plaintext ;
    plaintext = get_string();
    printf("ciphertext: ");
    //initialize j as the cipher counter
    int j = 0;
    //iterate through plaintext
    for (int i=0, n=strlen(plaintext); i<n; i++){
        //non-alphabetical characters leave as-is, and don't increment j;
        if (!isalpha(plaintext[i])){
            printf("%c",plaintext[i]);
        }
        if (isupper(plaintext[i])){
            printf("%c",((plaintext[i]-65+cipher[j%len])%26)+65);
            j +=1;
        }
        if (islower(plaintext[i])){
            printf("%c",((plaintext[i]-97+cipher[j%len])%26)+97);
            j +=1;
        }
    }
   printf("\n");
}