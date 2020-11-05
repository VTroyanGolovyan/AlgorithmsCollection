#include <iostream>

/* Трюк для выполнения попыток создать объект */
template <typename...>
using void_t = void;

template <typename Void, typename T, typename... Args>
struct my_is_constructible : std::false_type {};

/* тут все также как с is_constructible только пытаемся обратьться к функции-члену. */
template <typename T, typename... Args>
struct my_is_constructible <
        void_t <
           decltype(T::member(std::declval<Args>()...))
        >,
        T,
        Args...
> : std::true_type {
};

/* компилятор будет пытаться применить специализацию (когда T(std::declval<Args>()...) работает он сможет это сделать)
 * Если данная конструкция не валидна, компилятор будет использовать более общий случай */
template <typename T, typename... Args>
using is_constructible = my_is_constructible<void_t<>, T, Args...>;

class Test {
public:
    explicit Test(int x) {
    }
    static void member(int z){
        std::cout << z;
    }
};

int main() {
    std::cout << is_constructible<Test, int>() << std::endl;
    std::cout << is_constructible<Test, std::string>() << std::endl;
    return 0;
}
