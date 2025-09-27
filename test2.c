#include <stdio.h>
#include <string.h>
//宏定义 定义了相关的变量
#define MAX_ENTRIES 100
#define KEY_SIZE 10
#define VALUE_SIZE 10
//结构体的定义，将键和值绑到一个键值对中
typedef struct {
    char key[KEY_SIZE];
    char value[VALUE_SIZE];
} Pair;
// 一些变量的定义
int main() {
    char filename[100];
    char input[KEY_SIZE];
    Pair pairs[MAX_ENTRIES];
    int count = 0;
// 输入文件名  
    printf("输入文件名: ");
    scanf("%99s", filename);
//打开文件以及打不开时的处理
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("无法打开文件\n");
        return 1;
    }
// 设置一个循环体逐行读取文件，并且保证不超过最大键值对数
    char line[50];
    while (fgets(line, sizeof(line), file) && count < MAX_ENTRIES) 
    {
// 检查行首是否为空字符或者换行符以达到处理空行的目的     
        if (line[0] == '\n' || line[0] == '\0') continue;
        
// 查找冒号，如果找不到就跳过这一行       
        char *colon = strchr(line, ':');
        if (!colon) continue;

 // 对冒号进行处理，将冒号用空字符进行替换，以便于接下来对键和值的分别读取和复制，此后将冒号前的部分复制到键中并且留一个位置给空字符。
//并且确保字符串正确终止。       
        *colon = '\0';
        strncpy(pairs[count].key, line, KEY_SIZE-1);
        pairs[count].key[KEY_SIZE-1] = '\0';
        
// 将冒号之后的部分复制到默认数组值中，并且留空位给空字符，且保证正确终止
        strncpy(pairs[count].value, colon+1, VALUE_SIZE-1);
        pairs[count].value[VALUE_SIZE-1] = '\0';
        
// 处理值中的换行符，是做出的结果更符合要求并进行计数       
        char *nl = strchr(pairs[count].value, '\n');
        if (nl) *nl = '\0';
        
        count++;
    }
// 循环体结束，关闭文件
    fclose(file);
    
//处理用户查询端   
    printf("输入键查询(输入Quit退出): ");
// 构建另一个循环体，处理用户查询
while (scanf("%10s", input) && strcmp(input, "Quit") != 0) 
{
 //初始化找到标记，进行遍历键，并与用户输入进行比较
        int found = 0;
        for (int i = 0; i < count; i++) {
            if (strcmp(pairs[i].key, input) == 0) {
                printf("%s\n", pairs[i].value);
                found = 1;
                break;
            }
        }
    //处理找不到的情况
        if (!found) printf("Error\n");

        
        printf("输入键查询(输入Quit退出): ");
    }
    
    return 0;
}
