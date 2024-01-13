#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <utility>

//  g++ -std=c++20 count.cpp

std::map<std::string, int> counts;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        return -1;
    }
    std::string filename = argv[1];
    std::string word;
    std::ifstream infile(filename);

    // while (std::getline(infile, word, ' ')) {
    //     counts[word]++;
    // }
    // https://stackoverflow.com/a/19123667
    while (infile >> word) {
        counts[word]++;
    }

    // std::cout << "filename: " << filename << std::endl;
    // for (const auto& elem: counts) {
    //     std::cout << elem.first << ": " << elem.second << "\n";
    // }

    // orderd map.
    std::vector<std::pair<std::string, int>> ordered(counts.begin(), counts.end());

    std::sort(ordered.begin(), ordered.end(),
    [](auto const& a, auto const& b) {return a.second > b.second;});

    for (auto const& elem: ordered) {
        std::cout << elem.first << ": " << elem.second << "\n";
    }

    return 0;
}

// ./a.out kjvbible.txt  1.03s user 0.02s system 99% cpu 1.047 total
// ./a.out kjvbible.txt  0.90s user 0.02s system 99% cpu 0.925 total
// ./a.out kjvbible.txt  0.50s user 0.01s system 99% cpu 0.509 total