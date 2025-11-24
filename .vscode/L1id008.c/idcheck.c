#include "myid.h"

// (c) idcheck.c - 檢查身分證相關程式

/**
 * @brief 驗證台灣國民身分證字號或新式統一證號是否有效。
 * @param id_number 待驗證的身分證字號字串。
 * @return bool: 如果身分證字號有效則回傳 true，否則回傳 false。
 */
bool validateTaiwanID(const char *id_number) {
    
    // 1. 檢查長度
    if (id_number == NULL || strlen(id_number) != ID_LENGTH) {
        return false;
    }

    // 2. 檢查基本格式
    char first_char = id_number[0];
    
    // 檢查第一碼是否為英文字母 (利用 myid.h 引入的 ctype.h 函式)
    if (!isalpha(first_char)) {
        return false;
    }

    // 檢查第二碼是否為性別碼/統號碼 (1/2, 8/9)
    if (id_number[1] != '1' && id_number[1] != '2' && 
        id_number[1] != '8' && id_number[1] != '9') {
        return false;
    }

    // 檢查後八碼是否為數字
    for (int i = 2; i < ID_LENGTH; i++) {
        if (!isdigit(id_number[i])) {
            return false;
        }
    }

    // 3. 轉換第一碼英文字母為數字 (利用 util.c 的函數)
    int first_char_val = getCharValue(first_char);
    if (first_char_val == 0) {
        return false; // 非法戶籍碼 (不應該發生，但作為防禦性檢查)
    }

    int n0_1 = first_char_val / 10; // 十位數
    int n0_2 = first_char_val % 10; // 個位數
    
    // 4. 計算加權總和 S
    // 加權乘數 (從第二碼到第九碼依序為 8, 7, 6, 5, 4, 3, 2, 1)
    const int weights[] = {8, 7, 6, 5, 4, 3, 2, 1};
    
    // 初始化總和：戶籍碼十位數*1 + 戶籍碼個位數*9
    int S = n0_1 * 1 + n0_2 * 9;
    
    // 加上第二碼到第九碼的加權和
    for (int i = 0; i < 8; i++) {
        // id_number[1+i] 是從第二碼 (索引 1) 到第九碼 (索引 8)
        int digit = id_number[1 + i] - '0'; // 字符轉數字
        S += digit * weights[i];
    }
    
    // 5. 驗證檢查碼
    
    int actual_checksum = id_number[9] - '0'; // 第十碼 (檢查碼)
    
    // 計算預期的檢查碼：(10 - (S mod 10)) mod 10
    int remainder = S % 10;
    int expected_checksum = (10 - remainder) % 10;
    
    // 比較預期的檢查碼與實際的檢查碼
    return expected_checksum == actual_checksum;
}
