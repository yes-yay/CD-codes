//sri rama jayam
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#include<stdio.h>
#include<string.h>

char productions[20][20] = {

    "S=aBDh",
    "B=cC",
    "C=bC",
    "C=e",
    "D=EF",
    "E=g",
    "E=e",
    "F=f",
    "F=e"

};

int numProds=9;


char result[20];
char followResult[20];

void parse(char[]);
void createParsingTable();
// int num_terminals=6;
// int num_non_terminals=5;

// int resultLen=0;

char input[20];
char parsing_table[130][130][20];


void addToResult(char res[],char c){
    int i=0;
    while(res[i]!='\0'){
        if(res[i]==c)
            return;
        i=i+1;
    }
    res[i]=c;
    res[i+1]='\0';
    // printf("\nAdded %c to required array\n",c);
    // resultLen=resultLen+1;
}

void union_set(char set1[],char set2[]){
    //get union of set1 and set2
    for(int i=0;i<strlen(set2);i++){
        addToResult(set1,set2[i]);
    }
}

void remove_epsilon(char set[]){

    // printf("These are the set elements:");
    // for(int i=0;set[i]!='\0';i++){
    //     printf("%c ",set[i]);
    // }


    char set_copy[20];
    set_copy[0]='\0';
    for(int i=0;set[i]!='\0';i++){
        // printf("%d \n",i);
        if(set[i]!='e'){
            addToResult(set_copy,set[i]);
        }
    }

    // printf("These are the set_copy elements:");
    // for(int i=0;set_copy[i]!='\0';i++){
    //     printf("%c\n",set_copy[i]);
    // }

    // set[0]='\0';
    // for(int i=0;set_copy[i]!='\0';i++){
    //     set[i]=set_copy[i];
    // }
    strcpy(set,"");
    int temp_count = 0;
    while(set_copy[temp_count]!='\0'){
        set[temp_count] = set_copy[temp_count];
        temp_count++;
    }
    set[temp_count] = '\0';
    // &set=&set_copy;

    // printf("string length: %d",strlen(set));

}

int epsilonExists(char set[]){
    for(int i=0;i<strlen(set);i++){
        if(set[i]=='e')
            return 1;
    }
    return 0;
}

void First(char[],char );

int main(){

    // char set[6]={'a','b','c','e','d','\0'};
    // remove_epsilon(set);
    // printf("These are set elements after removing epsilon: \n");
    // for(int i=0;i<strlen(set);i++){
    //     printf("e%c ",set[i]);
    // }

    // printf("Enter number of productions now: ");
    // scanf("%d",&numProds);

    // printf("\nEnter productions now: ");
    // for(int i=0;i<numProds;i++){
    //     scanf("%s",productions[i]);
    // }

    // printf("\nYour productions are: ");
    
    // for(int i=0;i<numProds;i++){
    //     printf("%s\n",productions[i]);
    // }
    
    for(int i=0;i<numProds;i++){
        First(result,productions[i][0]);
        printf("FIRST %c is: \n",productions[i][0]);
        for(int j=0;result[j]!='\0';j++){
            printf("%c ",result[j]);
        }
        printf("\n");
    }

    for(int i=0;i<numProds;i++){
        follow(result,productions[i][0]);
        printf("FOLLOW %c is: \n",productions[i][0]);
        for(int j=0;result[j]!='\0';j++){
            printf("%c ",result[j]);
        }
        printf("\n");
    }


    while(1){
    // char c;
    // printf("\nEnter element you want to find Follow of: ");
    // scanf(" %c",&c);

    // follow(followResult,c);

    // printf("\nFOLLOW(%c) = { ",c);
    // for(int i=0;i<strlen(followResult);i++){
    //     printf("%c ",followResult[i]);
    // }
    // printf("}\n");

        printf("Enter string you want to parse: ");
        scanf("%s",input);

        createParsingTable();
        // for(int i=0;i<130;i++){
        //     for(int j=0;j<130;j++){
        //         for(int k=0;parsing_table[i][j][k]!='\0';k++){
        //             if(parsing_table[i][j][k]!='\0')
        //                 printf("%c",parsing_table[i][j][k]);
        //         }
        //         printf("\n");
        //     }
        //     printf("\n");
        // }
        parse(input);
    }
}

