// sri rama jayam

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Production {
    char lhs;
    char rhs[20];
};

struct Production grammar[20];
int numProductions = 0;

void addProduction(char lhs, char rhs[]) {
    grammar[numProductions].lhs = lhs;
    strcpy(grammar[numProductions].rhs, rhs);
    numProductions++;
}

struct Production deleteProduction(int productionNumber) {
    struct Production deleted_production = grammar[productionNumber];
    for (int i = productionNumber; i < numProductions - 1; i++) {
        grammar[i] = grammar[i + 1];
    }
    numProductions--;  // Move decrement outside loop
    return deleted_production;
}

void printGrammar() {
    for (int i = 0; i < numProductions; i++) {
        printf("%c->%s\n", grammar[i].lhs, grammar[i].rhs);
    }
}

int isLeftRecursive(struct Production production) {
    return production.lhs == production.rhs[0];
}

char non_lr_productions[20][20];
int non_lr_count = 0;

void addToNonLRProductions(char c) {
    int i = 0;
    while (i < numProductions) {
        if (grammar[i].lhs == c && !isLeftRecursive(grammar[i])) {
            strcpy(non_lr_productions[non_lr_count++], grammar[i].rhs);
            deleteProduction(i);  // Delete this production after copying to non-LR
        } else {
            i++;
        }
    }
}

char lr_productions[20][20];
int lr_count = 0;

void addToLRProductions(char c) {
    int i = 0;
    while (i < numProductions) {
        if (grammar[i].lhs == c && isLeftRecursive(grammar[i])) {
            strcpy(lr_productions[lr_count++], grammar[i].rhs);
            deleteProduction(i);  // Delete this production after copying to LR
        } else {
            i++;
        }
    }
}

char extraElement = 'X';

int main() {
    addProduction('A', "Aa");
    addProduction('A', "b");
    addProduction('A', "c");
    addProduction('A',"Ad");

    int i=0;
    while(i<numProductions){
    //for (int i = 0; i < numProductions; i++) {
        if (isLeftRecursive(grammar[i])) {
            addToNonLRProductions(grammar[i].lhs);
            struct Production deleted_production = deleteProduction(i);

            for (int j = 0; j < non_lr_count; j++) {
                struct Production newProduction;
                newProduction.lhs = grammar[i].lhs;
                snprintf(newProduction.rhs, sizeof(newProduction.rhs), "%s%c", non_lr_productions[j], extraElement);
                addProduction(newProduction.lhs, newProduction.rhs);
            }

            struct Production newProduction;
            newProduction.lhs = extraElement;
            snprintf(newProduction.rhs, sizeof(newProduction.rhs), "%s%c", deleted_production.rhs + 1, extraElement);
            addProduction(newProduction.lhs, newProduction.rhs);
            addProduction(deleted_production.lhs,"#");

            for(int j=0;j<lr_count;j++){
                struct Production newProduction;
                newProduction.lhs = extraElement;
                snprintf(newProduction.rhs, sizeof(newProduction.rhs), "%s%c", deleted_production.rhs + 1, extraElement);
                addProduction(newProduction.lhs, newProduction.rhs);
                addProduction(deleted_production.lhs,"#");
            }
        }
        else
            i++;
        // extraElement++;
        
    }

    printGrammar();
    return 0;
}
