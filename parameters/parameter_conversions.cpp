#include <algorithm>
#include <cstdio>
#include <functional>
#include <iterator>
#include <numeric>
#include <string>
#include <vector>

#include "parameters/parameter_conversions.h"

namespace impl {
    template <typename It, typename TItem>
    std::vector<TItem> parse_range(const It& begin, const It& end, const std::function<TItem(std::string)>& converter)
    {
        auto token_it = std::find_if(begin, end, [] (const auto& c) { return c == '-'; });

        auto lhs = converter({begin, token_it});
        auto rhs = converter({token_it + 1, end});

        auto min = std::min(lhs, rhs);
        auto max = std::max(lhs, rhs);
        auto size = max - min;
        auto result = std::vector<TItem>(static_cast<uint32_t>(size));

        std::iota(std::begin(result), std::end(result), min);
        return result;
    }

    template <typename It, typename TItem>
    std::vector<TItem> parse_values(const It& begin, const It& end, const std::function<TItem(std::string)>& converter)
    {
        auto hyphen_it = std::find_if(begin, end, [] (const auto& c) { return c == '-'; });
        if (hyphen_it != end)
            return parse_range<decltype(begin), TItem>(begin, end, converter);
        return { converter({begin, end}) };
    }

    template <typename TItem>
    std::vector<TItem> parse_status_code_list(const std::string& parameter, std::function<TItem(std::string)> converter)
    {
        auto result = std::vector<TItem>();
        auto comma_it = std::find_if(std::begin(parameter), std::end(parameter), [] (const auto& c) { return c == ','; });
        auto begin = std::begin(parameter);
        while (comma_it != std::end(parameter))
        {
            auto elements = parse_values<decltype(begin), TItem>(begin, comma_it, converter);
            result.insert(std::end(result), std::begin(elements), std::end(elements));

            begin = comma_it + 1;
            comma_it = std::find_if(begin, std::end(parameter), [] (const auto& c) { return c == ','; });
        }

        auto elements = parse_values<decltype(begin), TItem>(begin, std::end(parameter), converter);
        result.insert(std::end(result), std::begin(elements), std::end(elements));

        return result;
    }

    std::vector<std::string> parse_request_template_list(const std::string& parameter)
    {
        auto result = std::vector<std::string>();
        auto comma_it = std::find_if(std::begin(parameter), std::end(parameter), [] (const auto& c) { return c == ','; });
        auto begin = std::begin(parameter);
        while (comma_it != std::end(parameter))
        {
            auto element = std::string(begin, comma_it);
            result.push_back(element);

            begin = comma_it + 1;
            comma_it = std::find_if(begin, std::end(parameter), [] (const auto& c) { return c == ','; });
        }

        auto element = std::string(begin, comma_it);
        result.push_back(element);

        return result;
    }
}

std::function<std::vector<uint16_t>(const std::string&)> status_code_parser_factory(const cdif::Container& ctx)
{
    return [&] (const std::string& parameter)
    {
        return impl::parse_status_code_list<uint16_t>(
                parameter,
                ctx.resolve<std::function<uint16_t(std::string)>>());
    };
}

std::function<std::vector<uint32_t>(const std::string&)> content_length_parser_factory(const cdif::Container& ctx)
{
    return [&] (const std::string& parameter)
    {
        return impl::parse_status_code_list<uint32_t>(
                parameter,
                ctx.resolve<std::function<uint32_t(std::string)>>());
    };
}


std::function<std::vector<std::string>(const std::string&)> request_template_parser_factory(const cdif::Container&)
{
    return impl::parse_request_template_list;
}
