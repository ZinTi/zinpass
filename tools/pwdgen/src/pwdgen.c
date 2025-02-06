// 随机密码生成器-命令行版
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include "color.h"

// 当前项目信息
#define PROJ_NAME			"PwdGen"
#ifdef _WIN32
    #define PROJ_VER			"0.1-WinNT"
#else
    #define PROJ_VER			"0.1-GNU/Linux"
#endif
#define PROJ_YEARS			"2024-2025"
#define AUTHOR				"Mr. ZENG Lai(Zin)"
#define CONTACT 			"Email: vip201@126.com"

// 定义密码组成成分
#define DIGITS                 "0123456789"
#define UPPERCASE_LETTERS      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define LOWERCASE_LETTERS      "abcdefghijklmnopqrstuvwxyz"
#define SPECIAL_CHARACTERS     "!@#$\x25^&*()_+{}[]|:;<>,.?/~"

TextColor COLOR_PROMPT_General    =	{COLOR_BRIGHT_WHITE, COLOR_NONE};
TextColor COLOR_PROMPT_Error      =	{COLOR_RED, COLOR_NONE};
TextColor COLOR_DIGITS            =	{COLOR_BRIGHT_GREEN, COLOR_NONE};
TextColor COLOR_UPPERCASE         =	{COLOR_BRIGHT_BLUE, COLOR_NONE};
TextColor COLOR_LOWERCASE         =	{COLOR_BRIGHT_CYAN, COLOR_NONE};
TextColor COLOR_SPECIAL           =	{COLOR_BRIGHT_MAGENTA, COLOR_NONE};
TextColor COLOR_SerialNumber      = {COLOR_BLACK, COLOR_BRIGHT_YELLOW};

// 函数声明
void About(void);
void GenerateRandomPwd(char *password, const char *components, int length);
int ColorPrintPwd(char* password);
int GetPwdComponents(char* components);
int GetPwdLength(int* pPwdLength);
int GetPwdCount(int* pPwdCount);

int main(void) {
    About();

    while(1){
        char option;
        printf("Begin? (y/N) > ");
        _label:
        option = (char)getchar();
        if(option == '\n' || option == '\r'){
            goto _label;
        }
        if(option == 'y' || option == 'Y'){
            srand(time(NULL)); // 初始化随机数生成器
            int password_length, password_count;
            char* components = (char*)malloc(100);
            
            // 获取密码组成成分、密码长度和密码个数
            GetPwdComponents(components);
            GetPwdLength(&password_length);
            GetPwdCount(&password_count);
            
            // 生成密码
            for (int i = 0; i < password_count; i++) {
                char password[password_length + 1];
                GenerateRandomPwd(password, components, password_length);
                ColorPrintf(COLOR_SerialNumber, "%02d", i + 1); // 序号
                ColorPrintPwd(password);
            }
            if(components) free(components);
            printf("End\n");
        }else if(option == 'n' || option == 'N'){
            break;
        }else{
            ColorPrintf(COLOR_PROMPT_Error, "Invalid option! Enter 'y' or 'n', please!\n");
        }
    }

    return 0;
}

void About(void){
	printf("%s version %s (compiled %s, %s)\n", PROJ_NAME, PROJ_VER, __TIME__, __DATE__);
	printf("Copyleft (c) %s %s. Licensed under the GNU General Public License.\n%s\n\n", PROJ_YEARS, AUTHOR, CONTACT);
}

int ColorPrintPwd(char* password){
    printf("\t");
    for(int i=0 ; ; i++){
        if(*(password+i) >= '0' && *(password+i) <= '9'){  // 0-9数字
            ColorPrintf(COLOR_DIGITS, "%c",*(password+i));
        }else if(*(password+i) >= 'A' && *(password+i) <= 'Z'){ //A-Z大写字母
            ColorPrintf(COLOR_UPPERCASE, "%c",*(password+i));
        }else if(*(password+i) >= 'a' && *(password+i) <= 'z'){ //a-z小写字母
            ColorPrintf(COLOR_LOWERCASE, "%c",*(password+i));
        }else if(*(password+i) == '\0'){ //字符串结束符
            break;
        }else{ //其他字符
            //*(password+i) > '9' && *(password+i) < 'A' || *(password+i) < '0' || *(password+i) > 'Z' && *(password+i) < 'a' || *(password+i) > 'z'
            ColorPrintf(COLOR_SPECIAL, "%c",*(password+i));
        }
    }
    putchar('\n');

    return 0;
}

