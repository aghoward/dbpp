#include "requestfactory.h"

#include <string>
#include <functional>

#include <cpr/cpr.h>

#include "requests/requestmethod.h"

cpr::Response RequestFactory::make_request(const std::string& url) const
{
    if (_user == "")
        return _make_request(cpr::Url(url), cpr::VerifySsl(_verify_ssl));
    return _make_request(cpr::Url(url), cpr::Authentication(_user, _pass), cpr::VerifySsl(_verify_ssl));
}

cpr::Response RequestFactory::make_request(const std::string& url, const std::string& body_data) const
{
    using namespace std::string_literals;

    if (_user == ""s)
        return cpr::Post(
            cpr::Url(url),
            cpr::VerifySsl(_verify_ssl),
            cpr::Body(body_data),
            cpr::Header{{"Content-Type"s, _content_type}});
    return cpr::Post(
        cpr::Url(url),
        cpr::VerifySsl(_verify_ssl),
        cpr::Authentication(_user, _pass),
        cpr::Body(body_data),
        cpr::Header{{"Content-Type"s, _content_type}});
}