int isUpper(char c){
    if(c>='A' && c<='Z')
        return 1;
    return 0;
}

void First(char result[], char c) {
    char subresult[20];
    int foundEpsilon = 0;

    result[0] = '\0';
    subresult[0] = '\0';

    if (!isUpper(c)) { // Terminal case
        addToResult(result, c);
        return;
    } else {
        for (int i = 0; i < numProds; i++) { // Check for each production
            if (productions[i][0] == c) { // Production starts with the current symbol
                if (productions[i][2] == 'e') { // A -> e case
                    addToResult(result, 'e');
                    // printf("e added to FIRST of %c\n", c);
                } else {
                    int j = 2;
                    while (productions[i][j] != '\0') {
                        foundEpsilon = 0;
                        First(subresult, productions[i][j]); // Recursively find FIRST
                        for (int k = 0; subresult[k] != '\0'; k++) {
                            // printf("The subresult is: %c  ", subresult[k]);
                            if(subresult[k]!='e')
                                addToResult(result, subresult[k]);
                        }

                        // Check if epsilon exists in the subresult
                        for (int k = 0; subresult[k] != '\0'; k++) {
                            if (subresult[k] == 'e') {
                                foundEpsilon = 1;
                                // printf("Epsilon found, moving to the next element\n");
                                break;
                            }
                        }

                        if (!foundEpsilon) {
                            break; // Stop if no epsilon, we're done with this production
                        }

                        j++; // Move to the next symbol in the production

                        if(productions[i][j] == '\0' && foundEpsilon) {
                            addToResult(result,'e');
                        }

                    }
                }
            }
        }
    }

    return;
}

void follow(char followResult[],char c){

    followResult[0]='\0';
    char subFollowResult[20];
    subFollowResult[0]='\0';
    char subFirstResult[20];
    subFirstResult[0]='\0';

    printf("\n\n%c\n\n",c);

    if(c==productions[0][0]) 
        {
        addToResult(followResult,'$');
        
        // printf("Added $ to follow of %c\n",productions[0][0]);
        }

    for(int i=0;i<numProds;i++){

        for(int j=2;productions[i][j]!='\0';j++){

            if(productions[i][j]==c){

                if(productions[i][j+1]!='\0')
                    {
                        First(subFirstResult,productions[i][j+1]);
                        if(!epsilonExists(subFirstResult)){
                            union_set(followResult,subFirstResult);
                            // printf("First of %c is being added to follow of %c\n",productions[i][j+1],productions[i][j]);
                        }
                        else{

                            if(productions[i][j+2]=='\0'){
                            follow(subFollowResult,productions[i][0]);
                            // printf("follow of %c is being added to follow of %c\n",productions[i][0],productions[i][j]);
                            union_set(subFirstResult,subFollowResult);
                            union_set(followResult,subFirstResult);
                            // printf("subfollowresult has follow of %c:\n",productions[i][0]);
                            for(int x=0;subFollowResult[x]!='\0';x++){
                                printf("%c ",subFollowResult[x]);
                            }
                            printf("\n");

                            // printf("follow has follow of %c:\n",productions[i][j]);
                            for(int x=0;followResult[x]!='\0';x++){
                                printf("%c ",followResult[x]);
                            }
                            printf("\n");

                            // printf("subFirstResult has first of %c:\n",productions[i][j+1]);
                            for(int x=0;subFirstResult[x]!='\0';x++){
                                printf("%c ",subFirstResult[x]);
                            }
                            printf("\n");



                            // printf("First of %c is being added to follow of %c\n",productions[i][j+1],productions[i][j]);
                            remove_epsilon(followResult);
                            }
                            else{
                                First(subFollowResult,productions[i][j+2]);
                                // printf("follow of %c is being added to follow of %c\n",productions[i][j+2],productions[i][j]);
                                union_set(subFirstResult,subFollowResult);
                                union_set(followResult,subFirstResult);
                                remove_epsilon(followResult);
                            }
                        }
                    }
                else{

                    if(productions[i][0]!=c){
                        follow(subFollowResult,productions[i][0]);
                    
                        // printf("follow of %c is being added to follow of %c\n",productions[i][0],productions[i][j]);

                        for(int i=0;subFollowResult[i]!='\0';i++){
                            addToResult(followResult,subFollowResult[i]);
                        }
                    }
                }
                

            }

        }

    }

}

