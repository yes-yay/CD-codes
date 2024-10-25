// sri rama jayam
// sri rama jayam

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Production{
    char lhs;
    char rhs[20];
};

struct Item{
    char lhs;
    char rhs[20];
    int dotPos;
};

struct State{
    struct Item items[20];
    int itemCount;
};

struct Production grammar[20];
int numProductions=0;

struct State states[20];
int numStates=0;

char terminals[5]={'a','b','$','\0'};

int isUpper(char c){
    if(c>='A' && c<='Z')
        return 1;
    return 0;
}

void addProduction(char lhs,const char *rhs){
    grammar[numProductions].lhs=lhs;
    strcpy(grammar[numProductions].rhs,rhs);
    numProductions++;
}

int itemExists(struct State *state,char lhs,const char *rhs,int dotPos){
    for(int i=0;i<state->itemCount;i++){
        if(state->items[i].lhs==lhs && state->items[i].dotPos==dotPos && strcmp(rhs,state->items[i].rhs)==0)
            return 1;
    }
    return 0;
}

void addItem(struct State *state,char lhs,const char *rhs,int dotPos){
    if(!itemExists(state,lhs,rhs,dotPos)){
        state->items[state->itemCount].lhs=lhs;
        strcpy(state->items[state->itemCount].rhs,rhs);
        state->items[state->itemCount].dotPos=dotPos;
        state->itemCount++;
    }
}

void closure(struct State *state){
    int i=0;
    while(i<state->itemCount){
        struct Item currentItem=state->items[i];
        if(currentItem.dotPos<strlen(currentItem.rhs)){
            char symbolAfterDot=currentItem.rhs[currentItem.dotPos];
            for(int j=0;j<numProductions;j++){
                if(grammar[j].lhs==symbolAfterDot){
                    addItem(state,grammar[j].lhs,grammar[j].rhs,0);
                    // struct State prevState=*state;
                    // closure(state);
                    // if(equalStates(state,prevState))
                    //     break;
                }
            }
        }
        i++;
    }
}

struct State goToFunction(struct State *state,char symbol){
    // printf("Inside go-to for symbol %c\n",symbol);
    // printf("in state:\n");
    // displayState(state);

    struct State newState;
    newState.itemCount=0;

    for(int i=0;i<state->itemCount;i++){
        struct Item currentItem=state->items[i];
        if(currentItem.dotPos<strlen(currentItem.rhs) && currentItem.rhs[currentItem.dotPos]==symbol){
            addItem(&newState,currentItem.lhs,currentItem.rhs,currentItem.dotPos+1);
        }
    }
    closure(&newState);
    return newState;
}

void displayState(struct State *state) {
    for (int i = 0; i < state->itemCount; i++) {
        printf("%c -> ", state->items[i].lhs);
        for (int j = 0; j < strlen(state->items[i].rhs); j++) {
            if (j == state->items[i].dotPos) {
                printf(".");
            }
            printf("%c", state->items[i].rhs[j]);
        }
        if (state->items[i].dotPos == strlen(state->items[i].rhs)) {
            printf(".");
        }
        printf("\n");
    }
}

int equalStates(struct State state1,struct State state2){
    if(state1.itemCount!=state2.itemCount)
        return 0;

    for(int i=0;i<state1.itemCount;i++){
        if(state1.items[i].lhs!=state2.items[i].lhs || state1.items[i].dotPos!=state2.items[i].dotPos || strcmp(state1.items[i].rhs,state2.items[i].rhs)!=0)
            return 0;
    }
    return 1;
}

int stateExists(struct State *state){
    for(int i=0;i<numStates;i++){
        if(equalStates(states[i],*state))
            return 1;
    }
    return 0;
}

struct Entrie{
    struct State state;
    char symbol;
    struct State newState;
};

struct Entrie entries[20];
int numEntries=0;

struct tableEntry{
    int from_state_number;
    char symbol;
    int to_state_number;
    int reduction;
};

struct tableEntry table[20];
int numTableEntries=0;


int give_state_number(struct State* state){
    for(int i=0;i<numStates;i++){
        if(equalStates(states[i],*state))
            return i;
    }
    return -1;
}

int give_rhs_length_of_production(int prod_num){

    return strlen(grammar[prod_num-1].rhs);
}

char gives_lhs_of_production(int prod_num){

    return grammar[prod_num-1].lhs;
}

int state_equals_production(struct State state){
    if(state.itemCount==1){
        for(int i=0;i<numProductions;i++){
            if(grammar[i].lhs== state.items[0].lhs && strcmp(grammar[i].rhs,state.items[0].rhs)==0)
                return i+1;
        }
    }
    return 0;
}


void createTable(){
    for(int i=0;i<numEntries;i++){
        table[numTableEntries].from_state_number=give_state_number(&entries[i].state);
        table[numTableEntries].symbol=entries[i].symbol;
        table[numTableEntries].to_state_number=give_state_number(&entries[i].newState);
        numTableEntries=numTableEntries+1;
    }

    for(int i=0;i<numStates;i++){
        if(state_equals_production(states[i])){
            for(int j=0;j<3;j++){
                table[numTableEntries].from_state_number=give_state_number(&states[i]);
                table[numTableEntries].symbol=terminals[j];
                // table[numTableEntries].to_state_number=state_equals_production(goToFunction(&states[i],terminals[j]));
                table[numTableEntries].reduction=state_equals_production(states[i]);
                numTableEntries=numTableEntries+1;
            }
        }
    }
}

