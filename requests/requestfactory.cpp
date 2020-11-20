#include "requestfactory.h"

#include <map>
#include <string>
#include <functional>

#include <cpr/cpr.h>

#include "requests/requestmethod.h"

cpr::Response RequestFactory::make_request(const std::string& url) const
{
    if (_user == "")
        return _make_request(cpr::Url(url), cpr::VerifySsl(_verify_ssl), _create_header());
    return _make_request(cpr::Url(url), cpr::Authentication(_user, _pass), cpr::VerifySsl(_verify_ssl), _create_header());
}

cpr::Response RequestFactory::make_request(const std::string& url, const std::string& body_data) const
{
    using namespace std::string_literals;

    if (_user == ""s)
        return cpr::Post(
            cpr::Url(url),
            cpr::VerifySsl(_verify_ssl),
            cpr::Body(body_data),
            _create_header());
    return cpr::Post(
        cpr::Url(url),
        cpr::VerifySsl(_verify_ssl),
        cpr::Authentication(_user, _pass),
        cpr::Body(body_data),
        _create_header());
}

cpr::Header RequestFactory::_create_header() const
{
    using namespace std::string_literals;

    std::map<std::string, std::string> map = {};
    for (const auto& header : _headers)
        map[header.name] = header.value;

    if (_content_type != ""s)
        map["Content-Type"s] = _content_type;

    return cpr::Header(map.begin(), map.end());
}

