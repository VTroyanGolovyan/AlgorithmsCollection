#include <iostream>
using std::cin;
using std::cout;

template <typename MyType>
void shiftDown( MyType* a, int i, int n) {
    int firstChild = 2 * i + 1;
    int secondChild = 2 * i + 2;
    int max = i;
    if (firstChild < n && a[max] < a[firstChild]) {
        max = firstChild;
    }
    if (secondChild < n && a[max] < a[secondChild]) {
        max = secondChild;
    }
    if (max != i) { //нам не нужно менять элемент так как он уже максимальный.
        MyType t = a[i];
        a[i] = a[max];
        a[max] = t;
        shiftDown(a, max, n); //востанавливакм свойство кучи
    }
}

template <typename  MyType>
void buildHeap(MyType* a, int n) {
    for (int i = n/2; i >= 0; i--){
       shiftDown(a, i, n); //востанавливакм свойство кучи
    }
}

template <typename  MyType>
void heapSort( MyType* a, int n) {
    buildHeap(a, n); //строим кучу О(n)
    int m = n;
    for (int i = n - 1; i > 0; i--){
        MyType t = a[i];
        a[i] = a[0];
        a[0] = t; //меняем местами последний элемент с корнем кучи
        m--; // на i-m месте стоит нужный элемент, он не нужен в куче
        shiftDown(a,0,m); //востановление свойства кучи
    }
}

int main() {
    int n;
    cin>>n;
    int* a = new int[n];
    for (int i = 0; i < n; i++) {
        cin>>a[i];
    }
    heapSort(a, n);
    for (int i = 0; i < n; i++) {
        cout<<a[i]<<" ";
    }
    return 0;
}
