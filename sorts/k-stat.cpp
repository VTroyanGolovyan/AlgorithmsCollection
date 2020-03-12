#include <iostream>
#include  <algorithm>

using std::cin;
using std::cout;
using std::endl;
using std::swap;
using std::rand;

template <typename T>
int partition(T *a, int l, int r) {
    int mid = l + (r - l) / 2;

    if (a[l] > a[r])
        swap(a[l], a[r]);
    if (a[mid] > a[r])
        swap(a[mid],a[r]);
    if (a[l] < a[mid])
        swap(a[l],a[mid]);

    int pivot = a[l];

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

template <typename T>
T kstat(T *a, int l, int r,int& k) {
    int p;
    while (l < r){
         p = partition(a, l, r);
         if (p == k)
             return a[k];
         if (k < p)
             r = p - 1;
         else l = p + 1;
    }
    return a[k];
}

int main() {
    int n, k;
    cin >> n >> k;
    int* a = new int[n];
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    cout << kstat(a,0,n-1,k);
    delete[] a;
    return 0;
}
