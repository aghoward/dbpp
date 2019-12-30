#pragma once

#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <string>

class Logger
{
    private:
        mutable std::shared_mutex _mutex;

    public:
        void log(const std::string& msg);
};

