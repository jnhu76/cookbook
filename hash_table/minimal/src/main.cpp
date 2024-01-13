#include <iostream>
#include <cstdint>
#include <cassert>

using namespace std;

auto phf_shift(uint32_t x) -> uint8_t {
    uint32_t shift = (x * 0xa463293eULL) % (1ULL << 32);
    uint8_t res = static_cast<uint8_t>((0x824a1847ULL >> shift) & 0b11111);
    return res;
}

int main(int argc, char** argv)
{
    cout << "hello world!" << endl;
    uint32_t x = phf_shift(32145215);
    assert( x == 18);
    return 0;
}
