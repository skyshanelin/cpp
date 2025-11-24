#include "myhwk09to11.h"

// 輔助函數：四捨五入到小數點後兩位
float round_to_two_decimals(float value) {
    // 乘以 100.0, 加 0.5 後取整, 再除以 100.0
    return floorf(value * 100.0 + 0.5) / 100.0;
}

// (c) 計算加權平均分數
void calc_weighted_avg(Student *students, int count) {
    int totalCredits = 0;
    for (int i = 0; i < nSubj; i++) {
        totalCredits += credit[i];
    }

    for (int i = 0; i < count; i++) {
        float weightedSum = 0.0;
        for (int j = 0; j < nSubj; j++) {
            weightedSum += students[i].scores[j] * credit[j];
        }

        float rawAvg = weightedSum / totalCredits;

        // 四捨五入到小數點後兩位
        students[i].weightedAvg = round_to_two_decimals(rawAvg);
    }
}

// 輔助函數：交換兩個學生資料
void swap_students(Student *a, Student *b) {
    Student temp = *a;
    *a = *b;
    *b = temp;
}

// 比較函數：用於排名和 Quick Sort 的穩定性
// 返回負數表示 a 優先 (排名較高/分數較高), 返回正數表示 b 優先 (排名較低), 返回 0 表示相同
int compare_students_for_rank(const Student *a, const Student *b) {
    // 1. 比較加權平均分數 (高者優先)
    if (a->weightedAvg > b->weightedAvg) return -1;
    if (a->weightedAvg < b->weightedAvg) return 1;

    // 2. 平均分數相同，比較科目一 (高者優先)
    if (a->scores[0] > b->scores[0]) return -1;
    if (a->scores[0] < b->scores[0]) return 1;

    // 3. 科目一相同，比較科目二 (高者優先)
    if (a->scores[1] > b->scores[1]) return -1;
    if (a->scores[1] < b->scores[1]) return 1;

    // 4. 科目二相同，比較科目三 (高者優先)
    if (a->scores[2] > b->scores[2]) return -1;
    if (a->scores[2] < b->scores[2]) return 1;

    return 0; // 所有成績都相同
}

// (d) 依據每位同學平均分數做排名 (邏輯不變)
void rank_students(Student *students, int count) {
    Student *tempStudents = (Student *)malloc(count * sizeof(Student));
    if (tempStudents == NULL) {
        fprintf(stderr, "記憶體分配失敗 (排名)。\n");
        return;
    }
    memcpy(tempStudents, students, count * sizeof(Student));

    // 使用 qsort 排序 (成績高到低)
    qsort(tempStudents, count, sizeof(Student), (int (*)(const void *, const void *))compare_students_for_rank);

    // 賦予排名
    int current_rank = 1;
    int rank_skip_count = 0; 

    for (int i = 0; i < count; i++) {
        if (i == 0) {
            tempStudents[i].rank = current_rank;
            rank_skip_count = 1;
        } else {
            // 比較當前學生和前一個學生 (成績和 tie-breaker)
            if (compare_students_for_rank(&tempStudents[i], &tempStudents[i - 1]) == 0) {
                // 成績完全相同，排名也相同
                tempStudents[i].rank = current_rank;
                rank_skip_count++;
            } else {
                // 成績不同，排名跳過 (例如：兩個第3名後，下一名是第5名)
                current_rank += rank_skip_count;
                tempStudents[i].rank = current_rank;
                rank_skip_count = 1;
            }
        }
    }

    // 將排名結果寫回原始陣列 (根據學號匹配，這是 O(N^2) 的寫回方式)
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < count; j++) {
            if (students[j].studentID == tempStudents[i].studentID) {
                students[j].rank = tempStudents[i].rank;
                break;
            }
        }
    }
    
    free(tempStudents);
}

// =========================================================================
// Quick Sort 實作
// =========================================================================

