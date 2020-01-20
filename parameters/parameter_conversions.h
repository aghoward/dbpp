#pragma once

#include <cstdio>
#include <functional>
#include <string>
#include <vector>

#include "cdif/cdif.h"

namespace impl {
    std::vector<uint16_t> parse_status_code_list(const std::string& parameter, std::function<uint16_t(std::string)> converter);
    std::vector<std::string> parse_request_template_list(const std::string& parameter);
}

std::function<std::vector<uint16_t>(const std::string&)> status_code_parser_factory(const cdif::Container& ctx);
std::function<std::vector<std::string>(const std::string&)> request_template_parser_factory(const cdif::Container& ctx);
