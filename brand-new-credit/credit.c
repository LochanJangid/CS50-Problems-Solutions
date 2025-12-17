#include <stdio.h>

int main(){
    // input card Number
    int card_number;
    printf("Enter Your card Number = ");
    scanf("%d", &card_number);

    int card_length = 6; // length of card number

    int arr[card_length]; // array for put every integer from card_number and use it 

    int tracker = 1; // for in loop we can multiply it everytime by 10

    int additionAuth = 0; // addition of authantication
    
    // loop for convert integer to array

    for (int i = 0; i < card_length; i++){
        tracker *= 10;
        int oneNum = card_number % tracker; // remainder
        if(oneNum > 9){
                oneNum = ( oneNum - arr[i-1]) / ( tracker / 10); // finding number logic
        }
        arr[i] = oneNum; // made an array
        // condition for second-to-last digit addition
        if(i % 2 == 0){
            arr[i] *= 2; 
            // loop for if any number come like 12 we do 1 + 2 
             if(arr[i] > 9){
                int newlastDigit = arr[i] % 10; // get last digit
                int newFirstDigit = ( arr[i] - newlastDigit ) / 10; // get first digit
                arr[i] = newlastDigit + newFirstDigit; // add both 
            }
            additionAuth += arr[i];
        }
        // condition for another digits addition
        if(i % 2 != 0){
            additionAuth += arr[i];
        }
    }
    if(additionAuth % 2 == 0){
        printf("Welcome to My World Bro.");
    }else{
        printf("SORRY INVALID BRO");
    }
    return 0;
}