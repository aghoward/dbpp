#pragma once

#include <string>

#include <cpr/cpr.h>

class RequestFactory
{
    private:
        std::string _user;
        std::string _pass;

    public:
        RequestFactory()
            : _user(), _pass() {}

        RequestFactory(const std::string& user, const std::string& pass)
            : _user(user), _pass(pass) {}


        cpr::Response make_request(const std::string& url) const;
};
