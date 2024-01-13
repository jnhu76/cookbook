#include <tuple>
#include <chrono>
#include <random>
#include <limits>
#include <thread>
#include <vector>
#include <iomanip>
#include <iostream>
#include <functional>

#define TIMES 10000

#if defined(_MSC_VER)
#include <intrin.h>  // for "_addcarry_u32"
#endif 

int64_t clock_realtime()
{
    return std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
}

void isleep(unsigned long millisecond)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(millisecond));
}


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
    // Suppose "unsigned"=a 32-bit type and
    // "unsigned long long"=a 64-bit type.
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
    isleep(1000);
    auto ts = clock_realtime();
    unsigned int res {0};
    for (auto i=0;i<TIMES;++i) {
        res = f(as...);
    }
    ts = clock_realtime() - ts;
    return std::make_pair(ts, res);
}

int main() {

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<unsigned> dist(1, std::numeric_limits<unsigned>::max() / 2);


    unsigned a = dist(rng), b = dist(rng);
    auto benchmark1 = measure(AverageLevel1, a, b);
    auto benchmark2 = measure(AverageLevel2, a, b);
    auto benchmark3 = measure(AverageLevel3, a, b);
    auto benchmark4 = measure(AverageLevel4, a, b);
    auto benchmark5 = measure(AverageLevel5, a, b);

    std::cout << "A=" << a << ", B=" <<  b << "\n";
    std::cout << "Benchmark: result =" << std::setw(12) << std::get<1>(benchmark1) << ", time = " << std::get<0>(benchmark1) << "\n";
    std::cout << "Benchmark: result =" << std::setw(12) << std::get<1>(benchmark2) << ", time = " << std::get<0>(benchmark2) << "\n";
    std::cout << "Benchmark: result =" << std::setw(12) << std::get<1>(benchmark3) << ", time = " << std::get<0>(benchmark3) << "\n";
    std::cout << "Benchmark: result =" << std::setw(12) << std::get<1>(benchmark4) << ", time = " << std::get<0>(benchmark4) << "\n";
    std::cout << "Benchmark: result =" << std::setw(12) << std::get<1>(benchmark5) << ", time = " << std::get<0>(benchmark5) << "\n";

    return 0;
}