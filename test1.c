#include<stdio.h>
#include<string.h>
int main(){
    char input[10];
    while(1){
        scanf("%s",input);
        if(strcmp(input,"Dian")==0)
           { printf("2002\n");} 
       else if (strcmp(input,"Quit")==0)
       {break;}
       else 
       {printf("Error\n");}
    
}
    return 0 ;
     
}
    
