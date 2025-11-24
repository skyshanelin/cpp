#include "myhwk09to11.h"
#include "data_proc.c"
#include "data_gen.c"
#include "output.c"



// 全局變數的定義 (Global Variable Definitions)
int numStu = NUM_STU;
int nSubj = N_SUBJ;
// 實際學分數和期望的平均/標準差只用到前 3 個元素
int credit[MAX_SUBJ] = { 3, 4, 2, 3, 3, 3, 3, 3, 3 };  // 各科幾學分 (第一科3, 第二科4, 第三科2)
float expMean[MAX_SUBJ] = { 75.0, 70.5, 82.0, 74.25, 75.5 };  // 期望的各科平均
float expStd[MAX_SUBJ] = { 7.5, 9.75, 8.5, 6.0, 5.0 };  // 期望的各科標準差

int main(void) {
    // 依要求設定亂數種子，確保每次產生資料完全相同
    srand(23849); 

    // 動態分配學生資料陣列
    Student *allStudents = (Student *)malloc(numStu * sizeof(Student));
    if (allStudents == NULL) {
        fprintf(stderr, "記憶體分配失敗。\n");
        return 1;
    }

    printf("--- 開始產生 %d 位學生的資料 (Seed: 23849) ---\n", numStu);

    // (a), (b) 產生學號、姓名和成績 (所有測試將使用這組資料)
    generate_data(allStudents, numStu);
    printf("--- 資料產生完成 ---\n");

    // (c) 計算加權平均分數
    calc_weighted_avg(allStudents, numStu);

    // (d) 依平均做排名 (排名在原始資料上進行，後續排序使用複製的陣列)
    rank_students(allStudents, numStu);
    printf("--- 平均分數計算與排名完成 ---\n");


    // ----------------------------------------------------------------------------------
    // Test 1: Quick Sort (Median-of-Three Pivot) - 依排名高到低印出
    // ----------------------------------------------------------------------------------
    Student *rankedStudents_mo3 = (Student *)malloc(numStu * sizeof(Student));
    if (rankedStudents_mo3 == NULL) {
        fprintf(stderr, "記憶體分配失敗 (mo3)。\n");
        free(allStudents);
        return 1;
    }
    memcpy(rankedStudents_mo3, allStudents, numStu * sizeof(Student)); // 複製原始資料

    printf("\n==================================================================\n");
    printf(" (e) 排序測試 1: 依總成績 (加權平均) 由高到低 (Quick Sort, Median-of-Three Pivot)\n");
    printf("==================================================================\n");

    // (g) 測量排序時間
    double sortTime_rank_mo3 = sort_by_rank_quicksort_mo3(rankedStudents_mo3, numStu);
    printf("\n*** 排序 (Quick Sort Mo3) 耗時: %.9g 秒 ***\n", sortTime_rank_mo3);

    // (e) 輸出排名後的資料
    print_data(rankedStudents_mo3, numStu, "依排名 (Quick Sort Mo3) 排序");
    calc_and_print_stats(rankedStudents_mo3, numStu);
    free(rankedStudents_mo3);


    // ----------------------------------------------------------------------------------
    // Test 2: Quick Sort (Simple Pivot) - 依排名高到低印出 (用於比較)
    // ----------------------------------------------------------------------------------
    Student *rankedStudents_simple = (Student *)malloc(numStu * sizeof(Student));
    if (rankedStudents_simple == NULL) {
        fprintf(stderr, "記憶體分配失敗 (simple)。\n");
        free(allStudents);
        return 1;
    }
    memcpy(rankedStudents_simple, allStudents, numStu * sizeof(Student)); // 複製原始資料

    printf("\n\n==================================================================\n");
    printf(" (e) 排序測試 2: 依總成績 (加權平均) 由高到低 (Quick Sort, Simple Left Pivot)\n");
    printf("==================================================================\n");

    // (g) 測量排序時間
    double sortTime_rank_simple = sort_by_rank_quicksort_simple(rankedStudents_simple, numStu);
    printf("\n*** 排序 (Quick Sort Simple) 耗時: %.9g 秒 ***\n", sortTime_rank_simple);

    // (e) 輸出排名後的資料
    print_data(rankedStudents_simple, numStu, "依排名 (Quick Sort Simple) 排序");
    calc_and_print_stats(rankedStudents_simple, numStu);
    free(rankedStudents_simple);


    // ----------------------------------------------------------------------------------
    // Test 3: Standard qsort() - 依學號低到高印出
    // ----------------------------------------------------------------------------------
    Student *idSortedStudents_qsort = (Student *)malloc(numStu * sizeof(Student));
    if (idSortedStudents_qsort == NULL) {
        fprintf(stderr, "記憶體分配失敗 (qsort)。\n");
        free(allStudents);
        return 1;
    }
    memcpy(idSortedStudents_qsort, allStudents, numStu * sizeof(Student)); // 複製原始資料

    printf("\n\n==================================================================\n");
    printf(" (f) 排序測試 3: 依學號由小到大 (Standard Library qsort())\n");
    printf("==================================================================\n");

    // 進行學號排序並計時
    double sortTime_id_qsort = sort_by_id_qsort_wrapper(idSortedStudents_qsort, numStu);
    printf("\n*** 排序 (Standard qsort() by ID) 耗時: %.9g 秒 ***\n", sortTime_id_qsort);

    // (f) 輸出學號排序後的資料
    print_data(idSortedStudents_qsort, numStu, "依學號 (qsort) 排序");
    calc_and_print_stats(idSortedStudents_qsort, numStu);
    free(idSortedStudents_qsort);


    // 釋放原始資料記憶體
    free(allStudents);

    return 0;
}