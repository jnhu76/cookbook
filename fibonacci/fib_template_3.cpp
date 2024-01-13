#include <array>
#include <utility>
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
类模板 std::integer_sequence 表示一个编译时的整数序列。
在用作函数模板的实参时，能推导参数包 Ints 并将它用于包展开。
*/
template<size_t... I>
int fib_impl(std::index_sequence<I...>, const int i) {
    constexpr std::array<int, sizeof...(I)> a
        = { Fib<I>::val ...};
    return a[i];
}

// https://www.cnblogs.com/happenlee/p/14219925.html
const int fib(const int i) {
    return fib_impl(std::make_index_sequence<47>(), i);
}

/*
g++ -std=c++20 -O3 ./fib_template_3.cpp
1836311903
./a.out  0.00s user 0.00s system 86% cpu 0.001 total
*/
int main() {

    std::cout << Fib<46>::val << '\n';
}