void printTable(){
    for(int i=0;i<numTableEntries;i++){
        if(table[i].reduction==-1)
        printf("%d %c %d\n",table[i].from_state_number,table[i].symbol,table[i].to_state_number);
        else
        printf("%d %c R%d\n",table[i].from_state_number,table[i].symbol,table[i].reduction);
    }
}

struct output{
    int t_or_nt;
    int state_number;
};

struct output traverseTable(int number, char character){
    for(int i=0;i<numTableEntries;i++){
        if(table[i].from_state_number==number && table[i].symbol==character){
            if(table[i].reduction==-1)
                {
                    struct output return_value;
                    return_value.t_or_nt=1;
                    return_value.state_number=table[i].to_state_number;
                    return return_value;
                }
            else {
                // printf("Reduction exists: %d\n",table[i].reduction);
                struct output return_value;
                return_value.t_or_nt=0;
                return_value.state_number=table[i].reduction;
                return return_value;
            }
        }
    }
}

char stack[50];
int top=-1;

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

void printStack(){
    for(int i=0;i<=top;i++){
        printf("%c ",stack[i]);
    }
    printf("\n");
}

void parse(char input[]){
    printf("Inside parsing\n");
    int k=0;
    push('$');
    push('0');

    while(getTop()!='$'){
        // if(k==3)
        //     break;
        printStack();
        struct output value=traverseTable(getTop()-'0',input[k]);
        printf("we get result: %d %d for %d on %c where k is: %d\n",value.t_or_nt,value.state_number,getTop(),input[k],k);
        if(value.t_or_nt==1){
            int getTop_value=getTop();
            push(input[k]);
            push(value.state_number+'0');
            // printf("Pushing %d",value.state_number);
            k=k+1;
            printf("We got a shift operation from %c to %c due to %d on %c\n",input[k-1],input[k],getTop_value,input[k-1]);

        }
        else if(value.t_or_nt==0){
            int rhs_length=give_rhs_length_of_production(value.state_number);
            printf("we get reduction operation due to %d on %c\n",value.state_number,input[k]);
            for(int i=0;i<2*rhs_length;i++){
                pop();
            }

            int top=getTop();

            struct output another_value=traverseTable(getTop()-'0',gives_lhs_of_production(value.state_number));
            push(gives_lhs_of_production(value.state_number));
            push(another_value.state_number+'0');

        }

        if(getTop()=='1' && input[k]=='$'){
            printf("The string is accepted\n");
            return;
        }
    }
    printf("String is not accepted\n");
}



int main() {

    for(int i=0;i<20;i++){
        table[i].reduction=-1;
    }
    // Define the grammar
    addProduction('S', "AA");
    addProduction('A', "aA");
    addProduction('A', "b");

    // Initial state
    struct State initialState = {.itemCount = 0};
    addItem(&initialState, 'Q', "S", 0);

    states[0]=initialState;
    numStates=1;

    int i=0;
    while(i<numStates){
        closure(&states[i]);
        for(int j=0;j<states[i].itemCount;j++){
            if(states[i].items[j].dotPos<strlen(states[i].items[j].rhs))
                {
                struct State newState=goToFunction(&states[i],states[i].items[j].rhs[states[i].items[j].dotPos]);

                struct Entrie entry;
                entry.state=states[i];
                entry.symbol=states[i].items[j].rhs[states[i].items[j].dotPos];
                entry.newState=newState;
                entries[numEntries]=entry;
                numEntries=numEntries+1;

                if(!stateExists(&newState))
                    {

                        states[numStates]=newState;
                        numStates=numStates+1;
                    }
                }
        }
        i++;
    }

    // for(int i=0;i<numStates;i++){
    //     displayState(&states[i]);
    //     printf("\n");
    // }

    // printf("NumStates: %d\n",numStates)

    // for(int i=0;i<numEntries;i++){
    //     printf("State :\n");
    //     displayState(&entries[i].state);
    //     printf("goes to: \n");
    //     displayState(&entries[i].newState);
    //     printf("by symbol: %c\n",entries[i].symbol);

    //     printf("\n");

    // }

    createTable();
    printTable();

    for(int i=0;i<numTableEntries;i++)
    {   
        // if()
        printf("%d ",table[i].reduction);
    }

    // for(int i=0;i<numStates;i++){
    //     printf("%d\n",states[i].itemCount);
    // }


    // printf("%c\n",(char)3);
    // printf("%d",'0');

    char input[100];
    printf("Enter the string\n");
    scanf("%s",input);
    input[strlen(input)]='$';
    parse(input);



    struct output value=traverseTable(4,'a');
    printf("%d %d\n",value.t_or_nt,value.state_number);

    // printf("%d %c %d\n",table[18].from_state_number,table[18].symbol,table[18].reduction);
    return 0;
}

