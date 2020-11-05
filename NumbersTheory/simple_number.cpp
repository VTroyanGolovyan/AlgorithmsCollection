#include <iostream>
using namespace std;
bool is_simple(int a){
  if (a <=1 ) return false;
  for (int d = 2; d*d<=a; d++)
    if (a % d == 0)
      return false;
  return true;
}
int main(){
  cout<<"Please, write number for testing"<<endl;
  int a;
  cin>>a;
  if (is_simple(a))
    cout<<"This number is simple";
  else cout<<"This number isn't simple";
  return 0;
}
