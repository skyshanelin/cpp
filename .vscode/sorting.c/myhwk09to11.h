#ifndef MYHWK09TO11_H
#define MYHWK09TO11_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

// (b) 宏定義
#define MAX_STU 33885566
#define NUM_STU 95678    // 全校學生人數
#define MAX_SUBJ 9       // 最多科目數
#define N_SUBJ 3         // 本次實際科目數
#define MAX_NAME_LEN 9   // 中文姓名最多四個字 (8 bytes) + \0 (1 byte)

// 學生資料結構
typedef struct {
    long long studentID; // 學號 (10位數字)
    char chineseName[MAX_NAME_LEN]; // 中文姓名 (最多四字, 包含複姓)
    int scores[N_SUBJ]; // 三科目成績 (整數)
    float weightedAvg;  // 加權平均分數
    int rank;           // 排名
} Student;


// (c) 各 Global 變數宣告 (必須帶 extern，避免重複定義)
extern int numStu;
extern int nSubj;
extern int credit[];
extern float expMean[];
extern float expStd[];

// (d) 函數宣告 (Function Prototypes)
// data_gen.c
void generate_data(Student *students, int count);
float generate_normal_score(float mean, float stdDev);
void generate_name(char *name_buffer, size_t max_len);

// data_proc.c
void swap_students(Student *a, Student *b);
void calc_weighted_avg(Student *students, int count);
void rank_students(Student *students, int count);
int compare_students_for_rank(const Student *a, const Student *b); // 用於 quicksort 內部比較

// Quick Sort 排序 (依平均高低排序)
double sort_by_rank_quicksort_mo3(Student *students, int count); // Median-of-Three (計時 Wrapper)
double sort_by_rank_quicksort_simple(Student *students, int count); // Simple Pivot (計時 Wrapper)

// qsort() 排序 (依學號低到高排序)
double sort_by_id_qsort_wrapper(Student *students, int count); // qsort 函式庫實作 (計時 Wrapper)

// output.c
void print_data(const Student *students, int count, const char *title);
void calc_and_print_stats(const Student *students, int count);

#endif // MYHWK09TO11_H