#include <iostream>
using namespace std;
int gcd(int a,int b){
  while ( a != 0 && b != 0 )
    if ( a > b)
      a = a%b;
    else b = b%a;
  return a + b;
}
int main(){
  int a,b;
  cout<<"Please, write two numbers for search gcd"<<endl;
  cin>>a>>b;
  cout<<"gcd "<<a<<" and "<<b<<": "<<gcd(a,b);
  return 0;
}