// 輔助函數：Median-of-Three 選擇中間值作為 Pivot
int get_median_of_three_pivot(Student *students, int low, int high) {
    int mid = low + (high - low) / 2;

    // 使用 compare_students_for_rank 判斷誰的排名高 (負數表示第一個參數排名高)
    // 目標是找到排名第二高的 (也就是中間值)
    if (compare_students_for_rank(&students[low], &students[mid]) < 0) {
        if (compare_students_for_rank(&students[mid], &students[high]) < 0)
            return mid;
        else if (compare_students_for_rank(&students[low], &students[high]) < 0)
            return high;
        else
            return low;
    } else {
        if (compare_students_for_rank(&students[high], &students[mid]) < 0)
            return mid;
        else if (compare_students_for_rank(&students[low], &students[high]) < 0)
            return low;
        else
            return high;
    }
}

// Quick Sort Partition (Median-of-Three)
int partition_median_of_three(Student *students, int low, int high) {
    int pivot_idx = get_median_of_three_pivot(students, low, high);
    swap_students(&students[pivot_idx], &students[low]); // 將 pivot 移到最左邊

    Student pivot = students[low];
    int i = low;
    int j = high + 1;

    // 排序方向: 高分在前 (降序)
    while (1) {
        // 尋找 i，直到找到比 pivot 排名低的 (compare > 0)
        do {
            i++;
        } while (i <= high && compare_students_for_rank(&students[i], &pivot) < 0);

        // 尋找 j，直到找到比 pivot 排名高的 (compare < 0)
        do {
            j--;
        } while (compare_students_for_rank(&students[j], &pivot) > 0);

        if (i >= j) break;
        swap_students(&students[i], &students[j]);
    }

    swap_students(&students[low], &students[j]);
    return j;
}

// Quick Sort 遞迴函數 (Median-of-Three)
void quicksort_mo3_recursive(Student *students, int low, int high) {
    if (low < high) {
        int pi = partition_median_of_three(students, low, high);
        quicksort_mo3_recursive(students, low, pi - 1);
        quicksort_mo3_recursive(students, pi + 1, high);
    }
}

// Quick Sort Partition (Simple Left Pivot)
int partition_simple(Student *students, int low, int high) {
    Student pivot = students[low];
    int i = low;
    int j = high + 1;

    while (1) {
        // 尋找 i (降序: i 往右找比 pivot 排名低的)
        do {
            i++;
        } while (i <= high && compare_students_for_rank(&students[i], &pivot) < 0);

        // 尋找 j (降序: j 往左找比 pivot 排名高的)
        do {
            j--;
        } while (compare_students_for_rank(&students[j], &pivot) > 0);

        if (i >= j) break;
        swap_students(&students[i], &students[j]);
    }

    swap_students(&students[low], &students[j]);
    return j;
}

// Quick Sort 遞迴函數 (Simple Left Pivot)
void quicksort_simple_recursive(Student *students, int low, int high) {
    if (low < high) {
        int pi = partition_simple(students, low, high);
        quicksort_simple_recursive(students, low, pi - 1);
        quicksort_simple_recursive(students, pi + 1, high);
    }
}


// =========================================================================
// 排序計時 Wrapper
// =========================================================================

// Quick Sort (Median-of-Three) 計時
double sort_by_rank_quicksort_mo3(Student *students, int count) {
    clock_t start_time = clock();
    quicksort_mo3_recursive(students, 0, count - 1);
    clock_t end_time = clock();
    return (double)(end_time - start_time) / CLOCKS_PER_SEC;
}

// Quick Sort (Simple Pivot) 計時
double sort_by_rank_quicksort_simple(Student *students, int count) {
    clock_t start_time = clock();
    quicksort_simple_recursive(students, 0, count - 1);
    clock_t end_time = clock();
    return (double)(end_time - start_time) / CLOCKS_PER_SEC;
}

// qsort 比較函數 (依學號低到高)
int compare_students_for_id_qsort(const void *a, const void *b) {
    const Student *stuA = (const Student *)a;
    const Student *stuB = (const Student *)b;

    if (stuA->studentID < stuB->studentID) return -1;
    if (stuA->studentID > stuB->studentID) return 1;
    return 0;
}

// qsort() 排序計時 (依學號低到高)
double sort_by_id_qsort_wrapper(Student *students, int count) {
    clock_t start_time = clock();
    qsort(students, count, sizeof(Student), compare_students_for_id_qsort);
    clock_t end_time = clock();
    return (double)(end_time - start_time) / CLOCKS_PER_SEC;
}