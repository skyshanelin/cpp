#include "myhwk09to11.h"

// (e), (f) 輸出全校資料 (含人數限制)
void print_data(const Student *students, int count, const char *title) {
    printf("--- %s (%d 人) ---\n", title, count);
    
    // 輸出標題行
    printf("----------------------------------------------------------------------------------------------------------------------\n");
    printf("| 排名 | 學號       | 姓名     | 科目一(3) | 科目二(4) | 科目三(2) | 加權平均 |\n");
    printf("----------------------------------------------------------------------------------------------------------------------\n");

    int limit = 15; // 前後各印 15 人
    int i;

    // 判斷是否需要省略
    if (count > 2 * limit) {
        // 打印前面 15 人
        for (i = 0; i < limit; i++) {
            printf("| %-4d | %-10lld | %-8s | %-9d | %-9d | %-9d | %-8.2f |\n",
                   students[i].rank,
                   students[i].studentID,
                   students[i].chineseName,
                   students[i].scores[0],
                   students[i].scores[1],
                   students[i].scores[2],
                   students[i].weightedAvg);
        }

        // 打印省略行
        printf("|  ... | ...        | ...      | ...       | ...       | ...       | ...      |\n");

        // 打印後面 15 人
        for (i = count - limit; i < count; i++) {
            printf("| %-4d | %-10lld | %-8s | %-9d | %-9d | %-9d | %-8.2f |\n",
                   students[i].rank,
                   students[i].studentID,
                   students[i].chineseName,
                   students[i].scores[0],
                   students[i].scores[1],
                   students[i].scores[2],
                   students[i].weightedAvg);
        }
    } else {
        // 打印所有學生
        for (i = 0; i < count; i++) {
            printf("| %-4d | %-10lld | %-8s | %-9d | %-9d | %-9d | %-8.2f |\n",
                   students[i].rank,
                   students[i].studentID,
                   students[i].chineseName,
                   students[i].scores[0],
                   students[i].scores[1],
                   students[i].scores[2],
                   students[i].weightedAvg);
        }
    }
    printf("----------------------------------------------------------------------------------------------------------------------\n");
}

// (e), (f) 計算並輸出統計數據
void calc_and_print_stats(const Student *students, int count) {
    // 總和陣列 [科目1, 科目2, 科目3, 總平均]
    double sums[N_SUBJ + 1] = {0.0}; 
    // 平方和陣列，用於計算標準差
    double sumSquares[N_SUBJ + 1] = {0.0}; 
    
    // 1. 計算總和與平方和
    for (int i = 0; i < count; i++) {
        // 科目分數總和與平方和
        for (int j = 0; j < N_SUBJ; j++) {
            sums[j] += students[i].scores[j];
            sumSquares[j] += (double)students[i].scores[j] * students[i].scores[j];
        }
        // 加權平均總和與平方和
        sums[N_SUBJ] += students[i].weightedAvg;
        sumSquares[N_SUBJ] += (double)students[i].weightedAvg * students[i].weightedAvg;
    }

    // 2. 計算平均數
    double averages[N_SUBJ + 1];
    for (int i = 0; i <= N_SUBJ; i++) {
        averages[i] = sums[i] / count;
    }

    // 3. 計算標準差
    double stdDevs[N_SUBJ + 1];
    for (int i = 0; i <= N_SUBJ; i++) {
        // 標準差公式: sqrt( (Σx² / N) - (平均數)² )
        double variance = (sumSquares[i] / count) - (averages[i] * averages[i]);
        // 確保非負
        if (variance < 0) variance = 0.0;
        stdDevs[i] = sqrt(variance);
    }

    // 4. 輸出結果 (所有數值都印到小數後兩位)
    printf("\n各科平均與總成績平均：");
    for (int i = 0; i < N_SUBJ; i++) {
        printf(" %-8.2f", averages[i]);
    }
    printf(" | %-8.2f\n", averages[N_SUBJ]);


    printf("各科與總成績標準差： ");
    for (int i = 0; i < N_SUBJ; i++) {
        printf(" %-8.2f", stdDevs[i]);
    }
    printf(" | %-8.2f\n", stdDevs[N_SUBJ]);
    printf("----------------------------------------------------------------------------------------------------------------------\n");
}