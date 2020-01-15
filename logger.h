#pragma once

#include <iostream>
#include <cstdio>
#include <mutex>
#include <shared_mutex>
#include <string>

class Logger
{
    private:
        mutable std::shared_mutex _mutex;
        uint16_t line_length;

    public:
        void log_line(const std::string& msg);
        void log(const std::string& msg);
        void clear_line();
};

