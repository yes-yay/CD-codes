// sri rama jayam

#include<stdio.h>
#include<string.h>


char productions[20][20] = {

    "S=ACB",
    "S=CbB",
    "S=Ba",
    "A=da",
    "A=BC",
    "B=g",
    "B=e",
    "C=h",
    "C=e"
};

int numProds=9;

char result[20];
char followResult[20];

void parse(char[]);

void addToResult(char res[],char c){

    int i=0;
    while(res[i]!='\0'){
        if(res[i]==c)
            return;
        i=i+1;
    }

    res[i]=c;
    res[i+1]='\0';

}

int epsilonExists(char set[]){

    for(int i=0;set[i]!='\0';i++){
        if(set[i]=='e')
            return 1;
    }
    return 0;

}

void union_set(char set1[],char set2[]){

    for(int i=0;set2[i]!='\0';i++)
        addToResult(set1,set2[i]);

}

void remove_epsilon(char set[]){

    char set_copy[20];
    set_copy[0]='\0';

    int i;
    for(i=0;set[i]!='\0';i++)
    {
        if(set[i]!='e')
            addToResult(set_copy,set[i]);
    }

    set_copy[i]='\0';

    strcpy(set,set_copy);
}

int isUpper(char c){
    if(c>='A' && c<='Z')
        return 1;
    return 0;
}

void first(char result[],char c){

    char subResult[20];
    int foundepsilon=0;

    result[0]='\0';
    subResult[0]='\0';

    if(!isUpper(c)){
        addToResult(result,c);
        return;
    }
    else{
        for(int i=0;i<numProds;i++){

            if(productions[i][0]==c){
                if(productions[i][2]=='e')
                    addToResult(result,'e');
                else{
                    for(int j=2;productions[i][j]!='\0';j++){
                        printf("current element is: %c\n",productions[i][j]);
                        foundepsilon=0;
                        first(subResult,productions[i][j]);
                        for(int k=0;subResult[k]!='\0';k++){
                            if(subResult[k]!='e'){
                                addToResult(result,subResult[k]);
                            }
                        }

                        foundepsilon=epsilonExists(subResult);
                        printf("checking first of %c,foundepsilon value is:  %d , for character: %c\n",productions[i][0],foundepsilon,productions[i][j]);
                        if(!foundepsilon)
                            break;
                        // printf(if(productions[i][j]=='\0'))
                        if(productions[i][j+1]=='\0' && foundepsilon==1)
                        {
                            addToResult(result,'e');
                            printf("epsilon is added to first of: %c\n",productions[i][j]);
                            for(int x=0;result[x]!='\0';x++){
                                printf(" %c ",result[x]);
                            }
                        }
                        
                    }
                }
            }

        }
    }
    return;


}

void follow(char followresult[],char c){

    printf("Calculating follow of %c\n",c);
    
    char subFirstResult[20];
    char subFollowResult[20];
    subFirstResult[0]='\0';
    subFollowResult[0]='\0';
    followresult[0]='\0';

    if(isUpper(c) && c==productions[0][0])
        addToResult(followresult,'$');

    for(int i=0;i<numProds;i++){
        for(int j=2;productions[i][j]!='\0';j++){

        if(productions[i][j]==c){
            if(productions[i][j+1]=='\0' && productions[i][0]==c)
                continue;
            printf("Found match at %d %d\n",i,j);
            if(productions[i][j+1]=='\0'){
                printf("As this is the last element in that production we are adding follow of the non-terminal\n");
                follow(subFollowResult,productions[i][0]);
                union_set(followresult,subFollowResult);
            }

            else{
                printf("%c is not the last element in the production %s\n",productions[i][j],productions[i]);
                int k=j+1;
                printf("next element is: %c\n",productions[i][k]);
                for(k=j+1;productions[i][k]!='\0';k++){
                    printf("we are inside the for loop\n");
                    first(subFirstResult,productions[i][k]);
                    printf("we are adding elements of first of %c to follow of %c\n",productions[i][k],productions[i][j]);
                    
                    int foundEpsilon=epsilonExists(subFirstResult);

                    union_set(followresult,subFirstResult);

                    if(foundEpsilon && productions[i][k+1]=='\0'){
                        follow(subFollowResult,productions[i][0]);
                        union_set(followresult,subFollowResult);
                    }

                    if(foundEpsilon)
                        remove_epsilon(followresult);

                    if(!foundEpsilon)
                        break;
                    
                }
                printf("we are outside for loop\n");
            }
        }

        }
    }
}

int main(){

    printf("The productions are: \n");
    for(int i=0;i<numProds;i++){
        printf("%s\n",productions[i]);
    }

    // printf("\n\n\n\n");
    // printf("first of D is: ");
    // first(result,'D');

    // printf("\n\n\n\n");

    // for(int i=0;result[i]!='\0';i++){
    //     printf("%c ",result[i]);
    // }

    // while(1){
    //     char c;
    //     printf("what element do you want to find follow of: ");
    //     scanf(" %c",&c);

    //     follow(followResult,c);
    //     for(int i=0;followResult[i]!='\0';i++){
    //         printf("%c ",followResult[i]);
    //     }
    //     printf("\n");
    // }

    char input_string[20];
    printf("Enter string for parsing\n");
    scanf("%s",input_string);

    createParsingTable();
    parse(input_string);
}

char stack[20];
int top=-1;

char parsing_table[130][130][20];

void push(char c){
    top=top+1;
    stack[top]=c;
}

char pop(){
    char popped_element=stack[top];
    top=top-1;
    return popped_element;
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
        first(first_result,productions[i][0]);
        for(int j=0;first_result[j]!='\0';j++){
            if(first_result[j]!='e')
            {
                strcpy(parsing_table[productions[i][0]][first_result[j]],productions[i]);
                printf("Production %s on %c %c\n",productions[i],productions[i][0],first_result[j]);
            }
            else{
                char follow_result[20];
                follow(follow_result,productions[i][0]);
                char gives_epsilon[20]={productions[i][0],'=','e','\0'};
                for(int k=0;follow_result[k]!='\0';k++){
                    strcpy(parsing_table[productions[i][0]][follow_result[k]],gives_epsilon);
                    printf("Production %s on %c %c\n",gives_epsilon,productions[i][0],follow_result[k]);
                }
            }
        }
    }

}

void parse(char input[]){

    int k=0;
    push('$');
    push(productions[0][0]);

    while(getTop()!='$'){
        if(isUpper(getTop())){
            if(parsing_table[getTop()][input[k]][0]=='\0')
            {       printf("Error: String not accepted\n");
                    return;}
            else{
                
                char current_production[20]="";
                strcpy(current_production,parsing_table[getTop()][input[k]]);
                printf("production chosen is: %s\n",current_production);

                char current_production_reverse[20]="";
                int i=0,z=0;

                for(i=strlen(current_production)-1;current_production[i]!='=';i--){
                    current_production_reverse[z]=current_production[i];
                    z=z+1;
                }

                current_production_reverse[z]='\0';

                char non_terminal=pop();
                printf("%c has been popped\n",non_terminal);

                for(int j=0;current_production_reverse[j]!='\0';j++){
                    if(current_production_reverse[j]!='e'){
                        push(current_production_reverse[j]);
                    }
                }
            }
        }
        else{
            if(getTop()==input[k])
            {
                k=k+1;
                char terminal=pop();
                printf("%c has been popped\n",terminal);
            }
            else{
                printf("Error: String not accepted\n");
                return;
            }
        }
    }
    printf("String accepted\n");

}







