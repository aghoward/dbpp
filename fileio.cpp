#include <fstream>
#include <string>
#include <vector>

#include "fileio.h"

std::vector<std::string> get_word_list(const std::string& file)
{
    using namespace std::string_literals;

    auto words = std::vector<std::string>();
    std::ifstream fs(file);
    std::string str;

    while (std::getline(fs, str))
        if (str != ""s)
            words.push_back(str);
    
    return words;
}

