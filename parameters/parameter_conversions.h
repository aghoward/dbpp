#pragma once

#include <cstdio>
#include <functional>
#include <string>
#include <vector>

#include "cdif/cdif.h"
#include "requests/requestmethod.h"

std::function<std::vector<uint16_t>(const std::string&)> status_code_parser_factory(const cdif::Container& ctx);
std::function<std::vector<uint32_t>(const std::string&)> content_length_parser_factory(const cdif::Container& ctx);
std::function<std::vector<std::string>(const std::string&)> request_template_parser_factory(const cdif::Container& ctx);
std::function<RequestMethod(const std::string&)> request_method_parser_factory(const cdif::Container& ctx);
