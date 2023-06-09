#include <iostream>

template <size_t I>
struct Fib {
    static const size_t val = Fib<I-1>::val + Fib<I-2>::val;
};

template<>
struct Fib<0>
{
    static const size_t val = 0;
};

template<>
struct Fib<1>
{
    static const size_t val = 1;
};

/*
g++ -std=c++20 -O3 ./fib_template_2.cpp
1836311903
./a.out  0.00s user 0.00s system 88% cpu 0.003 total
*/
int main() {

    // 50 -> overflow
    // warning: integer overflow in expression of type ‘int’ results in ‘-1323752223’ [-Woverflow]
    // change int to size_t
    std::cout << Fib<46>::val << '\n';
}