#include <iostream>
#include <stack>
#include <utility>
#include <cstdlib>
#include <ctime>

using std::cin;
using std::cout;
using std::stack;
using std::pair;
using std::endl;
using std::rand;
using std::clock;

void swap(int &a, int &b) {
    if (a == b)
        return;
    a = a ^ b;
    b = a ^ b;
    a = a ^ b;
}

int median (int a, int b, int c) {
    if (a > b) swap (a, b) ;
    if (b > c) swap (b, c) ;
    return b;
}

int randNum(int l, int r) {
    return rand() % (r - l + 1) + l;
}

void copy(int* a, int* b, int n) {
    for (int i = 0; i < n; i++)
        a[i] = b[i];
}

void insertSort(int* a, int &l, int &r) {
    for (int i = l + 1; i <= r; i++) {
        int j = i;
        int x = a[i];
        while (j > l && x < a[j - 1]) {
            a[j] = a[j - 1];
            j--;
        }
        a[j] = x;
    }
}

int partition(int *a, int &l, int &r) {
    int piv = a[l + (r - l) / 2]; //Оказалось найболее эффективно
    //int piv = a[randNum(l,r)]; //Плохо оптимизирует
    //int piv = median(a[l],  a[l + (r - l) / 2], a[r]); // +- так же по эффективности как выбор центрального элемента.
    int i = l - 1;
    int j = r + 1;
    do {
        do {
            i++;
        } while(a[i] < piv);
        do {
            j--;
        } while(a[j] > piv);
        if (i >= j)
            return j;
        swap(a[i],a[j]);
    } while (true);
}

void StandartQsort(int *a, int l, int r) {
    if (l < r) {
        int p = partition(a, l, r);
        StandartQsort(a, l, p);
        StandartQsort(a, p + 1 , r);
    }
}

void QsortWithOneRecursionBranch(int *a, int l, int r) {
    while (l < r) {
        int p = partition(a, l, r);
        if (p > l + ( r - l ) / 2) {
            QsortWithOneRecursionBranch(a, p + 1, r);
            r = p;
        } else {
            QsortWithOneRecursionBranch(a, l, p);
            l = p + 1;
        }
    }
}

void QsortWithoutRecursion(int *a, int l, int r) {
    stack < pair<int, int> > st;
    st.push(pair<int, int> (l, r));
    while (!st.empty()) {
        pair <int, int> t = st.top();
        st.pop();
        if (t.first < t.second) {
            int p = partition(a, t.first, t.second);
            st.push(pair<int, int> (t.first, p));
            st.push(pair<int, int> (p + 1, t.second));
        }
    }
}

void QsortWithInsertionSortOptimization(int *a, int l, int r) {
    while (l < r) {
        if (r - l < 32){
            insertSort(a, l ,r);
            l = r;
        } else {
            int p = partition(a, l, r);
            if (p > l + ( r - l ) / 2) {
                QsortWithInsertionSortOptimization(a, p + 1, r);
                r = p;
            } else {
                QsortWithInsertionSortOptimization(a, l, p);
                l = p + 1;
            }
        }
    }
}

void randomTestGenerator(int* a, int n) {
    for (int i = 0; i < n; i++) {
        a[i] = randNum(0,50000);
    }
}

int main() {
    int n;
    int k;
    cout << "Enter array size and tests count:" << endl;
    cin >> n >> k;
    cout << endl;
    int *b = new int[n];
    int *a = new int[n];
    double s1 = 0;
    double s2 = 0;
    double s3 = 0;
    double s4 = 0;

    cout << "Qsort" << "|" << "One branch" << "|" << "No recursively" << "|" << "One branch + insertSort" << " " << endl;

    for (int i = 0; i < k; i++) {

        srand(time(nullptr) + i);
        clock_t start;
        clock_t end;

        randomTestGenerator(b, n); // генерируем тест

        copy(a, b, n);
        start = clock();
        StandartQsort(a, 0, n - 1); //обычная рекурсивная реализация
        end = clock();
        double time1 = (((double)end / (double)CLOCKS_PER_SEC) - ((double)start / (double)CLOCKS_PER_SEC));

        copy(a, b, n);
        start = clock();
        QsortWithOneRecursionBranch(a, 0, n - 1); //одна ветка рекурсии - работает чуть чуть быстрее чем с двумя ветками
        end = clock();
        double time2 = (((double)end / (double)CLOCKS_PER_SEC) - ((double)start / (double)CLOCKS_PER_SEC));

        copy(a, b, n);
        start = clock();
        QsortWithoutRecursion(a, 0, n - 1); //без рекурсии - работает очень медленно.
        end = clock();
        double time3 = (((double)end / (double)CLOCKS_PER_SEC) - ((double)start / (double)CLOCKS_PER_SEC));

        copy(a, b, n);
        start = clock();
        QsortWithInsertionSortOptimization(a, 0, n - 1); //сортировка с оптимизацией вставками и одной веткой рекурсии, самый оптимальный алгоритм который я смог сделать
        end = clock();
        double time4 = (((double)end / (double)CLOCKS_PER_SEC) - ((double)start / (double)CLOCKS_PER_SEC));

        cout << time1 << "     " << time2 << "     " << time3 << "     " << time4 << " " << endl;
        s1+= time1;
        s2+= time2;
        s3+= time3;
        s4+= time4;

    }

    double sa1 = s1 / k;
    double sa2 = s2 / k;
    double sa3 = s3 / k;
    double sa4 = s4 / k;

    cout << "Average Time" << endl;
    cout << "Qsort" << "|" << "One branch" << "|" << "No recursively" << "|" << "One branch + insertSort" << " " << endl;
    cout << sa1 << "     " << sa2 << "     " << sa3 << "     " << sa4 << " ";

    delete []a;
    delete []b;
    return 0;
}
