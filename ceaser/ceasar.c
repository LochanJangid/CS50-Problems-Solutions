#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]){
    
    // Get Key
    char* strKey = argv[1]; // take string type number
    int key = *strKey - 48; // convert string to number

    // Conditions of Key input

   if(argc > 2){
        printf("Usage: ./caesar key");
        return 1;
   }

    for(int i = 0; i < strlen(strKey); i++){
        if(!(strKey[i] >= '0' && strKey[i] <= '9')){
        printf("Usage: ./caesar key");
        return 1;
        }
    }

    // Get plain text
    char pText[100]; 

    printf("Etner PlainText: ");
    fgets(pText, sizeof pText, stdin);
    
    int length = strlen(pText);

    char cText[length];

    for(int i = 0; i < length; i++){
        if(!((pText[i] >= 'A' && pText[i] <= 'Z') || (pText[i] >= 'a' && pText[i] <= 'z'))){
            cText[i] = pText[i];
        }
        else if(pText[i] >= 'A' && pText[i] <= 'Z'){
            // for newText or reverse to a number
            int newText = pText[i] + key;
            if( newText > 'Z'){
                cText[i] = newText - ('[' - 'A');
            }else{
                cText[i] = newText;
            }
        }
        else if(pText[i] >= 'a' && pText[i] <= 'z'){
            int newText = pText[i] + key;
            if( newText > 'z'){
                cText[i] = newText - ('{' - 'a');
            }
            else{
                cText[i] = newText;
            }
        }
    }

    printf("Cipher Text: %s", cText);
    return 0; // for ignoring garbage things in the last please solve this if you know 
}