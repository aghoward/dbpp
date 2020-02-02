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
