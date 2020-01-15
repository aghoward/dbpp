#include "logger.h"

#include <iostream>
#include <shared_mutex>
#include <string>

void Logger::log_line(const std::string& msg)
{
    std::unique_lock<std::shared_mutex> lock(_mutex);
    if (line_length != 0u)
        clear_line();

    std::cout << msg << std::endl;
    line_length = 0u;
}

void Logger::log(const std::string& msg)
{
    std::unique_lock<std::shared_mutex> lock(_mutex);
    if (line_length != 0u)
        clear_line();

    std::cout << msg << std::flush;
    line_length = msg.size();
}

void Logger::clear_line()
{
    std::cout << std::string(line_length, ' ') << '\r' << std::flush;
}
