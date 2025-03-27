// sri rama jayam
#include<stdio.h>
#include<string.h>

struct Production{
    char lhs;
    char rhs[20];
};

struct Item{
    char lhs;
    char rhs[20];
    int dotPos;
    char LA[20];
};

struct State{
    struct Item items[40];
    int itemCount;
};

struct Production grammar[20];
int numProds=0;

struct State states[20];
int numStates=0;

char result[20];

int isUpper(char c){
    if(c>='A' && c<='Z')
        return 1;
    return 0;
}

void addToResult(char res[],char c){
    int i=0;
    while(res[i]!='\0'){
        if(res[i]==c)
            return;
        i++;
    }
    res[i]=c;
    res[i+1]='\0';
}

int epsilonExists(char res[]){
    for(int i=0;res[i]!='\0';i++){
        if(res[i]=='e')
            return 1;
    }
    return 0;
}

void remove_epsilon(char res[]){
    char set_copy[20]="";
    int i;
    for(i=0;res[i]!='\0';i++){
        if(res[i]!='e')
            addToResult(set_copy,res[i]);
    }

    set_copy[i]='\0';
    strcpy(res,set_copy);
}

void first(char res[],char c){
    // printf("calculating first of %c\n",c);
    char subResult[20]="";
    subResult[0]='\0';
    res[0]='\0';

    if(!isUpper(c))
    {   
        // printf("%c is a terminal\n",c);
        addToResult(res,c);
        // printf("added it to the result\n");
        return;
    }

    for(int i=0;i<numProds;i++){
        struct Production currProd=grammar[i];
        if(currProd.lhs==c){
            // printf("Match found for %c\n",c);
            if(currProd.rhs[0]=='e')
                addToResult(res,'e');
            else{
                for(int j=0;j<strlen(currProd.rhs);j++){
                    first(subResult,currProd.rhs[j]);
                    for(int k=0;subResult[k]!='\0';k++){
                        if(subResult[k]!='e')
                            addToResult(res,subResult[k]);
                    }

                    int foundEpsilon=epsilonExists(subResult);

                    if(!foundEpsilon)
                        break;
                    
                    if(foundEpsilon && currProd.rhs[j+1]=='\0')
                        addToResult(res,'e');
                }
            }
        }
    }
    return;

}




void addProduction(char lhs,char rhs[]){
    grammar[numProds].lhs=lhs;
    strcpy(grammar[numProds].rhs,rhs);
    numProds+=1;
}

int itemExists(struct State* state,char lhs,char rhs[],int dotPos,char LA[]){
    for(int i=0;i<state->itemCount;i++){
        struct Item currentItem=state->items[i];
        if(currentItem.lhs==lhs && strcmp(currentItem.rhs,rhs)==0 && currentItem.dotPos==dotPos && strcmp(currentItem.LA,LA)==0)
            return 1;
    }
    return 0;
}

void addItem(struct State* state,char lhs,char rhs[],int dotPos,char LA[]){

    if(!itemExists(state,lhs,rhs,dotPos,LA)){
    state->items[state->itemCount].lhs=lhs;
    strcpy(state->items[state->itemCount].rhs,rhs);
    state->items[state->itemCount].dotPos=dotPos;
    strcpy(state->items[state->itemCount].LA,LA);
    state->itemCount++;}
}

int equalStates(struct State state1,struct State state2){
    if(state1.itemCount!=state2.itemCount)
        return 0;
    
    for(int i=0;i<state1.itemCount;i++){
        if(state1.items[i].lhs!=state2.items[i].lhs || state1.items[i].dotPos!=state2.items[i].dotPos || strcmp(state1.items[i].LA,state2.items[i].LA)!=0 || strcmp(state1.items[i].rhs,state2.items[i].rhs)!=0){
            return 0;
        }
    }
    return 1;
}

int stateExists(struct State* state){
    for(int i=0;i<numStates;i++){
        if(equalStates(states[i],*state))
            return 1;
    }
    return 0;
}


void closure(struct State *state){

    int i=0;
    while(i<state->itemCount){
        struct Item currentItem=state->items[i];
        if(currentItem.dotPos<strlen(currentItem.rhs)){
            char symbolAfterDot=currentItem.rhs[currentItem.dotPos];
            for(int j=0;j<numProds;j++){
                if(grammar[j].lhs==symbolAfterDot){
                    char req_first[20]="";
                    if(currentItem.rhs[currentItem.dotPos+1]!='\0'){
                        first(req_first,currentItem.rhs[currentItem.dotPos+1]);
                    }
                    else{
                        strcpy(req_first,currentItem.LA);
                    }

                    addItem(state,grammar[j].lhs,grammar[j].rhs,0,req_first);

                }
            }
        }
        i++;
    }

}

struct State goToFunction(struct State* state,char symbol){
    struct State newState;
    newState.itemCount=0;

    for(int i=0;i<state->itemCount;i++){
        char symbolAfterDot=state->items[i].rhs[state->items[i].dotPos];
        if(symbolAfterDot==symbol){
            addItem(&newState,state->items[i].lhs,state->items[i].rhs,state->items[i].dotPos+1,state->items[i].LA);
        }
    }
    closure(&newState);

    return newState;
}

void displayState(struct State *state){
    for(int i=0;i<state->itemCount;i++){
        printf("%c->",state->items[i].lhs);

        for(int j=0;state->items[i].rhs[j]!='\0';j++){
            if(j==state->items[i].dotPos)
                printf(".");
            printf("%c",state->items[i].rhs[j]);
        }
        if(state->items[i].dotPos==strlen(state->items[i].rhs))
            printf(".");
        printf(",");
        printf("%s",state->items[i].LA);
        printf("\n");
    }
}

int main(){
    addProduction('S', "AaAb");
    addProduction('S', "BbBa");
    addProduction('A', "e");
    addProduction('B', "e"); 

    struct State initialState = {.itemCount = 0};
    addItem(&initialState, 'Q', "S", 0,"$");

    states[0]=initialState;
    numStates=1;

    int i=0;
    while(i<numStates){
        closure(&states[i]);
        for(int j=0;j<states[i].itemCount;j++){

            if(states[i].items[j].dotPos<strlen(states[i].items[j].rhs)){
                struct State newState=goToFunction(&states[i],states[i].items[j].rhs[states[i].items[j].dotPos]);

                    if(!stateExists(&newState))
                    {

                        printf("added: \n");
                        printf("%d\n",numStates);
                        displayState(&newState);
                        printf("\n");


                        states[numStates]=newState;
                        numStates=numStates+1;
                        // printf("%d\n",numStates);
                    }

            }

        }
        i++;
    }

}