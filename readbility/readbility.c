#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>


int main(){
    char text[100];

    // take input from user
    printf("Text: ");
    fgets(text, sizeof(text), stdin); // for read all text input

    int words = 0;
    int sentences = 0;
    int letters = 0;

    int length = strlen(text);
    char ch;

    for(int i = 0; i < length; i++){
        ch = text[i];
        if(isalpha(ch)){
            letters++;
            if(i == 0 || text[i-1] == ' '){
                words++;
            }
        }
        else if(ch == '.' || ch == '?' || ch == '!'){
            sentences++;
        }
    }

    float L = letters / ( float ) words * 100;
    float S = sentences / ( float ) words * 100;
    
    int index = round(0.0588 * L - 0.296 * S - 15.8);

    if(index < 1){
        printf("Before Grade 1");
    }
    else if(index > 16){
        printf("Grade 16+");
    }
    else{
    printf("Grade %d", index);
    }
}