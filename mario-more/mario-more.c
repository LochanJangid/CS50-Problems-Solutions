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
printf("Press enter to exit...\n");
    getchar();

    return 0;
}


int mario(int myheight){
        for(int i = 0; i < myheight; i++){
            int spaces = myheight - i;
            for(int j = 0; j < spaces; j++){
                printf(" ");
            }
            for(int k = 0; k <= i; k++){
                printf("#");
            }
            printf("  ");
            for(int l = 0; l <= i; l++){
                printf("#");
            }
            printf("\n");   
        }  
}