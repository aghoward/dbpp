#pragma once

#include <iostream>

enum class RequestMethod
{
    GET,
    HEAD,
    POST,
    PUT,
    PATCH,
    DEFAULT = HEAD
};

std::ostream& operator<<(std::ostream& out, const RequestMethod& request_method);

