#include <iostream>

template <typename T, size_t size = 1000>
class StackAllocator {
public:
    T mem[size]; //массив
    T* used;
    StackAllocator() {
        used = mem;
    }
    StackAllocator(const StackAllocator<T>& other) {
        used = other.used;
        mem = other.mem;
    }

    T* allocate(std::size_t n){
        used += n; //заняли память
        return used - n; //вернули указатель на начало выделенного куска
    }

    void deallocate(T* p, size_t n) {
        if (p + n == used) { //Проверяем выполняется ли условие LIFO (стековый же аллокатор)
            used -= n; //освободили место
        }
    }
};
int main() {
    /* выделяем 2 независимых куска */
    StackAllocator<int> a;
    int* x = a.allocate(2);
    *x = 2;
    *(x + 1) = 3;
    std::cout << *(x + 1) << std::endl;
    int* y = a.allocate(2);
    *y = 21;
    *(y + 1) = 3;
    std::cout << *(x + 1) << std::endl;
    std::cout << *(y) << std::endl;
    return 0;
}
