#include <iostream>
#include <cmath>

int getDigit(int i, int a){
    int d = std::pow(10,i + 1);
    int d2 = d/10;
    return a % d / d2;
}
void LSD(unsigned long long *a,int n, int k) {
    int *c = new int[10];
    unsigned long long *b = new unsigned long long[n];
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < 10; j++)
            c[j] = 0;
        for (int j = 0; j < n; j++){
            c[getDigit(i, a[j])]++;
        }
        for (int j = 1; j < 10; j++) {
            c[j]+=c[j-1];
        }
        for (int j = n - 1; j >= 0; j--) {
            b[c[getDigit(i, a[j])]-1] = a[j];
            c[getDigit(i, a[j])]--;
        }
        for (int j = 0; j < n; j++){
            a[j] = b[j];
        }
    }
    delete []b;
    delete []c;
}

int main() {
    int n;
    std::cin>>n;
    auto *a = new unsigned long long[n];
    int k = 1;
    unsigned long long t = 10;
    for (int i = 0; i < n; i++) {
        std::cin>>a[i];
        while (a[i] >= t ){
            k++;
            t*=10;
        }
    }
    LSD(a,n,k);
    for (int i = 0; i < n; i++){
        std::cout<<a[i]<<" ";
    }
    return 0;
}
