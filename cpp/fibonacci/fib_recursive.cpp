#include <iostream>

int fib(const int i) {
    if (i == 0)
        return 0;
    if (i == 1)
        return 1;
    return fib(i-1) + fib(i-2);
}

/*
g++ -std=c++20 -O3 ./fib_recursive.cpp
1836311903
./a.out  2.66s user 0.00s system 99% cpu 2.657 total
*/
int main() {
    std::cout << fib(46) << '\n';
    return 0;
}