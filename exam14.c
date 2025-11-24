#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

// ======================= 常數定義 (根據最新要求調整) =======================
#define NUM_STU 10000           // 固定學生人數
#define MAX_SCORE 99            // 最高分數
#define MIN_SCORE 0             // 最低分數
#define PER_ROW 5               // 每個區間的長度 (0..4, 5..9, ...)
#define NUM_INTERVALS ((MAX_SCORE + 1) / PER_ROW) // 區間總數 (100/5 = 20)

// 常態分佈固定參數
#define FIXED_AVG 70.0
#define FIXED_STD 9.8

#define MAX_STARS_PER_LINE 65   // 長條圖每行最大星號數

// ======================= 函數聲明 =======================
int uniSc();
int nmSc(double avg, double std);
double standard_normal_rand();

void calculateAndPrintStats(int n, double sum, double ss2, const char *dist_name);
void printBar(int n, int numRow, const int count[], const char *dist_name);
void testUniform();
void testNormal(double avg, double std);


// ======================= 輔助函數實現 (CLT N(0, 1)) =======================

// 中央極限定理: 生成標準常態分佈 N(0, 1) 隨機數
double standard_normal_rand() {
    double sum = 0.0;
    // 使用 12 個 [0, 1) 均勻隨機數
    for (int i = 0; i < 12; i++) {
        // (rand() + 0.5) / (RAND_MAX + 1.0) 產生 (0.0, 1.0) 之間的浮點數
        // 避免 rand() / RAND_MAX 導致的 1.0 邊界問題
        sum += (double)rand() / (RAND_MAX + 1.0); 
    }
    return sum - 6.0; // sum - 6.0 近似於 N(0, 1)
}

// ======================= 分數生成函數實現 =======================

// 產生 [0, 99] 符合 Uniform distribution 的成績
int uniSc() {
    return rand() % (MAX_SCORE + 1); // 產生 [0, 99] 的整數
}

// 產生符合 N(avg, std*std) 且 [0, 99] 的成績
int nmSc(double avg, double std) {
    double Z = standard_normal_rand();
    double X = avg + std * Z; // 轉換為 N(avg, std*std)

    int score = (int)round(X); // 四捨五入取整數

    // 處理邊界條件 [0, 99]
    if (score < MIN_SCORE) return MIN_SCORE;
    if (score > MAX_SCORE) return MAX_SCORE;
    
    return score;
}

// ======================= 統計與繪圖函數實現 =======================

// 計算並輸出統計數據
void calculateAndPrintStats(int n, double sum, double ss2, const char *dist_name) {
    double avg_calc = sum / n;
    // 變異數 (平方之均 - 均之平方)
    double variance_calc = ss2 / n - avg_calc * avg_calc;   
    
    // 變異數必須大於等於 0，防止浮點數誤差導致微小負值
    if (variance_calc < 0.0) variance_calc = 0.0;
    
    double std_calc = sqrt(variance_calc);

    printf("\n--- %s 分佈 (N=%d) 實際統計結果 ---\n", dist_name, n);
    printf("平均分數: %.2f\n", avg_calc);
    printf("變異數:   %.2f\n", variance_calc);
    printf("標準差:   %.2f\n", std_calc);
    printf("----------------------------------------\n");
}


// 印出長條圖 (橫的)
void printBar(int n, int numRow, const int count[], const char *dist_name) {
    // 找出最大區間人數
    int max_count = 0;
    for (int i = 0; i < numRow; i++) {
        if (count[i] > max_count) {
            max_count = count[i];
        }
    }

    // 計算每個 * 代表的學生人數 (取上限，確保長度不超過 MAX_STARS_PER_LINE)
    int oneStar = (int)ceil((double)max_count / MAX_STARS_PER_LINE);
    if (oneStar < 1) oneStar = 1;     // 防呆

    printf("\n--- %s 成績統計長條圖 (每 * 約 %d 人) ---\n", dist_name, oneStar);
    
    for (int i = 0; i < numRow; i++) {
        int lower = i * PER_ROW;
        int upper = lower + PER_ROW - 1;
        
        // 確保區間上限不超過 MAX_SCORE (即 99)
        if (upper > MAX_SCORE) upper = MAX_SCORE;
        
        int num_students = count[i];
        int num_stars = num_students / oneStar;

        // 格式化輸出區間 [XX..YY]
        printf("[%02d..%02d] (%4d): ", lower, upper, num_students);

        // 繪製長條圖 (限制最大星號數)
        if (num_stars > MAX_STARS_PER_LINE) {
            num_stars = MAX_STARS_PER_LINE;
        }

        for (int j = 0; j < num_stars; j++) {
            printf("*");
        }
        printf("\n");
    }
    printf("------------------------------------------------------------------------------------------------\n");
}

// ------------------------- 均勻分佈測試函數 -------------------------
void testUniform() {
    printf("============= 開始測試 Uniform Distribution =============\n");
    double sum = 0.0;
    double ss2 = 0.0;
    int count[NUM_INTERVALS] = {0}; 
    int n = NUM_STU;

    for (int i = 0; i < n; i++) {
        int score = uniSc();
        
        // 統計區間
        int interval_index = score / PER_ROW;
        if (interval_index < NUM_INTERVALS) {
            count[interval_index]++;
        }
        
        // 累積總和與平方和
        sum += score;
        ss2 += (double)score * score;
    }
    
    // 輸出統計結果
    calculateAndPrintStats(n, sum, ss2, "Uniform");
    
    // 輸出長條圖
    printBar(n, NUM_INTERVALS, count, "Uniform");
    printf("=========================================================\n");
}

// ------------------------- 常態分佈測試函數 -------------------------
void testNormal(double avg, double std) {
    printf("============== 開始測試 Normal Distribution =============\n");
    printf("設定平均: %.1f, 設定標準差: %.1f\n", avg, std);
    
    double sum = 0.0;
    double ss2 = 0.0;
    int count[NUM_INTERVALS] = {0}; 
    int n = NUM_STU;

    for (int i = 0; i < n; i++) {
        int score = nmSc(avg, std);
        
        // 統計區間
        int interval_index = score / PER_ROW;
        if (interval_index < NUM_INTERVALS) {
            count[interval_index]++;
        }
        
        // 累積總和與平方和
        sum += score;
        ss2 += (double)score * score;
    }

    // 輸出統計結果
    calculateAndPrintStats(n, sum, ss2, "Normal");
    
    // 輸出長條圖
    printBar(n, NUM_INTERVALS, count, "Normal");
    printf("=========================================================\n");
}

// ======================= 主程式 (無輸入) =======================
int main() {
    // 初始化隨機數種子
    srand(time(0));  
    
    // 執行 Uniform Distribution 測試 (一次)
    testUniform(); 
    
    // 執行 Normal Distribution 測試 (一次)
    testNormal(FIXED_AVG, FIXED_STD); 
    
    return 0;
}