#include "myid.h"
#include "idcheck.c"
#include "idgen.c"
#include "util.c"

// (b) main.c - 主程式，處理使用者介面和流程控制

/**
 * @brief 清除輸入緩衝區，以避免 scanf 或 fgets 讀取換行符造成問題。
 */
static void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

/**
 * @brief 顯示戶籍地代號列表給使用者選擇。
 */
static void displayAreaCodes() {
    printf("\n--- 戶籍地代號 (請輸入 A-Z) ---\n");
    for (size_t i = 0; i < NUM_AREA_CODES; i++) {
        printf("%c: %-10s", AREA_CODES[i].code, AREA_CODES[i].name);
        if ((i + 1) % 4 == 0) {
            printf("\n");
        }
    }
    printf("\n----------------------------------\n");
}

/**
 * @brief 處理身分證檢查功能。
 */
static void handleCheckID() {
    char input_buffer[MAX_BUFFER];
    char id_number[ID_LENGTH + 1];
    size_t len;

    printf("\n--- 身分證字號檢查模式 ---\n");
    printf("請輸入 %d 碼身分證字號或統一證號: ", ID_LENGTH);
    
    if (fgets(input_buffer, MAX_BUFFER, stdin) == NULL) {
        printf("讀取輸入失敗。\n");
        return;
    }

    // 淨化輸入：移除可能的換行符
    len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
        len--;
    }

    // 複製並轉為大寫，同時檢查長度
    if (len != ID_LENGTH) {
        printf("驗證結果: 長度錯誤 (輸入 %zu 碼，應為 %d 碼)。\n", len, ID_LENGTH);
        return;
    }

    for (int i = 0; i < len; i++) {
        id_number[i] = toupper(input_buffer[i]);
    }
    id_number[len] = '\0';

    if (validateTaiwanID(id_number)) {
        printf("\n✅ 身分證字號 %s 驗證結果: 合法。\n", id_number);
    } else {
        printf("\n❌ 身分證字號 %s 驗證結果: 不合法。\n", id_number);
    }
}

/**
 * @brief 處理身分證產生功能。
 */
static void handleGenerateID() {
    char area_char_input[MAX_BUFFER];
    int gender_int;
    char area_code_char;
    char generated_id[ID_LENGTH + 1];

    printf("\n--- 身分證字號產生模式 ---\n");
    
    // 1. 選擇戶籍地
    displayAreaCodes();
    do {
        printf("請輸入戶籍地代號 (單一英文字母 A-Z): ");
        if (fgets(area_char_input, MAX_BUFFER, stdin) == NULL) return;
        
        // 檢查輸入是否為單一字母
        if (area_char_input[0] != '\n' && 
            (area_char_input[1] == '\n' || area_char_input[1] == '\0')) {
            
            area_code_char = toupper(area_char_input[0]);
            if (getCharValue(area_code_char) != 0) {
                break;
            }
        }
        printf("輸入錯誤。請輸入有效的 A-Z 戶籍地代號。\n");
    } while(1);

    // 2. 選擇性別
    do {
        printf("請選擇性別/統號代碼 (1:男, 2:女, 8:統號男, 9:統號女): ");
        if (scanf("%d", &gender_int) != 1) {
            clearInputBuffer();
            printf("輸入錯誤。請輸入數字 1, 2, 8 或 9。\n");
            continue;
        }
        clearInputBuffer(); // 清理換行符
        
        if (gender_int == 1 || gender_int == 2 || gender_int == 8 || gender_int == 9) {
            break;
        }
        printf("輸入錯誤。請輸入 1, 2, 8 或 9。\n");
    } while(1);

    // 3. 產生身分證
    generateTaiwanID(generated_id, area_code_char, gender_int);

    printf("\n============================================\n");
    printf("✅ 成功產生合法身分證字號 (戶籍碼: %c, 代碼: %d):\n", area_code_char, gender_int);
    printf("   -> %s\n", generated_id);
    printf("============================================\n");
}


int main() {
    // 初始化隨機種子 (用於產生器)
    srand((unsigned int)time(NULL));

    int choice;
    
    do {
        printf("\n============================================\n");
        printf("  台灣身分證字號檢查與產生器 (C 語言)\n");
        printf("============================================\n");
        printf("請選擇功能:\n");
        printf("1. 檢查身分證字號 (驗證合法性)\n");
        printf("2. 產生身分證字號 (依據戶籍地和性別)\n");
        printf("3. 退出程式\n");
        printf("輸入您的選擇 (1, 2 或 3): ");
        
        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            choice = 0; // 設為無效選擇
        } else {
            clearInputBuffer(); // 清理換行符
        }
        
        switch (choice) {
            case 1:
                handleCheckID();
                break;
            case 2:
                handleGenerateID();
                break;
            case 3:
                printf("\n程式結束。謝謝使用！\n");
                return 0;
            default:
                printf("\n[錯誤] 無效的選擇，請重新輸入。\n");
                break;
        }
    } while (choice != 3);

    return 0;
}
