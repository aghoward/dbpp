#include "logger.h"

#include <iostream>
#include <shared_mutex>
#include <string>

void Logger::log(const std::string& msg)
{
    std::unique_lock<std::shared_mutex> lock(_mutex);
    std::cout << msg << std::endl;
}
