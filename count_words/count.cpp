#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>

//  g++ -std=c++20 count.cpp

std::map<std::string, int> counts;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        return -1;
    }
    std::string filename = argv[1];
    std::string word;
    std::ifstream infile(filename);

    for (std::string line; std::getline(infile, line);) {
        std::istringstream iss(line);
        while (std::getline(iss, word, ' ')) {
            // std::cout << word << "\n";
            if (counts.contains(word)) {
                counts[word]++;
            } else {
                counts[word] = 1;
            }
        }
    }

    // std::cout << "filename: " << filename << std::endl;
    for (const auto& elem: counts) {
        std::cout << elem.first << ": " << elem.second << "\n";
    }
    return 0;
}

// ./a.out kjvbible.txt  1.03s user 0.02s system 99% cpu 1.047 total