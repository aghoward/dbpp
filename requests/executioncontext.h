#pragma once

#include <cstdio>
#include <string>
#include <vector>

#include "requests/requestfactory.h"
#include "multi-threading/logger.h"

struct ExecutionContext
{
    std::string base_url;
    std::string extension;
    std::vector<uint16_t> ignored_status_codes;

    RequestFactory request_factory;
    Logger logger;

    ExecutionContext(
        const std::string& url,
        const std::string& ext,
        const RequestFactory& rf,
        const std::vector<uint16_t>& status_codes)
        : base_url(url),
        extension(ext),
        ignored_status_codes(status_codes),
        request_factory(rf),
        logger()
    {}
};


