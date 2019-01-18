#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <cpr/cpr.h>

#include "argparsing/argparsing.h"

struct Arguments
{
    std::string base_url;
    std::string wordlist_file;
    bool recursive;
};

std::ostream& operator<<(std::ostream& out, const Arguments& args)
{
    out << "base_url: " << args.base_url << '\n';
    out << "wordlist_file: " << args.wordlist_file << '\n';
    out << "recursive: " << (args.recursive ? "true" : "false") << '\n';
    return out;
}

ap::ArgumentParser<Arguments> createArgumentParser()
{
    using namespace std::string_literals;

    return ap::ArgumentParserBuilder<Arguments>()
        .add_optional(
            "wordlist_file"s,
            &Arguments::wordlist_file,
            "/usr/share/dirb/wordlists/common.txt"s,
            { "-w"s, "--wordlist" },
            "Wordlist containing words to try to find on server"s)
        .add_optional(
            "recursive_search"s,
            &Arguments::recursive,
            false,
            { "-r"s, "--recursive"s },
            "Recursive search"s)
        .add_positional(
            "base_url"s,
            &Arguments::base_url,
            ""s,
            "URL to the server to attempt to find files from"s)
        .build();
}

std::vector<std::string> getWordList(const std::string& file)
{
    auto words = std::vector<std::string>();
    std::ifstream fs(file);
    std::string str;

    while (std::getline(fs, str))
        words.push_back(str);
    
    return words;
}

bool statusCodeIndicatesExistance(int status_code)
{
    return status_code != 404 && status_code != 400;
}

std::vector<std::string> searchServer(const std::string& base_url, const std::vector<std::string>& wordlist)
{
    using namespace std::string_literals;
    std::vector<std::string> found = {};

    for (const auto& word : wordlist)
    {
        auto url = base_url + "/"s + word;
        auto fileResponse = cpr::Get(cpr::Url{url});
        if (statusCodeIndicatesExistance(fileResponse.status_code))
            std::cout << "\"" << url << "\"" << " - " << std::to_string(fileResponse.status_code) << std::endl;
        else
        {
            auto dirResponse = cpr::Get(cpr::Url{url + "/"s});
            if (statusCodeIndicatesExistance(dirResponse.status_code))
            {
                std::cout << "\"" << url << "\"" << " - " << std::to_string(dirResponse.status_code) << std::endl;
                found.push_back(url);
            }
        }
    }

    return found;
}

void recursiveSearch(const std::string& base_url, const std::vector<std::string>& wordlist)
{
    using namespace std::string_literals;
    auto hits = searchServer(base_url, wordlist);
    for (const auto& hit : hits)
        recursiveSearch(hit, wordlist);
}

int main(int argc, const char * argv[])
{
    auto parser = createArgumentParser();
    auto argresult = parser.parse(argc, argv);
    if (!argresult)
    {
        std::cerr << parser.usage(argv[0]) << std::endl;
        return 1;
    }

    auto args = argresult.value();

    auto words = getWordList(args.wordlist_file);

    std::cout << args << std::endl;
    std::cout << "wordlist size: " << words.size() << std::endl;
    
    if (args.recursive)
        recursiveSearch(args.base_url, words);
    else
        searchServer(args.base_url, words);
    return 0;
}
