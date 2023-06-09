#include <cmath>
#include <iostream>

const size_t fib(const size_t i) {
    constexpr auto sqrt_5 = std::sqrt(5);
    if (i == 0) return 0;
    if (i == 1) return 1;

    return static_cast<size_t>((std::pow(1 + sqrt_5, i) - std::pow(1 - sqrt_5, i)) / (std::pow(2, i) * sqrt_5));
}

/*
g++ -std=c++20 -O3 ./fib_math.cpp
1836311903
./a.out  0.00s user 0.00s system 87% cpu 0.003 total
*/
int main() {
    std::cout << fib(46) << '\n';
    return 0;
}