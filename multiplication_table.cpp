#include <iostream>
#include<string>
#include<cmath>
using namespace std;

int main() 
{
    cout << "114611008林宥翔" << endl;
    char word[99];
    while(3388){
      cout << "Give me a interger number among 6 ~ 19 , input q if you want to quit." << endl;
      cin >> word ;
      int m , n;
       
      if(word ==  "Q" ){
        cout << "Bye bye" << endl ; 
        return 0;
      }
      int num = atoi(word);
      if(num >= 6 && num <=19){
        for(m = 1 ; m <= num ; m++ ){
          cout << endl ;
          for(n = 1; n <= num ; n++){
            cout << m << "*" << n << "=" << n*m << " ";
          }
        }
        cout << endl ;

      }else{
        cout << "your selected number is illegal" ;
      }
      return 0;
    }
    
}