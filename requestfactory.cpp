#include "requestfactory.h"

#include <string>

#include <cpr/cpr.h>

cpr::Response RequestFactory::make_request(const std::string& url) const
{
    if (_user == "")
        return cpr::Get(cpr::Url(url));
    return cpr::Get(cpr::Url(url), cpr::Authentication(_user, _pass));
}
