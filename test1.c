#include<stdio.h>
#include<string.h>
int main(){
    //定义1个叫input的数组，容量为10
    char input[10];
    //构建了一个无限循环体。分别处理输入Dian，Quit，以及其他情况
    while(1){
        scanf("%s",input);
        if(strcmp(input,"Dian")==0 )
        { printf("2002\n");} 
       else if (strcmp(input,"Quit")==0)
       {break;}
       else 
       {printf("Error\n");}
    
}
    return 0 ;
     
}
    
