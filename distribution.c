#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h> // for memset

// (2) 控制隨機數種子
// *** 每次測試請手動修改此值 ***
// 奇數次測試 (1, 3, 5): 設為 0 (不使用 srand)
// 偶數次測試 (2, 4, 6): 設為 1 (使用 srand)
#define USE_SRAND 0    // 0 表示不用, 1 表示要用 srand(time(0))

// (5) 程式定義
#define MIN_STU  500    // 最少人次
#define MAX_STU  34567  // 最多
#define MAX_HIST_COLS 65 // 長條圖最多印出的字符數
#define PER_ROW  4  // 每 4 分算一個區間: 0..3, 4..7, ...
#define NUM_SCORE_MAX 100 // 最高分數 (0..99 共 100 個)

// 計算區間數量: (100 / 4) = 25
const int numRow = NUM_SCORE_MAX / PER_ROW; 
int count[NUM_SCORE_MAX / PER_ROW];  // 統計次數用, [0] 到 [24] 共 25 個

// =======================================================
// (1) 函數定義
// =======================================================

// 產生 [0, 99] 符合 Uniform distribution
int uniSc( ) {
    // 產生 [0, RAND_MAX] 的隨機數，然後縮放到 [0, 99]
    // 由於 rand() 產生的數值是 [0, RAND_MAX]，
    // (rand() / (RAND_MAX + 1.0)) 得到 (0, 1) 的浮點數
    // (int)((double)rand() / (RAND_MAX + 1.0) * 100.0) 得到 [0, 99]
    return rand() % 100;
}

// 產生符合 N(avg, std*std) 且[0,99]的常態分佈成績
int nmSc(double avg, double std) {
    // 使用 Box-Muller 轉換生成常態分佈隨機數
    // 這是 Box-Muller 演算法的一個常用實現 (Polar Form)
    static int phase = 0;
    static double z2;
    double z1;

    if (phase == 0) {
        double u1, u2;
        // 產生 (0, 1] 之間的均勻分佈隨機數
        do {
            u1 = rand() / (RAND_MAX + 1.0);
            u2 = rand() / (RAND_MAX + 1.0);
        } while (u1 <= 1e-6); // 避免 log(0)

        // Box-Muller 轉換
        double R = sqrt(-2.0 * log(u1));
        double theta = 2.0 * M_PI * u2; // M_PI 定義在 math.h

        z1 = R * cos(theta);
        z2 = R * sin(theta);
        phase = 1; // 下次返回 z2

    } else {
        z1 = z2;
        phase = 0; // 下次重新計算
    }

    // 將標準常態分佈 (Z) 映射到 N(avg, std*std) 的 X
    double score = avg + (z1 * std);

    // 限制分數在 [0, 99] 之間 (截斷處理)
    if (score < 0.0) {
        return 0;
    } else if (score > 99.0) {
        return 99;
    }

    // 四捨五入到最近的整數
    return (int)round(score);
}


