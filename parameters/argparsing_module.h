#pragma once

#include <cstdio>
#include <functional>
#include <string>
#include <vector>

#include "cdif/cdif.h"

namespace impl {
    std::vector<uint16_t> parse_status_code_list(const std::string& parameter, std::function<uint16_t(std::string)> converter);
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
