#ifndef MYID_H
#define MYID_H

// (a) myid.h - 包含所有需要的宣告和標準函式庫
// 根據要求，所有 .c 檔案只會 include 此檔案，因此標準函式庫需在此引入。
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>    // 用於產生器設定隨機種子

// 2. 常數定義
#define ID_LENGTH 10    // 身分證字號標準長度
#define MAX_BUFFER 20   // 緩衝區大小 (容納 10 碼 + 換行 + 結束符)

// 3. 戶籍地代號資訊 (用於產生器的提示)
typedef struct {
    char code;
    const char *name;
} AreaInfo;

// 戶籍地代號表
static const AreaInfo AREA_CODES[] = {
    {'A', "台北市"}, {'B', "台中市"}, {'C', "基隆市"}, {'D', "台南市"},
    {'E', "高雄市"}, {'F', "新北市"}, {'G', "宜蘭縣"}, {'H', "桃園市"},
    {'I', "嘉義市"}, {'J', "新竹縣"}, {'K', "苗栗縣"}, {'L', "台中縣"},
    {'M', "南投縣"}, {'N', "彰化縣"}, {'O', "新竹市"}, {'P', "雲林縣"},
    {'Q', "嘉義縣"}, {'R', "台南縣"}, {'S', "高雄縣"}, {'T', "屏東縣"},
    {'U', "花蓮縣"}, {'V', "台東縣"}, {'W', "金門縣"}, {'X', "澎湖縣"},
    {'Y', "陽明山"}, {'Z', "連江縣"}
};
#define NUM_AREA_CODES (sizeof(AREA_CODES) / sizeof(AREA_CODES[0]))


// 4. 函數原型宣告 (Function Prototypes)

// (c) idcheck.c 檢查身分證相關程式
bool validateTaiwanID(const char *id_number);

// (d) idgen.c 身分證產生器相關程式
void generateTaiwanID(char id_output[ID_LENGTH + 1], char area_code, int gender_int);

// (e) util.c 工具函數
int getCharValue(char c);

#endif // MYID_H
