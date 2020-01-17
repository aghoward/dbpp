#pragma once

#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric>
#include <string>
#include <vector>
#include <cstdio>

#include "cdif/cdif.h"

namespace impl {
    template <typename It>
    std::vector<uint16_t> parse_range(const It& begin, const It& end, const std::function<uint16_t(std::string)>& converter)
    {
        auto token_it = std::find_if(begin, end, [] (const auto& c) { return c == '-'; });

        auto lhs = converter({begin, token_it});
        auto rhs = converter({token_it + 1, end});

        auto min = std::min(lhs, rhs);
        auto max = std::max(lhs, rhs);
        auto size = max - min;
        auto result = std::vector<uint16_t>(static_cast<uint32_t>(size));

        std::iota(std::begin(result), std::end(result), min);
        return result;
    }

    template <typename It>
    std::vector<uint16_t> parse_values(const It& begin, const It& end, const std::function<uint16_t(std::string)>& converter)
    {
        auto hyphen_it = std::find_if(begin, end, [] (const auto& c) { return c == '-'; });
        if (hyphen_it != end)
            return parse_range(begin, end, converter);
        return { converter({begin, end}) };
    }

    std::vector<uint16_t> parse_status_code_list(const std::string& parameter, std::function<uint16_t(std::string)> converter)
    {
        auto result = std::vector<uint16_t>();
        auto comma_it = std::find_if(std::begin(parameter), std::end(parameter), [] (const auto& c) { return c == ','; });
        auto begin = std::begin(parameter);
        while (comma_it != std::end(parameter))
        {
            auto elements = parse_values(begin, comma_it, converter);
            result.insert(std::end(result), std::begin(elements), std::end(elements));

            begin = comma_it + 1;
            comma_it = std::find_if(begin, std::end(parameter), [] (const auto& c) { return c == ','; });
        }

        auto elements = parse_values(begin, std::end(parameter), converter);
        result.insert(std::end(result), std::begin(elements), std::end(elements));

        return result;
    }
}

class ArgparsingModule : public cdif::IModule
{
    public:
        void load(cdif::Container& ctx) override
        {
            ctx.bind<std::vector<uint16_t>, std::string>(
                [&ctx] (const auto& parameter) {
                    return impl::parse_status_code_list(parameter, ctx.template resolve<std::function<uint16_t(std::string)>>());
                })
                .build();
        }
};
