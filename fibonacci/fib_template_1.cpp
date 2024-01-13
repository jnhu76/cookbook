#include <iostream>

template <int I>
struct Fib {
    static const int val = Fib<I-1>::val + Fib<I-2>::val;
};

template<>
struct Fib<0>
{
    static const int val = 0;
};

template<>
struct Fib<1>
{
    static const int val = 1;
};

/*
g++ -std=c++20 -O3 ./fib_template_1.cpp
1836311903
./a.out  0.00s user 0.00s system 88% cpu 0.003 tota
*/
int main() {
    // 47 -> overflow
    std::cout << Fib<46>::val << '\n';
}