int GetPwdComponents(char* components){
    ColorPrintf(COLOR_DIGITS, "D/d\tDigits( "DIGITS" )\n");
    ColorPrintf(COLOR_UPPERCASE, "U/u\tUppercase letters( "UPPERCASE_LETTERS" )\n");
    ColorPrintf(COLOR_LOWERCASE, "L/l\tLowercase letters( "LOWERCASE_LETTERS" )\n");
    ColorPrintf(COLOR_SPECIAL, "S/s\tSpecial characters( "SPECIAL_CHARACTERS" )\n");
    ColorPrintf(COLOR_PROMPT_General, "Enter password components > ");
    if(scanf("%s", components)){};
    // 合法性显示
    char* theUnderline = (char*)malloc(sizeof(components));
    for(int i=0 ; ; i++){
        if(*(components+i) == 'D' || *(components+i) == 'd'){
            ColorPrintf(COLOR_DIGITS, "%c",*(components+i));
            *(theUnderline+i) = ' ';
        }else if(*(components+i) == 'U' || *(components+i) == 'u'){
            ColorPrintf(COLOR_UPPERCASE, "%c",*(components+i));
            *(theUnderline+i) = ' ';
        }else if(*(components+i) == 'S' || *(components+i) == 's'){
            ColorPrintf(COLOR_SPECIAL, "%c",*(components+i));
            *(theUnderline+i) = ' ';
        }else if(*(components+i) == 'L' || *(components+i) == 'l'){
            ColorPrintf(COLOR_LOWERCASE, "%c",*(components+i));
            *(theUnderline+i) = ' ';
        }else if(*(components+i) == '\0'){
            *(theUnderline+i) = '\0';
            break;
        }else{
            ColorPrintf(COLOR_PROMPT_Error, "%c",*(components+i));
            *(theUnderline+i) = '^';
        }
    }
    ColorPrintf(COLOR_PROMPT_Error, "\n%s\n", theUnderline);

    return 0;
}

int GetPwdLength(int* pPwdLength){
    ColorPrintf(COLOR_PROMPT_General, "Enter password length > ");
    if(scanf("%d", pPwdLength)){};
    return 0;
}

int GetPwdCount(int* pPwdCount){
    ColorPrintf(COLOR_PROMPT_General, "Enter number of passwords to generate > ");
    if(scanf("%d", pPwdCount)){};
    return 0;
}

void GenerateRandomPwd(char *password, const char *components, int length) {
    int components_length = 0;
    int x1[4] = {0,0,0,0};
    if (strchr(components, 'D') != NULL || strchr(components, 'd') != NULL){
        components_length += strlen(DIGITS);
        x1[0] = 1;
    }
    if (strchr(components, 'U') != NULL || strchr(components, 'u') != NULL){
        components_length += strlen(UPPERCASE_LETTERS);
        x1[1] = 1;
    }
    if (strchr(components, 'L') != NULL || strchr(components, 'l') != NULL){
        components_length += strlen(LOWERCASE_LETTERS);
        x1[2] = 1;
    }
    if (strchr(components, 'S') != NULL || strchr(components, 's') != NULL){
        components_length += strlen(SPECIAL_CHARACTERS);
        x1[3] = 1;
    }
    for (int i = 0; i < length; i++) {
        size_t index = rand() % components_length;
        if (x1[0] == 1) {
            if (index < strlen(DIGITS)) {
                password[i] = DIGITS[index];
                continue;
            }
            index -= strlen(DIGITS);
        }
        if (x1[1] == 1) {
            if (index < strlen(UPPERCASE_LETTERS)) {
                password[i] = UPPERCASE_LETTERS[index];
                continue;
            }
            index -= strlen(UPPERCASE_LETTERS);
        }
        if (x1[2] == 1) {
            if (index < strlen(LOWERCASE_LETTERS)) {
                password[i] = LOWERCASE_LETTERS[index];
                continue;
            }
            index -= strlen(LOWERCASE_LETTERS);
        }
        if (x1[3] == 1) {
            if (index < strlen(SPECIAL_CHARACTERS)) {
                password[i] = SPECIAL_CHARACTERS[index];
                continue;
            }
            index -= strlen(SPECIAL_CHARACTERS);
        }
    }
    password[length] = '\0'; // 添加字符串结束符
    return;
}
