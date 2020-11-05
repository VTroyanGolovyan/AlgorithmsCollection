#include <iostream>
#include <cmath>
using std::cin;
using std::endl;
using std::cout;
using std::pow;
int digit(unsigned long long n, unsigned long long k) {
    cout<<n<<" "<<k;
    return n & k == k ? 1 : 0;
}
void radixSort(unsigned long long *a, int n, int m){
    int* c = new int[2];
    unsigned long long* b = new unsigned long long[n];
    int bin = 1;
    for(int i = 0; i < m; i++) {
        for (int j = 0; j < 2; j++) {
            c[j] = 0;
        }
        for (int j = 0; j < n; j++) {
            c[digit(a[j], bin)]++;
        }
        cout<<c[0]<<" "<<c[1]<<endl;
        int count = 0;
        for (int j = 0; j < 2; j++) {
            int t = c[j];
            c[j] = count;
            count += t;
        }
        for (int j = 0; j < n; j++) {
            b[c[digit(a[j], bin)]++] = a[j];
        }
        for (int j = 0; j < n; j++) {
            a[j] = b[j];
            cout<<a[j]<<" ";
        }
        cout<<endl;
        bin*=2;
        cout<<bin<<endl;
    }
    delete []b;
    delete []c;
}
int main() {
    int n;
    cin>>n;
    unsigned long long* a = new unsigned long long[n];
    for (int i = 0; i < n; i++) {
        cin >> a[i];

    }
    radixSort(a,n,32);
    for (int i = 0; i < n; i++)
        cout << a[i] << " ";
    return 0;
}
