#pragma once

#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

#include "argparsing/argparsing.h"

struct Arguments
{
    std::string base_url;
    std::string extension;
    std::string wordlist_file;
    bool recursive;
    std::string username;
    std::string password;
    bool ignore_ssl_errors;
    std::vector<uint16_t> ignore_codes;
};

ap::ArgumentParser<Arguments> createArgumentParser();
std::ostream& operator<<(std::ostream& out, const Arguments& args);
