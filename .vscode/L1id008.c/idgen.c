#include "myid.h"

// (d) idgen.c - 身分證產生器相關程式

/**
 * @brief 根據前九碼的加權和計算出合法的檢查碼 (第十碼)。
 * @param char_value 戶籍碼對應的數字值 (10-35)。
 * @param validation_digits 包含性別碼和七位流水號的字串 (共 8 碼)。
 * @return int: 計算出的檢查碼 (0-9)。
 */
static int calculateChecksumDigit(int char_value, const char *validation_digits) {
    
    int n0_1 = char_value / 10; // 戶籍碼十位數
    int n0_2 = char_value % 10; // 戶籍碼個位數

    // 加權乘數 (從第二碼到第九碼依序為 8, 7, 6, 5, 4, 3, 2, 1)
    const int weights[] = {8, 7, 6, 5, 4, 3, 2, 1};
    
    // 初始化總和 S：戶籍碼十位數*1 + 戶籍碼個位數*9
    int S = n0_1 * 1 + n0_2 * 9;
    
    // 加上第二碼到第九碼的加權和
    for (int i = 0; i < 8; i++) {
        int digit = validation_digits[i] - '0'; // 字符轉數字
        S += digit * weights[i];
    }
    
    // 計算預期的檢查碼：(10 - (S mod 10)) mod 10
    int remainder = S % 10;
    int expected_checksum = (10 - remainder) % 10;
    
    return expected_checksum;
}

/**
 * @brief 根據戶籍地代號和性別產生一組合法的身分證字號。
 * @param id_output 輸出結果的字串緩衝區 (長度需 >= ID_LENGTH + 1)。
 * @param area_code 戶籍地英文字母代號 (A-Z)。
 * @param gender_int 性別代碼 (1:男性, 2:女性; 8/9:新式統號)。
 */
void generateTaiwanID(char id_output[ID_LENGTH + 1], char area_code, int gender_int) {
    char upper_area_code = toupper(area_code);
    char gender_char;
    
    // 1. 處理輸入和前兩碼
    int char_value = getCharValue(upper_area_code);
    if (char_value == 0) {
        // 輸入無效，使用預設值 A1
        char_value = 10; 
        upper_area_code = 'A';
    }

    if (gender_int == 1 || gender_int == 2 || gender_int == 8 || gender_int == 9) {
        gender_char = (char)('0' + gender_int);
    } else {
        gender_char = '1'; // 預設為男性
    }

    // 2. 產生 7 位隨機流水號 (第 3 碼到第 9 碼)
    char random_digits[8]; // 7位數字 + \0
    for (int i = 0; i < 7; i++) {
        random_digits[i] = (char)('0' + (rand() % 10)); // 產生 0-9 隨機數字
    }
    random_digits[7] = '\0';
    
    // 3. 準備計算檢查碼所需的前九碼數據
    char validation_digits[9]; // [性別碼] + [流水號7碼] + \0
    validation_digits[0] = gender_char;
    strncpy(&validation_digits[1], random_digits, 7); 
    validation_digits[8] = '\0';
    
    // 4. 計算檢查碼 (第十碼)
    int checksum = calculateChecksumDigit(char_value, validation_digits);

    // 5. 組合最終 ID
    // 格式: [戶籍碼][性別碼][流水號7碼][檢查碼]
    sprintf(id_output, "%c%c%s%d", upper_area_code, gender_char, random_digits, checksum);
}