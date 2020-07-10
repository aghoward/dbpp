#pragma once

#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

#include "argparsing/argparsing.h"
#include "requests/requestmethod.h"

struct Arguments
{
    bool help;
    std::string base_url;
    std::vector<std::string> request_templates;
    std::string wordlist_file;
    bool recursive;
    std::string username;
    std::string password;
    bool ignore_ssl_errors;
    std::vector<uint16_t> ignore_codes;
    std::vector<uint32_t> ignore_content_lengths;
    RequestMethod request_method;
    uint32_t thread_count;
    std::string request_body;
};

ap::ArgumentParser<Arguments> createArgumentParser();
std::ostream& operator<<(std::ostream& out, const Arguments& args);
