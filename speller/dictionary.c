// Implements a dictionary's functionality
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
#define N 26

// Hash table
node *table[N];
int words_in_dict = 0;
// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int hashIndex = hash(word);
    node *SearchPointer = table[hashIndex];
    while(SearchPointer != NULL){
            if(strcmp(word, SearchPointer->word) == 0){
                return true;
            }
            SearchPointer = SearchPointer->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    return toupper(word[0]) - 'A';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open the dictionary file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL){
        return false;
    }
    // Read each word in the file
    node *wordP = malloc(sizeof(node));
    if(wordP == NULL){
        return false;
    }
    int index = 0, hashIndex;
    char ch;
    while(fread(&ch, sizeof(char), 1, file)){
        if((isalpha(ch) && index < LENGTH) || (ch == '\'' && index > 0)){
            // append characters to word
            wordP->word[index] = tolower(ch);
            index++;
        }
        // we must read a whole word
        else if(index > 0){
            // terminate current word
            wordP->word[index] = '\0';
            words_in_dict++;


            // Add each word to the hash table
            hashIndex = hash(wordP->word);
            wordP->next = table[hashIndex];
            table[hashIndex] = wordP;
            
        
            // prepare for next word
            wordP = malloc(sizeof(node));
            if(wordP == NULL){
                return false;
            }
            index = 0;
        }
    }
    // handle last word
    if(index > 0){
        wordP->word[index] = '\0';
        words_in_dict++;
        hashIndex = hash(wordP->word);
        wordP->next = table[hashIndex];
        table[hashIndex] = wordP;
    }
    else if(index == 0){
        free(wordP);
    }
    // Close the dictionary file
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return words_in_dict;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for(int i = 0; i < N; i++){
        node *ptr = table[i];
        while(ptr != NULL){
            node *tmp = ptr;
            ptr = ptr->next;
            free(tmp);
        }
        table[i] = NULL;
    }    
    words_in_dict = 0;
    return true;
}
