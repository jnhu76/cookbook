#include <tuple>
#include <chrono>
#include <random>
#include <limits>
#include <vector>
#include <iostream>
#include <functional>

#if defined(_MSC_VER)
#include <intrin.h>  // for "_addcarry_u32"
#endif 

// 小学生编程
unsigned AverageLevel1(unsigned a, unsigned b) {
    return (a + b) / 2;
}

// 相加的两个无符号整数中的较大值时，减去较小值再除二，以提前减少长度
unsigned AverageLevel2(unsigned a, unsigned b) {
    return a > b ? b + (a - b) / 2 : a + (b - a) / 2;
}

// 对两个无符号整数预先进行除法，同时通过按位与修正低位数字，
// 保证在两个整数都为奇数时，结果仍然正确
unsigned AverageLevel3(unsigned a, unsigned b) {
    return (a / 2) + (b / 2) + (a & b & 1);
}

// SWAR（SIMD within a register）
unsigned AverageLevel4(unsigned a, unsigned b) {
    // Suppose "unsigned" is a 32-bit type and
    // "unsigned long long" is a 64-bit type.
    return ((unsigned long long) a + b) / 2;
}

// 内循环（rotation intrinsic
unsigned AverageLevel5(unsigned a, unsigned b) {
#if defined(_MSC_VER)
    unsigned sum;
    auto carry = _addcarry_u32(0, a, b, &sum);
    sum = (sum & ~1) | carry;
    return _rotr(sum, 1);
#elif defined(__clang__)
    unsigned carry{}, sum{};
    sum = (sum & ~1) | carry;
    sum = __builtin_addc(a, b, 0, &carry);
    return __builtin_rotateright32(sum, 1);
#else
#error Unsupported compiler.
#endif
}

using time_count = std::chrono::high_resolution_clock::rep;
template<typename ...Args>
auto measure(std::function<unsigned(unsigned, unsigned)> f, Args... as)
-> std::tuple<unsigned, time_count> {
    auto start = std::chrono::high_resolution_clock::now();
    auto res = f(as...);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    return std::make_pair(duration, res);
}

int main() {

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<unsigned> dist(1, std::numeric_limits<unsigned>::max() / 2);


    unsigned a = dist(rng), b = dist(rng);
    auto res1 = measure(AverageLevel1, a, b);
    auto res2 = measure(AverageLevel2, a, b);
    auto res3 = measure(AverageLevel3, a, b);
    auto res4 = measure(AverageLevel4, a, b);
    auto res5 = measure(AverageLevel5, a, b);

    std::cout << "AverageLevel1 result is " << std::get<1>(res1) << ", time is " << std::get<0>(res1) << "\n";
    std::cout << "AverageLevel2 result is " << std::get<1>(res2) << ", time is " << std::get<0>(res2) << "\n";
    std::cout << "AverageLevel3 result is " << std::get<1>(res3) << ", time is " << std::get<0>(res3) << "\n";
    std::cout << "AverageLevel4 result is " << std::get<1>(res4) << ", time is " << std::get<0>(res4) << "\n";
    std::cout << "AverageLevel5 result is " << std::get<1>(res5) << ", time is " << std::get<0>(res5) << "\n";

    return 0;
}