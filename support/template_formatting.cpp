#include "support/template_formatting.h"

#include <string>
#include <map>
#include <utility>

namespace impl
{
    std::string format_single(const std::string& _template, const std::pair<std::string, std::string>& replacement)
    {
        auto result = _template;
        auto index = 0ul;
        while ((index = result.find(replacement.first)) != std::string::npos)
            result.replace(index, replacement.first.size(), replacement.second);

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