//creating stack adt

char stack[20];
int top = -1;

void push(char c){
    top++;
    stack[top]=c;
}

char pop(){
    top--;
    return stack[top+1];
}

void printStack(){
    for(int i=0;i<=top;i++){
        printf("%c ",stack[i]);
    }
    printf("\n");
}

char getTop(){
    return stack[top];
}

void createParsingTable(){

    for(int i=0;i<130;i++){
        for(int j=0;j<130;j++){
            for(int k=0;k<20;k++){
                parsing_table[i][j][k]='\0';
            }
        }
    }


    for(int i=0;i<numProds;i++){
        char first_result[20];
        First(first_result,productions[i][2]);
        for(int j=0;first_result[j]!='\0';j++){
            printf("%c exists in first of %c\n",first_result[j],productions[i][2]);
            if(first_result[j]!='e'){
                strcpy(parsing_table[productions[i][0]][first_result[j]],productions[i]);
                printf("entered %s on %c %c\n",productions[i],productions[i][0],first_result[j]);
            }
            else{
                printf("epsilon found in first of %c\n",productions[i][2]);
                char follow_result[20];
                follow(follow_result,productions[i][0]);
                char gives_epsilon[20]={productions[i][0],'=','e'};
                

                for(int k=0;follow_result[k]!='\0';k++){
                    strcpy(parsing_table[productions[i][0]][follow_result[k]],gives_epsilon);
                    printf("entered %s on %c %c\n",gives_epsilon,productions[i][0],follow_result[k]);
                }
            }
        }
    }
}

void parse(char input[]){

    printf("Parsing has started\n\n\n");

    int k=0;
    push('$');
    push(productions[0][0]);

    while(getTop()!='$'){
        if(isUpper(getTop())){
            if(parsing_table[getTop()][input[k]][0]=='\0')
            {
                printf("error because top is: %c and input lookahead is: %c\n",getTop(),input[k]);

                // break;
                return;
            }


            printf("top is a non terminal: %c\n",getTop());
            if(k<strlen(input)){
                printf("The input lookahead is: %c\n",input[k]);
            }
            else{
                printf("input is over\n");
                break;
            }
                char current_production[20]="";
                strcpy(current_production,parsing_table[getTop()][input[k]]);
                printf("production chosen is: %s\n",current_production);

                printf("current production[2] is: %c\n",current_production[2]);
                char current_production_reverse[20]="";
                int i=0;
                int z=0;
                for(i=strlen(current_production)-1;current_production[i]!='=';i--){
                    current_production_reverse[z]=current_production[i];
                    z=z+1;
                    printf("%c has been added to reverse_production\n",current_production_reverse[z-1]);
                }
                current_production_reverse[z]='\0';
                printf("The reverse production is: %s\n",current_production_reverse);

            char non_terminal=pop();
            printf("%c has been popped\n",non_terminal);

            for(int j=0;current_production_reverse[j]!='\0';j++){
                if(current_production_reverse[j]!='e')
                {
                    push(current_production_reverse[j]);
                    printf("%c has been pushed\n",current_production_reverse[j]);
                }
            }

        }

        else{
            printf("top is a terminal: %c\n",getTop());
            printf("input lookahead is: %c\n",input[k]);
            if(getTop()==input[k]){
                k++;
                char popped=pop();
                printf("%c has been popped\n",popped);
            }
            else{
                printf("error\n");
                return;
            }
        }


        
    }
    printf("string accepted\n");


}


