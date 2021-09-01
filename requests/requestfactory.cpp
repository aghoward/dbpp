#include "requestfactory.h"

#include <map>
#include <string>
#include <functional>

#include <cpr/cpr.h>

#include "requests/requestmethod.h"
#include "support/template_formatting.h"

cpr::Response RequestFactory::make_request(
        const std::string& url,
        const std::string& data,
        const std::map<std::string, std::string>& templates)
{
    return _make_request_with_data(
            data,
            cpr::Url(url),
            cpr::VerifySsl(_verify_ssl),
            _create_header(templates));
}

cpr::Header RequestFactory::_create_header(const std::map<std::string, std::string>& templates) const
{
    using namespace std::string_literals;

    std::map<std::string, std::string> map = {};
    for (const auto& header : _headers)
        map[format_template(header.name, templates)] = format_template(header.value, templates);

    if (_content_type != ""s)
        map["Content-Type"s] = _content_type;

    return cpr::Header(map.begin(), map.end());
}
