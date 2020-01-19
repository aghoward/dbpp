#pragma once

#include <string>

#include <cpr/cpr.h>

class RequestFactory
{
    private:
        std::string _user;
        std::string _pass;
        bool _verify_ssl;

    public:
        RequestFactory()
            : _user(), _pass() {}

        RequestFactory(const std::string& user, const std::string& pass, bool verify_ssl)
            : _user(user), _pass(pass), _verify_ssl(verify_ssl) {}

        cpr::Response make_request(const std::string& url) const;
};
