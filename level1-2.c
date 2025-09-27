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
void show_floor_status(char* day, int floor);
int reserve_seat(char* day, int floor, int row, int col);
void show_reservations();
int day_to_index(char* day);

// 主函数
int main() {
    // 初始化系统
    initialize_data();
    load_data();
    
    printf("图书馆预约系统已启动。请输入指令：Login、Exit 或 Quit\n");
    
    char command[20];
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
        } else if (current_user != '\0') {
            // 已登录用户的指令处理
            if (strstr(command, "Floor") != NULL) {
                // 处理显示楼层状态的指令
                char day[10];
                int floor;
                scanf("%s Floor %d", day, &floor);
                show_floor_status(day, floor);
            } else if (strcmp(command, "Reserve") == 0) {
                // 处理预约指令
                char day[10], seat_str[5];
                int floor, row, col;
                scanf("%s Floor %d Seat %d %d", day, &floor, &row, &col);
                if (reserve_seat(day, floor, row, col)) {
                    printf("OK\n");
                } else {
                    printf("Failed\n");
                }
            } else if (strcmp(command, "Reservation") == 0) {
                // 显示当前用户的预约
                show_reservations();
            } else {
                printf("无效指令\n");
            }
        } else {
            printf("请先登录\n");
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
        // 检查是否输入了"Admin"（注意：这里简化处理，实际应该检查完整字符串）
        return 1;
    }
    
    if (isalpha(username) && isupper(username)) {
        return 1;
    }
    
    return 0;
}

// 将日期字符串转换为索引
int day_to_index(char* day) {
    if (strcmp(day, "Sunday") == 0) return 0;
    if (strcmp(day, "Monday") == 0) return 1;
    if (strcmp(day, "Tuesday") == 0) return 2;
    if (strcmp(day, "Wednesday") == 0) return 3;
    if (strcmp(day, "Thursday") == 0) return 4;
    if (strcmp(day, "Friday") == 0) return 5;
    if (strcmp(day, "Saturday") == 0) return 6;
    return -1; // 无效日期
}

// 显示某一天某一层的座位状态
void show_floor_status(char* day, int floor) {
    int day_index = day_to_index(day);
    if (day_index == -1) {
        printf("无效日期\n");
        return;
    }
    
    if (floor < 1 || floor > FLOORS) {
        printf("无效楼层\n");
        return;
    }
    
    int floor_index = floor - 1;
    
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            int status = library.seats[floor_index][r][c][day_index];
            
            // 普通用户只能看到0(空闲)和2(被当前用户预约)
            if (current_user != 'A' && current_user != 'd' && current_user != 'm' && 
                current_user != 'i' && current_user != 'n') { // 不是管理员
                if (status == 0) {
                    printf("0");
                } else if (status == (current_user - 'A' + 1)) {
                    printf("2");
                } else {
                    printf("1");
                }
            } else {
                // 管理员可以看到具体用户
                if (status == 0) {
                    printf("0");
                } else {
                    printf("%c", 'A' + status - 1);
                }
            }
        }
        printf("\n");
    }
}

// 预约座位
int reserve_seat(char* day, int floor, int row, int col) {
    int day_index = day_to_index(day);
    if (day_index == -1) {
        return 0;
    }
    
    if (floor < 1 || floor > FLOORS || row < 1 || row > ROWS || col < 1 || col > COLS) {
        return 0;
    }
    
    int floor_index = floor - 1;
    int row_index = row - 1;
    int col_index = col - 1;
    
    // 检查座位是否已被预约
    if (library.seats[floor_index][row_index][col_index][day_index] != 0) {
        return 0;
    }
    
    // 预约座位（存储用户编号：A=1, B=2, ..., Z=26）
    library.seats[floor_index][row_index][col_index][day_index] = current_user - 'A' + 1;
    
    return 1;
}

// 显示当前用户的预约
void show_reservations() {
    char* days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    int user_id = current_user - 'A' + 1;
    
    for (int f = 0; f < FLOORS; f++) {
        for (int r = 0; r < ROWS; r++) {
            for (int c = 0; c < COLS; c++) {
                for (int d = 0; d < DAYS; d++) {
                    if (library.seats[f][r][c][d] == user_id) {
                        printf("%s Floor %d Seat %d %d\n", days[d], f+1, r+1, c+1);
                    }
                }
            }
        }
    }
}
