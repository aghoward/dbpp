#pragma once

#include <string>
#include <map>
#include <utility>

std::string format_template(const std::string& _template, const std::map<std::string, std::string>& replacements);
