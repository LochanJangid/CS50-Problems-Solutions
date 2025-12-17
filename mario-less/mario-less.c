#include <stdio.h>


int mario(int myheight);

int main(){

    int height;
    printf("Height: ");
    scanf("%d", &height);

    while( height < 1){
        printf("Height: ");
        scanf("%d", &height);
    }

    mario(height);

    return 0;
}


int mario(int myheight){
        for(int i = 0; i < myheight; i++){
            int spaces = myheight - i; // calculate how many spaces we want
            // for spaces 
            for(int j = 0; j < spaces; j++){
                printf(" ");
            }
            // for hashesh after spaces
            for(int k = 0; k <= i; k++){
                printf("#");
            }
            printf("\n");
        } 
}