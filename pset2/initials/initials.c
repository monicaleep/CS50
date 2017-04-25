#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    //get name from user//
    string name = get_string(); 
    
    //if first letter isn't a space, capitalize and print it
    if (name[0]!=32){ 
        printf("%c",toupper(name[0]));
    }
    //iterate through rest of string//
    for (int i=1, n=strlen(name); i<n; i++){
        //if i'th character isn't a space, but previous one is a space, capitalize and print i'th character //
        if (name[i]!=32 && name[i-1]==32){
            printf("%c",toupper(name[i]));
        }
    }
    printf("\n");
}