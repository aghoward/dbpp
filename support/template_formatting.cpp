#include "support/template_formatting.h"

#include <string>
#include <map>
#include <utility>

namespace impl
{
    std::string format_single(const std::string& _template, const std::pair<std::string, std::string>& replacement)
    {
        auto result = _template;
        auto index = result.find(replacement.first);
        while (index != std::string::npos)
        {
            auto replacement_end_index = index + replacement.first.size() - 1;
            auto end_index = result.size() - 1;
            auto end = result.substr(replacement_end_index + 1, end_index - replacement_end_index);
            auto begin = result.substr(0, index);
            result = begin + replacement.second + end;
            index = result.find(replacement.first);
        }

        return result;
    }
}

std::string format_template(const std::string& _template, const std::map<std::string, std::string>& replacements)
{
    auto result = _template;
    for (const auto& replacement : replacements)
        result = impl::format_single(result, replacement);
    return result;
} 
