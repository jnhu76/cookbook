#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>

// 字母表
std::map<char, std::string> morse = {
    {'A', "·-"},
    {'B', "-·-"},
    {'C', "-·-·"},
    {'D', "-··"},
    {'E', "·"},
    {'F', "··-·"},
    {'G', "--·"},
    {'H', "····"},
    {'I', "··"},
    {'J', "·---"},
    {'K', "-·-"},
    {'L', "·-··"},
    {'M', "--"},
    {'N', "-·"},
    {'O', "---"},
    {'P', "·--·"},
    {'Q', "--·-"},
    {'R', "·-·"},
    {'S', "···"},
    {'T', "-"},
    {'U', "··-"},
    {'V', "···-"},
    {'W', "·--"},
    {'X', "-··-"},
    {'Y', "-·--"},
    {'Z', "--··"},

    {'1', "·----"},
    {'2', "··---"},
    {'3', "···--"},
    {'4', "····-"},
    {'5', "·····"},
    {'6', "-····"},
    {'7', "--···"},
    {'8', "---··"},
    {'9', "----·"},
    {'0', "-----"},

    {'.', "·-·-·-"},
    {':', "---···"},
    {',', "--··--"},
    {';', "-·-·-·"},
    {'?', "··--··"},
    {'=', "-···-"},
    {'\'', "·----·"},
    {'/', "-··-·"},
    {'!', "-·-·--"},
    {'-', "-····-"},
    {'_', "··--·-"},
    {'\"', "·-··-·"},
    {'(', "-·--·"},
    {')', "-·--·-"},
    {'$', "···-··-"},
    {'&', "·-···"},
    {'@', "·--·-·"},
    {'+', "·-·-·"},
};

std::map<std::string, char> morse2;

// https://stackoverflow.com/a/868894
char* getCmdOption(char** begin, char** end, const std::string& option) {
    char** iter = std::find(begin, end, option);
    if (iter != end && ++iter != end) {
        return *iter;
    }
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}

void output(std::string& o) {
    for (auto c: o) {
        std::cout << morse[toupper(c)] << " ";
    }
    std::cout << std::endl;
}

void output2(std::vector<std::string> vt) {
    for (const auto& str: vt) {
        std::cout << morse2[str];
    }
    std::cout << "\n";
}

// std::vector<std::string> split_string(std::string& str, char s) {
//     std::string tmp;
//     std::vector<std::string> result;

//     for (const auto& c: str) {
//         if (c != s) {
//             tmp += c;
//         } else {
//             if (!tmp.empty()) {
//                 result.emplace_back(tmp);
//                 tmp.erase();
//             }
//         }
//     }

//     if (!tmp.empty())
//         result.emplace_back(tmp);
//     return result;

// }

// https://stackoverflow.com/a/68397071
std::vector<std::string> split_string(std::string& str, std::string& delimiter) {
    std::vector<std::string> results;
    std::string tmp;
    size_t pos = 0;

    while ((pos = str.find(delimiter)) != std::string::npos) {
        tmp = str.substr(0, pos);
        results.emplace_back(std::move(tmp));
        str.erase(0, pos + delimiter.length());
    }
    results.emplace_back(std::move(str));
    return results;
}


int main(int argc, char** argv)
{
    for (auto iter=morse.begin(); iter!=morse.end();++iter) {
        morse2[iter->second] = iter->first;
    }

    // for (auto iter=morse2.begin(); iter!=morse2.end();++iter) {
    //     std::cout << iter->first << ": " << iter->second << "\n";
    // }

    std::string str;
    char* tmp_str;
    if (cmdOptionExists(argv, argv+argc, "-g")) {
        tmp_str = getCmdOption(argv, argv+argc, "-g");
        str = tmp_str;
        output(str);
    }

    if (cmdOptionExists(argv, argv+argc, "-e")) {
        tmp_str = getCmdOption(argv, argv+argc, "-e");
        str = tmp_str;
        std::string delimeter(" ");
        auto tt = split_string(str, delimeter);
        output2(tt);
    }

    return 0;
}
