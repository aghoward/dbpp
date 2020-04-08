#pragma once

#include <string>
#include <functional>

#include <cpr/cpr.h>

#include "requests/requestmethod.h"

class RequestFactory
{
    private:
        std::string _user;
        std::string _pass;
        bool _verify_ssl;
        RequestMethod _request_method;

        template <typename ... TArgs>
        cpr::Response _make_request(TArgs&&... args) const
        {
            using namespace std::string_literals;

            if (_request_method == RequestMethod::HEAD)
                return cpr::Head(args...);
            if (_request_method == RequestMethod::GET)
                return cpr::Get(args...);
            if (_request_method == RequestMethod::POST)
                return cpr::Post(args...);
            return cpr::Head(args...);
        }

    public:
        RequestFactory()
            : _user(), _pass(), _request_method(RequestMethod::HEAD) {}

        RequestFactory(
                const std::string& user,
                const std::string& pass,
                bool verify_ssl,
                const RequestMethod& request_method)
            :
                _user(user),
                _pass(pass),
                _verify_ssl(verify_ssl),
                _request_method(request_method)
        {}

        cpr::Response make_request(const std::string& url) const;
        cpr::Response make_request(const std::string& url, const std::string& body_data) const;
};
