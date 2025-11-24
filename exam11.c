#include<stdio.h>
#include<stdlib.h>
#include<string.h>

  int getLongs(long *x) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        return 0; // 讀取失敗或 EOF
    }

    int count = 0;
    char *ptr = line;
    char *endptr;

    // 忽略開頭的空白
    ptr += strspn(ptr, " \t\n");

    // 循環解析數字
    while (*ptr != '\0') {
        // 使用 strtol 轉換字串為 long，並將 endptr 指向數字後的下一個字元
        x[count] = strtol(ptr, &endptr, 10);

        // 如果沒有讀到任何數字 (ptr 等於 endptr)，則跳出
        if (ptr == endptr) {
            break;
        }

        count++;
        if (count >= 99) break; // 防止溢出陣列

        // 移動 ptr 到下一個非數字字元 (即數字後第一個字元)
        ptr = endptr;
        // 忽略數字後的空白
        ptr += strspn(ptr, " \t\n");
    }
    return count;
}

long gcd(long a, long b) {
  
   a = (a < 0) ? -a : a;
   b = (b < 0) ? -b : b;
  
   if (b == 0){
     return a;
   } 
   return (a < b)? gcd(b , a):gcd(b, a % b);
}

long lcm(long a, long b) {

   a = (a < 0) ? -a : a;
   b = (b < 0) ? -b : b;

   if (a == 0 || b == 0) {
      return 0;
   } 
   return (a / gcd(a, b)) * b;
}
   
int main() {
   int name[] = {909390129, 808464689, -1751194056, -408572187,-553610049, 32766};
   char* id = (char*)name;
   printf("==%s\n", id); 
   printf("Give me at least 2 numbers, I will show GCD and LCM of them.\n"); 
   long x[99];
   while(100) { 
      printf("Numbers: ");
        
      int n = getLongs(x);
      
      if (n < 2) {
         break;
      }

   long result_gcd = x[0];
      for (int i = 1; i < n; i++) {
         result_gcd = gcd(result_gcd, x[i]);
         
         if (result_gcd == 1) {
            break;
         }
      }

      long result_lcm = x[0];
      result_lcm = (result_lcm < 0) ? -result_lcm : result_lcm;

      for (int i = 1; i < n; i++) {
         result_lcm = lcm(result_lcm, x[i]);
   }

   printf("GCD(");
        for (int i = 0; i < n; i++) {
            printf("%ld", x[i]);
            if (i < n - 1) {
                printf(", ");
            }
        }
        printf(") = %ld\n", result_gcd);
        

        printf("LCM(");
        for (int i = 0; i < n; i++) {
            printf("%ld", x[i]);
            if (i < n - 1) {
                printf(", ");
            }
        }
        printf(") = %ld\n", result_lcm);
   }
   return 0;
}
