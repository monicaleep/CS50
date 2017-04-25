#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>

int main(int argc, string argv[])
{
    //confirm two arguments presented by user
    if (argc !=2){
        printf("Usage: ./caesar k\n");
        return 1;
    }
    int k = atoi(argv[1]);
    //request plaintext from user
   printf("plaintext:");
    string plaintext ;
    plaintext = get_string();
    printf("ciphertext: ");
    //iterate through plaintext
    for (int i=0, n=strlen(plaintext); i<n; i++){
        //non-alphabetical characters leave as-is;
        if (!isalpha(plaintext[i])){
            printf("%c",plaintext[i]);
        }
        if (isupper(plaintext[i])){
            printf("%c",((plaintext[i]-65+k)%26)+65);
        }
        if (islower(plaintext[i])){
            printf("%c",((plaintext[i]-97+k)%26)+97);
        }
    }
   printf("\n");
}