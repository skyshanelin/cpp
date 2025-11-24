#include "myid.h"

// (e) util.c - 工具函數實現

/**
 * @brief 將身分證字號的第一個字母 (A-Z) 轉換為其對應的兩位數字值 (10-35)。
 * @param c 英文字母 (大小寫皆可)。
 * @return int: 對應的數字值，如果非法則返回 0。
 */
int getCharValue(char c) {
    c = toupper(c); // 確保是大寫
    
    // 根據台灣戶籍地代碼轉換規則
    if (c >= 'A' && c <= 'H') {
        return (int)(c - 'A' + 10);
    } 
    else if (c == 'I') {
        return 34;
    } 
    else if (c >= 'J' && c <= 'N') {
        return (int)(c - 'J' + 18);
    } 
    else if (c == 'O') {
        return 35;
    } 
    else if (c >= 'P' && c <= 'T') {
        return (int)(c - 'P' + 23);
    }
    else if (c >= 'U' && c <= 'V') {
        return (int)(c - 'U' + 28);
    }
    else if (c == 'W') {
        return 32;
    } 
    else if (c >= 'X' && c <= 'Y') {
        return (int)(c - 'X' + 30);
    }
    else if (c == 'Z') {
        return 33;
    }
    
    return 0; // 非法字符
}
