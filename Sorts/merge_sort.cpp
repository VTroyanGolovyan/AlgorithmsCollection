#include <iostream>

class  isLess{
public:
    bool operator ()(int & a, int & b) {
        return a < b;
    }
};

template <typename T, typename TLess>
void  merge(T *a, int l, int r, TLess& isLess) {
    int mid = l + (r - l) / 2;
    int i = 0;
    int j = 0;
    T *res = new int[r - l];
    while(l + i < mid && mid + j < r) {
        if (isLess( a[l+i], a[mid + j]) ) {
            res[i + j] = a[l + i];
            i++;
        } else {
            res[i + j] = a[mid + j];
            j++;
        }
    }
    while(l + i < mid) {
        res[i + j] = a[l + i];
        i++;
    }
    while(mid + j < r) {
        res[i + j] = a[mid + j];
        j++;
    }
    for (int z = 0; z < r - l; z++){
        a[l + z] = res[z];
    }
    delete []res;
}

template <typename T, typename TLess>
void mergeSort(T *a, int l, int r, TLess& isLess) {
    if (l + 1 >= r)
        return;
    int mid = l + (r - l) / 2;
    mergeSort(a, l, mid, isLess);
    mergeSort(a, mid, r, isLess);
    merge(a, l, r, isLess);
}

int main() {
    int *a = new int[10];
    for (int i = 0; i < 10; i++)
        a[i] = 0;
    mergeSort(a, 0, 10, isLess());
    return 0;
}