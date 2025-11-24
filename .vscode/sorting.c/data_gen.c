#include "myhwk09to11.h"

// 簡化版中文字符集，以字節表示
const char *family_names[] = { "李", "王", "張", "劉", "陳", "楊", "黃" };
const char *compound_family_names[] = { "歐陽", "司徒", "諸葛" };
const char *given_names[] = { "華", "偉", "敏", "芳", "傑", "靜", "豪", "翔", "佳", "倫", "翰", "恩", "軒" };

const int family_names_count = sizeof(family_names) / sizeof(family_names[0]);
const int compound_family_names_count = sizeof(compound_family_names) / sizeof(compound_family_names[0]);
const int given_names_count = sizeof(given_names) / sizeof(given_names[0]);

// (a) 產生中文姓名 (至少兩字至多四字)
void generate_name(char *name_buffer, size_t max_len) {
    // 0: 兩字 (姓1+名1), 1: 三字 (姓1+名2), 2: 四字 (複姓2+名2)
    int name_length_type = rand() % 3; 

    name_buffer[0] = '\0';
    
    // 選擇姓氏 (1-2 字)
    int is_compound = 0;
    if (name_length_type == 2 && (rand() % 4 == 0)) { // 複姓機率設低一點 (25%)
        // 複姓 (4字: 複姓2+名2)
        strcat(name_buffer, compound_family_names[rand() % compound_family_names_count]);
        is_compound = 1;
    } else {
        // 單姓 (2字: 姓1+名1 或 3字: 姓1+名2)
        strcat(name_buffer, family_names[rand() % family_names_count]);
    }

    // 選擇名
    int given_len;
    if (!is_compound) {
        // 單姓
        if (name_length_type == 0) given_len = 1; // 兩字 (姓1+名1)
        else given_len = 2; // 三字 (姓1+名2)
    } else {
        // 複姓
        given_len = 2; // 四字 (複姓2+名2)
    }

    // 選擇名 (1 or 2 字)
    for (int i = 0; i < given_len; i++) {
        // 確保不會溢出緩衝區
        if (strlen(name_buffer) + strlen(given_names[0]) + 1 > max_len) break;
        strcat(name_buffer, given_names[rand() % given_names_count]);
    }
}

// (b) 使用 CLT (中央極限定理) 產生近似常態分佈的成績
// 策略: 12 個 rand() 的和 (平均 6.0, 標準差 1.0)
float generate_normal_score(float mean, float stdDev) {
    int num_samples = 12; 
    float sum_of_uniform = 0.0;

    for (int i = 0; i < num_samples; i++) {
        // 產生 0.0 到 1.0 之間的均勻亂數
        sum_of_uniform += (float)rand() / (float)RAND_MAX;
    }

    // 轉換為標準常態分佈 (平均 0.0, 標準差 1.0)
    float standard_normal = sum_of_uniform - 6.0;

    // 縮放和平移到所需的平均數和標準差
    float score = mean + standard_normal * stdDev;

    // 將成績限制在 0 到 100 之間
    if (score < 0.0) score = 0.0;
    if (score > 100.0) score = 100.0;

    // 轉為整數 (使用 round 進行四捨五入)
    return round(score);
}

// (a), (b) 產生所有學生資料
void generate_data(Student *students, int count) {
    long long baseID = 1140000000LL; // 114 開頭的 10 位數字學號

    for (int i = 0; i < count; i++) {
        // (a) 產生學號 (不可重複, 用順序產生)
        students[i].studentID = baseID + i;

        // (a) 產生中文姓名
        generate_name(students[i].chineseName, MAX_NAME_LEN);

        // (b) 產生三科目成績
        for (int j = 0; j < nSubj; j++) {
            // 使用常態分佈亂數產生
            students[i].scores[j] = (int)generate_normal_score(expMean[j], expStd[j]);
        }
    }
}