#include <stdio.h>
#include <string.h>
#include <math.h>
float length; // length of text

int main(){
    char text[100];

    // take input from user
    printf("Text: ");
    fgets(text, sizeof(text), stdin); // for read all text input

    int words = 1;
    int letters = -1;
    int sentences = 0;

    length = strlen(text);

    for(int i = 0; i < length; i++){
        if(text[i] == ' '){
            words++;
        }else if(text[i] == '.' || text[i] == '?' || text[i] == '!'){
            sentences++;
        }
        else{
            letters++;
        }
    }

    float L = letters / ( float ) words;
    float S = sentences / ( float ) words;
    int index = round(5.88 * L + 29.6 * S - 15.8);

    if(index < 1){
        printf("Before Grade 1");
    }
    else if(index > 16){
        printf("Grade 16+");
    }
    else{
    printf("Your Grade is %d", index);
    }
}