#include <stdio.h>

int change(int changeAmount);

int main(){

    int amount;

    printf("owes amount ? =");
    scanf("%d", &amount);

    if( amount < 0){
    printf("owes mount ? =");
    scanf("%d", &amount);
    }
   
    change(amount);

}

int change(int changeAmount){
    int quarters = 50;
    int dimes = 10;
    int nickels = 5;
    int pennies  = 1;
    int i = 0;
    while(quarters <= changeAmount){
        changeAmount -= quarters;
        i++;
    }
    while(dimes <= changeAmount){
        changeAmount -= dimes;
        i++;
    }
    while(nickels <= changeAmount){
        changeAmount -= nickels;
        i++;
    }
    while(pennies <= changeAmount){
        changeAmount -= pennies;
        i++;
    }
    printf("%d", i);
}