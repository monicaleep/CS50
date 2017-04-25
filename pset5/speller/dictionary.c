/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "dictionary.h"

    typedef struct node{
        bool isword;
        struct node *child[27];
    }node;
    
    node* root;
    long* count;
/**
 * Returns true if word is in dictionary else false.
 */
void freeTrie(node* root);

bool check(const char *word)
{
    node* trav = root;
    
    int length = strlen(word);
    //printf("%i",length);
    for (int i=0; i<=length; i++){
        int hold = -1;
        if (word[i]=='\0'){
            if (trav->isword == true){
                return true;
            }
        }
        if (isalpha(word[i])){
            hold = tolower(word[i])-97; 
        }
        if (word[i]=='\''){
            hold = 26;
        }
        if (trav->child[hold] != NULL){
            trav = trav->child[hold];
        }
        else {
            return false;
        }

    }
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
   FILE* dic = fopen(dictionary,"r");
   if (dic == NULL){
       printf("Error to open dictionary\n");
       return false;
   }
   int c = '\0';
   long* num_words;
   count = calloc(1,sizeof(long));
   num_words = count;
   //printf("%li", *num_words);
   root = calloc(1,sizeof(node));
   node* trav;
    while (c != EOF){
        //reset trav back to root
        trav = root;
        for (c = fgetc(dic); ((c != '\n') && (c != EOF)); c = fgetc(dic)){
            int letter = -1;
            if (isalpha(c)){
                letter = c - 97;
            }
            if (c== '\''){
                letter = 26;
            }
            if (trav->child[letter]==NULL){
                trav->child[letter]= (node *)calloc(1,sizeof(node));
                trav = trav->child[letter];
            }
            else {
                trav = trav->child[letter];
            }
        }
        //reached end of word
        trav->isword = true;
        *num_words = *num_words + 1;
        
   }
   fclose(dic);
   *count = *num_words - 1;
   return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void){
    if (count != NULL){
    return *count;
    }
    return 0;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    //free(count);
    //node* trav = root;
    freeTrie(root);
    free(count);
    return true;
}

void freeTrie(node* ptr){
    node* trav = ptr;
    for (int i=0; i<27; i++){
        if (trav->child[i] != NULL){
            freeTrie(trav->child[i]);
        }
        if (i==26 && trav != NULL){
            free(trav);
        }
    }
}
