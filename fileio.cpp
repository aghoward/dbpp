#include <fstream>
#include <string>
#include <vector>

#include "fileio.h"

std::vector<std::string> get_word_list(const std::string& file)
{
    auto words = std::vector<std::string>();
    std::ifstream fs(file);
    std::string str;

    while (std::getline(fs, str))
        words.push_back(str);
    
    return words;
}

