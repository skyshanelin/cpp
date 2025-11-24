#include <stdio.h>

// 函式用於計算並印出 0! 到 max_n!
void calculate_and_print_factorials(int max_n) {
    long long current_factorial = 1; // 0! 初始化為 1
    int i;
    int overflow_flag = 0; // 0: 尚未溢位, 1: 第一次溢位已發生
    int first_overflow_n = -1; // 記錄第一次溢位發生的 n 值

    // 從 0 開始計算
    for (i = 0; i <= max_n; i++) {
        
        // 檢查前一次計算 (i-1)! * i 是否導致 i! 溢位
        if (i > 0 && overflow_flag == 0) {
            // 如果 current_factorial 變成負數，表示 i! 溢位了
            if (current_factorial < 0) {
                // 這是第一次溢位，依照要求，在印出這列 (i!) 之前先印出分隔線
                printf("-------\n");
                overflow_flag = 1;      // 設定溢位旗標
                first_overflow_n = i;   // 記錄第一次溢位發生的 n
            }
        }
        
        // 印出當前的 n 和 n!
        // n 用 %d, n! 用 %lld
        printf("%d ! = %lld\n", i, current_factorial);

        // 僅在第一次溢位發生的那一列 (first_overflow_n) 之後印出 =======
        if (i == first_overflow_n) {
             printf("=======\n");
        }
        
        // 準備計算下一個階乘 (i+1)!
        if (i < max_n) {
            // 即使溢位了，也繼續計算
            current_factorial *= (i + 1);
        }
    }
}

int main() {
    calculate_and_print_factorials(52);
    return 0;
}