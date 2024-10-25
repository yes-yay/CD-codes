//sri rama jayam

#include<stdio.h>
#include<string.h>

char productions[20][20];
int numProds;

char result[20];
// int resultLen=0;

void addToResult(char res[],char c){
    int i=0;
    while(res[i]!='\0'){
        if(res[i]==c)
            return;
        i=i+1;
    }
    res[i]=c;
    res[i+1]='\0';
    // resultLen=resultLen+1;
}

void First(char[],char );

int main(){

    printf("Enter number of productions now: ");
    scanf("%d",&numProds);

    printf("\nEnter productions now: ");
    for(int i=0;i<numProds;i++){
        scanf("%s",productions[i]);
    }

    printf("\nYour productions are: ");
    
    for(int i=0;i<numProds;i++){
        printf("%s\n",productions[i]);
    }

    while(1){
    char c;
    printf("\nEnter element you want to find FIRST of: ");
    scanf(" %c",&c);

    First(result,c);

    printf("\nFIRST(%c) = { ",c);
    for(int i=0;i<strlen(result);i++){
        printf("%c ",result[i]);
    }
    printf("}\n");
    }
}

int isUpper(char c){
    if(c>='A' && c<='Z')
        return 1;
    return 0;
}

void First(char result[],char c){
    
    char subresult[20];
    int foundepsiolon=0;

    result[0]='\0';
    subresult[0]='\0';

    if(!isUpper(c)){
        addToResult(result,c);
        return;
    }
    else{
        for(int i=0;i<numProds;i++){
            if(productions[i][0]==c){
                if(productions[i][2]=='$'){
                    addToResult(result,'$');
                }
                else{
                    int j=2;
                    while(productions[i][j]!='\0'){
                        foundepsiolon=0;
                        First(subresult,productions[i][j]);
                        for(int k=0;k<subresult[k]!='\0';k++){
                            addToResult(result,subresult[k]);
                        }
                        for(int k=0;k<subresult[k]!='\0';k++){
                            // addToResult(result,subresult[k]);
                            if(subresult[k]=='$')
                                {
                                foundepsiolon=1;
                                break;
                                }
                        }

                        if(!foundepsiolon){
                            break;
                        }
                        j++;

                        
                    }
                }
            }
        }
    }

    return;
    
}