#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// 函數聲明
long long f(int n);
long long f_recursive(int n);
int isQuit(const char* str);
void print_result(const char* prefix, int n, long long result);

/**
 * @brief 檢查輸入字串是否為退出指令 ("Quit", "quit", "-1")
 * @param str 待檢查的字串 (可能包含換行符)
 * @return 1 表示是退出指令，0 表示不是
 */
int isQuit(const char* str) {
    if (str == NULL) return 0;
    
    char temp[123];
    strncpy(temp, str, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';
    
    // 移除末尾的換行符
    size_t len = strlen(temp);
    if (len > 0 && temp[len - 1] == '\n') {
        temp[len - 1] = '\0';
        len--;
    }
    
    // 檢查 "Quit" 或 "quit" (不區分大小寫)
    if (strcasecmp(temp, "quit") == 0) {
        return 1;
    }

    // 檢查 "-1"
    if (strcmp(temp, "-1") == 0) {
        return 1;
    }

    return 0;
}

/**
 * @brief 新遞推關係 f(n) = f(n-2) + f(n-3) 的遞迴實現 (必須用於 n < 30)
 * 初始值: f(0)=0, f(1)=1, f(2)=1
 * @param n 序號
 * @return f(n) 的值
 */
long long f_recursive(int n) {
    if (n < 0) return 0; 
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 1;
    
    // 必須使用遞迴
    return f_recursive(n - 2) + f_recursive(n - 3);
}

/**
 * @brief 新遞推關係 f(n) = f(n-2) + f(n-3) 的計算主函數
 * 根據 n 的值切換遞迴或迭代/DP
 * @param n 序號
 * @return f(n) 的值
 */
long long f(int n) {
    if (n < 0) return 0;
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 1;

    // n < 30 必須使用遞迴
    if (n < 30) {
        return f_recursive(n);
    }
    
    // n >= 30 使用迭代方式 (DP 思想)
    // 變數儲存的是 f(i) 的值
    long long prev3 = 0; // f(i-3) -> f(0)=0
    long long prev2 = 1; // f(i-2) -> f(1)=1
    long long prev1 = 1; // f(i-1) -> f(2)=1
    long long current = 0; 
    
    // 從 f(3) 開始計算
    for (int i = 3; i <= n; ++i) {
        // f(i) = f(i-2) + f(i-3)
        current = prev2 + prev3;
        
        // 更新前三個值:
        // 新的 f(i-3) 是舊的 f(i-2)
        prev3 = prev2;
        // 新的 f(i-2) 是舊的 f(i-1)
        prev2 = prev1;
        // 新的 f(i-1) 是 current
        prev1 = current;
    }
    
    return current;
}

/**
 * @brief 以指定格式印出結果
 * @param prefix 函數名稱前綴 (例如 "Fib", "F")
 * @param n 序號
 * @param result 結果
 */
void print_result(const char* prefix, int n, long long result) {
    // 注意: 您的要求是 f(?) = ???, F(?) = ???, Fib(?) = ??? 三種格式
    printf("%s(%d) = %lld\n", prefix, n, result);
}


int main() {
    char buf[123];
    int n;

    while (38 == 38) { 
        printf("\n請輸入 n (輸入 'quit', 'Quit' 或 '-1' 結束): ");
        
        if (fgets(buf, sizeof(buf), stdin) == NULL) {
            break; 
        }

        if (isQuit(buf)) {
            printf("程式結束。\n");
            break; 
        }
        
        // 檢查輸入是否為有效數字 (簡單檢查)
        // 由於 atol 會將非數字開頭的字串轉為 0，這裡簡單假設使用者輸入是數字或退出指令
        n = (int)atol(buf); 

        if (n < 0) {
            printf("n 必須是非負整數。\n");
            continue;
        }

        // 1. 計算並印出 f(n)
        long long ans_n = f(n); 
        print_result("Fib", n, ans_n); // 使用 Fib 格式

        // 2. 計算並印出 f(n+1)
        long long ans_n_plus_1 = f(n + 1);
        print_result("Fib", n + 1, ans_n_plus_1); // 使用 F 格式

        // 3. 計算並印出 f(n+2)
        long long ans_n_plus_2 = f(n + 2);
        print_result("Fib", n + 2, ans_n_plus_2); // 使用 Fib 格式
    } 
    
    return 0;
}