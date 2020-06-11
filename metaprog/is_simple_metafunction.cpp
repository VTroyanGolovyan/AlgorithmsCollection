#include <iostream>

/* Template params:
 * 1) number
 * 2) is number simple now
 * 3) sqrt(number) == counter size or number is simple
 * 4) counter params
 * */
template <int num, bool isSimple, bool isEnd, int... counter>
class IsSimpleHelp {
public:
    enum {
        result = IsSimpleHelp <
                num,
                !isSimple ||
                num % sizeof...(counter) != 0,
                !isSimple || sizeof...(counter) * sizeof...(counter) >= num + 1,
                counter..., 1
        >::result
    };
};
/* number is simple*/
template <int num, int... counter>
class IsSimpleHelp <num, true, true, counter...>  {
public:
    enum {
        result = true
    };
};

/* isn't simple*/
template <int num, bool isEnd, int... counter>
class IsSimpleHelp <num, false, isEnd, counter...>  {
public:
    enum {
        result = false
    };
};

template <int num>
class IsSimple {
public:
    enum {
        result = IsSimpleHelp<num, true, false, 1, 1>::result
    };
};

int main() {
    std::cout << IsSimple<120>::result << std::endl;
    std::cout << IsSimple<11>::result << std::endl;
    std::cout << IsSimple<112>::result << std::endl;
    std::cout << IsSimple<5>::result << std::endl;
    return 0;
}
