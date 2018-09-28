#include <iostream>
#include <vector>
#include <utility>
using namespace std;
int min(int a,int b){
  return a > b ? b : a;
}
int main(){
  int n,s,f;
  cin>>n>>s>>f;
  s--;
  f--;
  int a[n][n];
  int w[n];
  bool u[n];
  for (int i = 0; i < n; i++){
    w[i] = 100000000;//расстояние до всех вершин бесконечно
    u[i] = false;
  }

  w[s] = 0;
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++){
      cin>>a[i][j];
      if (a[i][j]==-1)
        a[i][j] = 100000000000; //бесконечные ребра

  }

  for (int i = 1; i < n; i++){//просмотрим все вершины
    int v = -1; // индекс вершины расстояние от s до которой минимально
    for (int j = 0; j < n; j++){//поиск вершины
      if ( !u[j] && (v == -1 || w[j] < w[v])){
        v = j;
      }
    }

    if (w[v] == 100000000)
      break;  //не осталось вершин до которых можно дойти
    u[v] = true;//пометили что эту вершину мы уже посетили
    for (int j = 0; j < n; j++){

        w[j] = min(w[j],w[v]+a[v][j]);//ищем что меньше , предыдущее найденное расстояние или сейчас найденное

    }
  }
  cout<< (w[f] == 100000000 ? -1 : w[f]);
  return 0;
}
