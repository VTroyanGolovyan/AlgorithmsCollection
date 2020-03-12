#include <iostream>

using std::cin;
using std::cout;
using std::endl;

unsigned long long pow(int a, int k) {
    if (k == 0)
        return 1;
    if (k % 2 == 1)
        return pow (a, k - 1) * a;
    else {
        unsigned long long res = pow (a, k / 2);
        return res * res;
    }
}

short kBit(unsigned long long a, int k) {
    unsigned long long u = pow(2, k);
    if ((a & u) == u)
        return 1;
    else return 0;
}
void MSDSort(long long * a, int l, int r, int k) {
    long long *b = new long long[r - l]; //перестановка на отрезке
    int *c = new int[2]; //массив для сортировки подсчетом

    for (int i = 0; i < 2; i++) {
        c[i] = 0; //обнуляем значения
    }

    for (int i = l; i < r; i++) {
        c[kBit(a[i], k)]++; //считаем биты
    }

    int* indexes = new int[2]; //массив для индексации
    for (int i = 0; i < 2; i++) {
        indexes[i] = c[i];
    }

    int count = 0; //количество элементов которые находяться до iого элемента
    for (int i = 0; i < 2; i++) {
        int t = indexes[i];
        indexes[i] = count;
        count += t; //увеличили счетчик
    }

    for (int i = l; i < r; i++) { //перестановка элементов
        int bit = kBit(a[i], k);
        b[indexes[bit]] = a[i];
        indexes[bit]++;
    }
    for (int i = l; i < r; i++) {
        a[i] = b[i - l];
    }

    delete []b;

    if (k > 0) {
        if (c[0] > 0)
            MSDSort(a, l, l + c[0],  k - 1);
        if (c[1] - 1 > 0)
            MSDSort(a,l + c[0], l + c[0] + c[1],  k - 1);
    }

    delete []c;
    delete []indexes;
}
int main() {
    int n;
    cin >> n;
    auto *a = new long long[n];
    long long max = 0;
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        if (a[i] > max)
            max = a[i];
    }
    int k = 0;
    while (max != 0){
        max /= 2;
        k++;
    }
    MSDSort(a, 0, n, k);
    for (int i = 0; i < n; i++)
        cout << a[i] << " ";
    delete []a;
    return 0;
}