// (4) 繪製橫向長條圖的函數
int printBar(int n, int perStar, int numRow, int cnt[]) {
    printf("\n📊 橫向長條圖 (Bar Chart)\n");
    printf("Total students n = %d, each '*' represents %d counts.\n", n, perStar);
    
    // 計算半個星號代表的次數
    int halfStar = (perStar + 1) / 2;
    if (halfStar < 1) halfStar = 1; // 至少為 1
    
    int i, numStars, remainder;
    for(i = 0; i < numRow; ++i) {
        // 區間範圍 (例如 0..3) 和實際次數 (例如: 50)
        printf("%2d..%2d:%5d |", i*PER_ROW, i*PER_ROW + (PER_ROW - 1), cnt[i]);
        
        // 計算星號數量
        numStars = cnt[i] / perStar;
        
        // 計算剩餘次數 (用來判斷是否印 '.' )
        remainder = cnt[i] % perStar;
        
        // 根據題目要求，判斷是否四捨五入:
        // 如果剩餘次數大於或等於半個星號代表的次數，則星號數量 + 1
        if (remainder >= halfStar) {
            numStars++;
            remainder = 0; // 已進位，不需再印點
        } else if (remainder > 0 && numStars == 0) {
            // 如果次數很低 (例如 1..halfStar-1)，且沒有星號，印一個點 '.'
            // 這是題目舉例 1 |. 的情況
            remainder = 1; // 設為 1 標記印點
        } else {
            remainder = 0; // 其他情況不印點
        }

        // 限制最多印出 MAX_HIST_COLS 個字符，避免換行
        if (numStars > MAX_HIST_COLS) {
            numStars = MAX_HIST_COLS;
            remainder = 0; // 超過限制就不印點了
        }

        // 1. 印星號 '*'
        for (int j = 0; j < numStars; ++j) {
            printf("*");
        }

        // 2. 印點 '.'
        if (remainder > 0) {
            printf(".");
        }
        
        printf("\n"); // 換行
    } 
    return 0;
}

// =======================================================
// 成績生成及統計函數
// =======================================================

// 產生 n 個均勻分佈成績並統計
void genUnifrom(int n, int numRow, double* sum, double* ss2, int cnt[]) {
    *sum = 0.0;
    *ss2 = 0.0;
    for (int i = 0; i < n; ++i) {
        int score = uniSc();
        
        // 統計到區間 count[]
        int bucket_index = score / PER_ROW;
        if (bucket_index >= 0 && bucket_index < numRow) {
            cnt[bucket_index]++;
        }
        
        // 統計總和 (sum) 和平方和 (ss2)
        *sum += score;
        *ss2 += (double)score * score;
    }
}

// 產生 n 個常態分佈成績並統計
void genNormal(int n, int numRow, double* sum, double* ss2, int* cnt, double avg_desire, double std_desire) {
    *sum = 0.0;
    *ss2 = 0.0;
    for (int i = 0; i < n; ++i) {
        int score = nmSc(avg_desire, std_desire);
        
        // 統計到區間 count[]
        int bucket_index = score / PER_ROW;
        if (bucket_index >= 0 && bucket_index < numRow) {
            cnt[bucket_index]++;
        }
        
        // 統計總和 (sum) 和平方和 (ss2)
        *sum += score;
        *ss2 += (double)score * score;
    }
}


// =======================================================
// 主程式
// =======================================================

