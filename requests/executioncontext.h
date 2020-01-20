#pragma once

#include <cstdio>
#include <string>
#include <vector>

#include "requests/requestfactory.h"
#include "multi-threading/logger.h"

struct ExecutionContext
{
    std::string base_url;
    std::vector<std::string> request_templates;
    std::vector<uint16_t> ignored_status_codes;

    RequestFactory request_factory;
    Logger logger;

    ExecutionContext(
        const std::string& url,
        const std::vector<std::string>& rts,
        const RequestFactory& rf,
        const std::vector<uint16_t>& status_codes)
        : base_url(url),
        request_templates(rts),
        ignored_status_codes(status_codes),
        request_factory(rf),
        logger()
    {}
};


