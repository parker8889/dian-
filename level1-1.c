#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FLOORS 5
#define ROWS 4
#define COLS 4
#define DAYS 7
#define MAX_USERS 27
#define FILENAME "library_data.txt"

// 数据结构
typedef struct {
    int seats[FLOORS][ROWS][COLS][DAYS];
    char users[MAX_USERS];
    int user_count;
} LibrarySystem;

LibrarySystem library;
char current_user = '\0';

// 函数声明
void initialize_data();
void load_data();
void save_data();
int login(char username);
void logout();
int is_valid_user(char username);

int main() {
    // 初始化系统
    initialize_data();
    load_data();
    
    printf("图书馆预约系统已启动。请输入指令：Login、Exit 或 Quit\n");
    
    char command[10];
    while (1) {
        printf("> ");
        scanf("%s", command);
        
        if (strcmp(command, "Login") == 0) {
            char username;
            printf("请输入用户名: ");
            scanf(" %c", &username);
            
            if (login(username)) {
                printf("登录成功\n");
            } else {
                printf("用户名无效，请输入 A-Z 或 Admin\n");
            }
        } else if (strcmp(command, "Exit") == 0) {
            if (current_user != '\0') {
                logout();
                printf("退出当前登录成功\n");
            } else {
                printf("当前未登录\n");
            }
        } else if (strcmp(command, "Quit") == 0) {
            save_data();
            printf("退出程序\n");
            break;
        } else {
            printf("无效指令，请输入 Login、Exit 或 Quit\n");
        }
    }
    
    return 0;
}

// 初始化数据
void initialize_data() {
    // 初始化所有座位状态为0（空闲）
    for (int f = 0; f < FLOORS; f++) {
        for (int r = 0; r < ROWS; r++) {
            for (int c = 0; c < COLS; c++) {
                for (int d = 0; d < DAYS; d++) {
                    library.seats[f][r][c][d] = 0;
                }
            }
        }
    }
    
    // 初始化用户列表
    library.user_count = 0;
    for (char c = 'A'; c <= 'Z'; c++) {
        library.users[library.user_count++] = c;
    }
}

// 加载数据
void load_data() {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        return; // 文件不存在，使用默认初始化数据
    }
    
    // 读取座位状态
    for (int f = 0; f < FLOORS; f++) {
        for (int r = 0; r < ROWS; r++) {
            for (int c = 0; c < COLS; c++) {
                for (int d = 0; d < DAYS; d++) {
                    fscanf(file, "%d", &library.seats[f][r][c][d]);
                }
            }
        }
    }
    
    fclose(file);
}

// 保存数据
void save_data() {
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("无法保存数据到文件\n");
        return;
    }
    
    // 保存座位状态
    for (int f = 0; f < FLOORS; f++) {
        for (int r = 0; r < ROWS; r++) {
            for (int c = 0; c < COLS; c++) {
                for (int d = 0; d < DAYS; d++) {
                    fprintf(file, "%d ", library.seats[f][r][c][d]);
                }
                fprintf(file, "\n");
            }
        }
    }
    
    fclose(file);
}

// 用户登录
int login(char username) {
    if (is_valid_user(username)) {
        current_user = username;
        return 1;
    }
    return 0;
}

// 退出登录
void logout() {
    current_user = '\0';
}

// 检查用户是否有效
int is_valid_user(char username) {
    if (username == 'A' || username == 'd' || username == 'm' || username == 'i' || username == 'n') {
        // 检查是否输入了"Admin"
        return 1;
    }
    
    if (isalpha(username) && isupper(username)) {
        return 1;
    }
    
    return 0;
}
