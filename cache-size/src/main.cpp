#include <random>
#include <ctime>
#include <vector>
#include <algorithm>
#include <iostream>

#define KB(x) ((size_t) (x) << 10)

using namespace std;

int main() {
    // https://www.zhihu.com/question/30563694
    vector<size_t> sizes_KB;
    for (auto i=1;i<18;i++)
        sizes_KB.emplace_back(1 << i);

    random_device rd;
    mt19937 gen(rd());

    for (size_t size: sizes_KB){
        uniform_int_distribution<> dis(0, KB(size) -1);
        vector<char> memory(KB(size));
        fill(memory.begin(), memory.end(), 1);

        int dummy = 0;

        clock_t begin = clock();
        for (auto i=0;i<(1<<25);i++)
            dummy += memory[dis(gen)];
        clock_t end = clock();

        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        cout << size << " KB, " << elapsed_secs << " secs, dummy: " << dummy << "\n";
    }

    return 0;
}