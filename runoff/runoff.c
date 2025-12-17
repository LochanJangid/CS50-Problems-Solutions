#include <stdio.h>
#include <string.h>

#define MAX_CANDIDATES 9
#define MAX_VOTERS 100

typedef struct
{
    string name;
    int votes;
    int eliminated;
}
candidate;

candidate candidates[MAX_CANDIDATES];
int preferenes[MAX_VOTERS][MAX_CANDIDATES];

int main(int argc, char *argv[]){
    

    for(int i = 0; i < argc-1; i++){
        candidates[i].name = argv[i+1];
        candidates[i].votes = 0;
    }

    int voters;
    printf("Number of Voters: ");
    scanf("%d", &voters);

    for(int i = 0; i < voters; i++){

        for(int j = 1; j < argc-1; j++){
            string vote;
            printf("Rank %d:", j);
            scanf("%s", vote);

            for(int k = 0; k < argc-1; k++){
            if(strcmp(vote, candidates[j].name) == 0) printf("beta  .... %s\n", vote);
            }
        }
    }

    // VOTE FUNCTION
    
}