int main() {
    // (2) 決定是否做 srand( time(0) );
    if (USE_SRAND == 1) {
        // set PRNG seed; C23開始的compiler一定要含入 <time.h> 
        srand(time(0));  
        printf("✅ Use srand() to set time(0) as Seed\n");
    } else {
        // 若沒用 srand 每次重跑 rand() 亂數序會一樣 
        printf("❌ NOT use srand()\n");
    }
    printf("==================================================\n");

    int n = 0; 
    char input_buffer[256];
    
    // (5) 讀取學生數 n
    printf("請輸入學生數 N (建議 %d ~ %d): ", MIN_STU, MAX_STU);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
        n = (int)strtol(input_buffer, NULL, 10);
    }
    
    // 調整到範圍內 
    if (n < MIN_STU) n = MIN_STU; 
    if (n > MAX_STU) n = MAX_STU;
    
    double avg, std, variance, sum, ss2;
    int max_count = 0;
    int oneStar = 0;

    // =======================================================
    // I. 均勻分佈 (Uniform Distribution) 測試
    // =======================================================
    
    // 清除 count[] 
    memset(count, 0, sizeof(count));

    printf("\n\n=== 1. 均勻分佈 (Uniform Distribution) 成績生成 ===\n");
    printf("Generating %d Uniform dist. scores...\n", n);
    
    // 產生 n 個成績並統計 sum, ss2, count[]
    genUnifrom(n, numRow, &sum, &ss2, count);

    // 計算統計量
    avg = sum / n;
    variance = ss2 / n - avg * avg;
    if (variance < 0) variance = 0; // 避免浮點數誤差造成負值
    std = sqrt(variance);

    // (3) 聰明地計算出每個星號要代表幾次或幾個
    // 找出最大次數
    max_count = 0;
    for(int i = 0; i < numRow; ++i) {
        if (count[i] > max_count) max_count = count[i];
    }
    
    // 決定 oneStar: 讓最大次數 (max_count) 在長條圖上約印出 MAX_HIST_COLS 個星號
    oneStar = max_count / MAX_HIST_COLS;
    
    // 題目要求 oneStar 要是偶數
    oneStar = (oneStar + 1) / 2 * 2; 
    
    if (oneStar < 1) oneStar = 1;  // 防呆 
    if (oneStar % 2 != 0) oneStar++; // 確保是偶數 (如果除法運算造成奇數)
    
    printf("\n📊 Uniform: 計算出每個 * 代表 %d 個\n", oneStar);
    printBar(n, oneStar, numRow, count);
    
    printf("\n📈 統計結果 (Uniform)\n");
    printf("Average = %.2f\n", avg);
    printf("Variance = %.2f\n", variance);
    printf("Std Dev = %.2f\n", std);

    printf("\n= = Hit ENTER key to continue for Normal dist.: "); 
    // 清除輸入緩衝區中殘餘的換行符
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    // =======================================================
    // II. 常態分佈 (Normal Distribution) 測試
    // =======================================================
    
    double avg_desire = 0.0, std_desire = 0.0;
    
    printf("\n\n=== 2. 常態分佈 (Normal Distribution) 成績生成 ===\n");
    
    // 讀取希望的平均和標準差
    printf("請輸入希望的平均分數 (例如 60.0): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
        avg_desire = atof(input_buffer);
    }
    
    printf("請輸入希望的標準差 (例如 15.0): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
        std_desire = atof(input_buffer);
    }

    // 參數防呆
    if (std_desire <= 0.0) std_desire = 15.0; // 避免標準差為 0
    if (avg_desire <= 0.0 || avg_desire >= 100.0) avg_desire = 60.0; // 避免平均不合理
    
    // 清除 count[]
    memset(count, 0, sizeof(count));

    printf("\nGenerating %d Normal dist. scores [0, 99]; Desired Avg=%.2f, Std=%.2f\n",
             n, avg_desire, std_desire);
             
    // 產生 n 個成績並統計 sum, ss2, count[]
    genNormal(n, numRow, &sum, &ss2, count, avg_desire, std_desire);

    // 重新計算 oneStar (常態分佈中間會比較多，可以適當放大 oneStar)
    // 隨便寫的 oneStar = oneStar * 1.5; 這裡我們重新計算最大次數
    max_count = 0;
    for(int i = 0; i < numRow; ++i) {
        if (count[i] > max_count) max_count = count[i];
    }
    
    oneStar = max_count / MAX_HIST_COLS;
    oneStar = (oneStar + 1) / 2 * 2; // 變偶數
    if (oneStar < 1) oneStar = 1;  // 防呆
    if (oneStar % 2 != 0) oneStar++; // 確保是偶數
    
    printf("\n📊 Normal: 計算出每個 * 代表 %d 個\n", oneStar);
    printBar(n, oneStar, numRow, count);
    
    printf("\n📈 統計結果 (Normal)\n");
    printf("Desired Avg=%.2f, Desired Std=%.2f\n", avg_desire, std_desire);
    
    // 計算所產生資料的實際統計量
    avg = sum / n;
    variance = ss2 / n - avg * avg;
    if (variance < 0) variance = 0;
    std = sqrt(variance);
    
    printf("Actual Average = %.2f\n", avg);
    printf("Actual Variance = %.2f\n", variance);
    printf("Actual Std Dev = %.2f\n", std);

    printf("\nHit ENTER to finish..");
    getchar(); 
    
    return 0;
}