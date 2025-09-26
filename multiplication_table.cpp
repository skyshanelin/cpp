#include <iostream>
#include<cmath>
using namespace std;

int main() 
{
    cout << "114611008林宥翔" << endl ;
    int num , m , n;
    char word[99];
    cout << "Give me a interger number among 6 ~ 19 , input Q if you want to quit." << endl;
    cin >>  word ;
    if ( word[0] == 'Q' ){
      cout << "Bye bye" ;
      return 0;
    }
    num = atoi(word);
    if(num >= 6 && num <=19){
      for(m = 1 ; m <= num ; m++ ){
        cout << endl;
        for(n = 1; n <= num ; n++){
            cout << m*n << " " ;
            if (m*n < 10){
              cout << "  ";
            }
            if(m * n < 100 && m*n >= 10){
              cout << " ";
            }
          }
        }
      }else{
        cout << "your input is illegal" <<endl;
      }
      return 0;
    }
    
