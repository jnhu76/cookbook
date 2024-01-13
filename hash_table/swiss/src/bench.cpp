#include <iostream>
#include <string>
#include <random>
#include <unordered_map>
#include <chrono>
#include <algorithm>
#include <vector>

// #include "swisstable.h"

static const size_t kNumEntries = 1 << 24;
static constexpr const char* DEFAULT_CHARSET = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
// std::random_device rd;
// std::mt19937 gen(rd());
// std::uniform_int_distribution<std::mt19937::result_type> dis(0, SIZE_MAX);


// // https://gist.github.com/NikolasK-source/00edef2a78177ccdc2d62f18b25793e9
// std::string generate(size_t length, const std::string& charset = "") {
//     std::string str = charset.empty() ? std::string(DEFAULT_CHARSET) : charset;

//     while (length > str.length()) str + =str;

//     std::shuffle(str.begin(), str.end(), gen);

//     return str.substr(0, length);
// }

using param_t = std::uniform_int_distribution<>::param_type;

class Bench {
public:
    Bench(size_t num_bench, size_t num_records = kNumEntries):
         num_bench(num_bench),
         num_records(num_records) {
            for (auto i=0;i<num_records;i++) {
                intdata.emplace_back(dis(gen, param_t(1, kNumEntries)));
                strdata.emplace_back(generate(12));
            }
    }

    ~Bench() {}

    void execute() {
        
    }

    void setName(const std::vector<std::string>& names) {
        bench_names = names;
    }

    void print() {
        for (auto i=0;i<num_records;i++) {
            std::cout << intdata[i] << "\t" << strdata[i] << "\n";
        }
    }

    std::string generate(size_t length, const std::string& charset = "", size_t entries = 100) {
        auto str = charset.empty() ? std::string(DEFAULT_CHARSET) : charset;
        while (length > str.length()) str += str;
        std::shuffle(str.begin(), str.end(), gen);
        return str.substr(0, length);
    }

private:

    size_t num_bench;
    size_t num_records;
    std::vector<std::string> bench_names;
    std::vector<double> timedata {};
    std::vector<size_t> intdata {};
    std::vector<std::string> strdata {};

    std::random_device rd {};
    std::mt19937 gen {rd()};
    std::uniform_int_distribution<> dis;
};


int main(int argc, char** argv) {
    // unsigned duplicates;

    // void* swisstable;
    // void* swisstableint;
    // std::unordered_map<size_t, size_t> int_map;
    // std::unordered_map<std::string, std::string> string_map;
    Bench b(1, 12);
    std::vector<std::string> names {"hello world"};
    b.setName(names);
    b.print();


    return 0;
}