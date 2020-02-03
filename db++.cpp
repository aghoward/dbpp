#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "argparsing/argparsing.h"
#include "requests/requestexecutor.h"

int main(int argc, const char * argv[])
{
    auto parser = createArgumentParser();
    auto argresult = parser.parse(argc, argv);

    return argresult.match(
        [] (const auto& args) {
            std::cout << args << std::endl;

            auto request_executor = RequestExecutor(args);

            if (args.recursive)
                request_executor.recursive_search();
            else
                request_executor.search();

            return 0;
        },
        [&] (const auto& error) {
            std::cerr << parser.usage(argv[0]) << std::endl << std::endl;
            std::cerr << parser.get_error_message(error) << std::endl;

            return 1;
        });
}
