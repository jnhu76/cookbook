#include <string>
#include <iostream>
#include <vector>
#include <climits>
#include <random>

// std::random_device rd;
// std::mt19937 gen(rd());
// std::uniform_int_distribution<int> dist(0, INT_MAX);

// https://stackoverflow.com/a/28329163
using param_t = std::uniform_int_distribution<>::param_type;

class Test {
public:
    Test(size_t length = 20): length(length) {
        // std::mt19937 gen(rd());
        // std::uniform_int_distribution<int> dist(0, INT_MAX);

        for (auto i=0;i<length;i++)
            data.push_back(connections_dist(gen, param_t(0, 33)));
    }

    void print() {
        for (auto i=0;i<length;i++)
            std::cout << data[i] << "\n";
        std::cout << std::endl;
    }
private:
    size_t length;
    std::vector<int> data {};

    std::random_device rd {};
    std::mt19937 gen {rd()};
    std::uniform_int_distribution<int> connections_dist;
};

int main() {
    Test test(20);
    test.print();
    return 0;
}