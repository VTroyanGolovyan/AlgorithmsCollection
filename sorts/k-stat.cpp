#include <iostream>
#include  <algorithm>
#include <cstdlib>

using std::cin;
using std::cout;
using std::endl;
using std::swap;
using std::rand;

int partition(int *a, int l, int r) {
   int index = rand() % (r - l + 1) + l;
   int pivot = a[index];
   swap(a[l],a[index]);
   int i = r;
   for (int j = r; j > l; j--) {
       if (a[j] > pivot) {
           swap(a[i],a[j]);
           i--;
       }
   }
    swap(a[i],a[l]);
    return i;
}

void qsort(int *a, int l, int r) {
    if (l < r) {
        int p = partition(a, l, r);
        qsort(a, l,p - 1);
        qsort(a, p + 1, r);
    }
}

int kstat(int *a, int l, int r, int& k) {
    if (l < r) {
        int p = partition(a, l, r);
        if (p == k)
            return a[k];
        if (k < p)
            return kstat(a,l,p - 1,k);
        else return kstat(a,p + 1, r, k);
    } else return a[k];
}

int main() {
    int n, k;
    cin>>n>>k;
    int* a = new int[n];
    for (int i = 0; i < n; i++) {
        cin>>a[i];
    }
    cout<<kstat(a, 0, n-1, k);
    delete []a;
    return 0;
}
