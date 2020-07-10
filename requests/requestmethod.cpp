#include "requests/requestmethod.h"

#include <iostream>

std::ostream& operator<<(std::ostream& out, const RequestMethod& request_method)
{
    if (request_method == RequestMethod::HEAD)
        return out << "HEAD";
    if (request_method == RequestMethod::GET)
        return out << "GET";
    if (request_method == RequestMethod::POST)
        return out << "POST";
    return out << "Unknown";
}
