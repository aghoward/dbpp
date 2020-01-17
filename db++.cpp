#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "argparsing/argparsing.h"
#include "logger.h"
#include "requestfactory.h"
#include "threadpool.h"
#include "workqueue.h"

struct Arguments
{
    std::string base_url;
    std::string extension;
    std::string wordlist_file;
    bool recursive;
    std::string username;
    std::string password;
    bool ignore_ssl_errors;
};

std::ostream& operator<<(std::ostream& out, const Arguments& args)
{
    out << "base_url: " << args.base_url << '\n';
    out << "wordlist_file: " << args.wordlist_file << '\n';
    out << "recursive: " << (args.recursive ? "true" : "false") << '\n';
    out << "ignore_ssl_errors: " << (args.ignore_ssl_errors ? "true" : "false") << '\n';
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
        .add_optional(
            "username"s,
            &Arguments::username,
            ""s,
            { "-u"s, "--username"s },
            "Username for basic authentication"s)
        .add_optional(
            "password"s,
            &Arguments::password,
            ""s,
            { "-p"s, "--password"s },
            "Password for basic authentication"s)
        .add_optional(
            "extension"s,
            &Arguments::extension,
            ""s,
            { "-e"s, "--extension"s },
            "File extension to add to all guesses"s)
        .add_optional(
            "ignore_ssl_errors"s,
            &Arguments::ignore_ssl_errors,
            false,
            { "-k"s, "--ignore-ssl-errors"s },
            "Ignore SSL errors"s)
        .build();
}

std::vector<std::string> get_word_list(const std::string& file)
{
    auto words = std::vector<std::string>();
    std::ifstream fs(file);
    std::string str;

    while (std::getline(fs, str))
        words.push_back(str);
    
    return words;
}

struct ExecutionContext
{
    std::string base_url;
    std::string extension;

    RequestFactory request_factory;
    Logger logger;

    ExecutionContext(const std::string& url, const std::string& ext, const RequestFactory& rf)
        : base_url(url), extension(ext), request_factory(rf), logger()
    {}
};

bool statusCodeIndicatesExistance(int status_code)
{
    return status_code != 404 && status_code != 400;
}

std::string execute(std::shared_ptr<ExecutionContext> context, const std::string& item)
{
    using namespace std::string_literals;
    auto url = context->base_url + "/"s + item;
    if (context->extension != ""s)
        url += "."s + context->extension;

    context->logger.log("Trying: \""s + url + "\"\r");
    auto resp = context->request_factory.make_request(url);

    if (statusCodeIndicatesExistance(resp.status_code)) {
        context->logger.log_line("\""s + url + "\" - "s + std::to_string(resp.status_code));
    }

    context->logger.log("Trying: \""s + url + "\"\r");
    auto dir_resp = context->request_factory.make_request(url + "/"s);

    if (statusCodeIndicatesExistance(dir_resp.status_code)) {
        context->logger.log_line("\""s + url + "/\" - "s + std::to_string(dir_resp.status_code));
        return url;
    }

    return ""s;
}

std::shared_ptr<WorkQueue<std::string, 10>> create_work_queue(const std::string& wordlist_file)
{
    auto word_list = get_word_list(wordlist_file);
    auto work_pool = std::make_shared<WorkQueue<std::string, 10>>();
    work_pool->add_items(word_list.begin(), word_list.end());
    return work_pool;
}

std::vector<std::string> searchServer(const Arguments& args, const std::string& base_url)
{
    using namespace std::string_literals;

    auto request_factory = RequestFactory(args.username, args.password, !args.ignore_ssl_errors);
    auto context = std::make_shared<ExecutionContext>(base_url, args.extension, request_factory);

    auto work_pool = create_work_queue(args.wordlist_file);

    auto thread_pool = ThreadPool([&](const std::string& item) { return execute(context, item); }, work_pool);
    auto pool_results = thread_pool.execute();

    auto directory_results = std::vector<std::string>{};
    std::copy_if(pool_results.begin(), pool_results.end(), std::back_inserter(directory_results),
        [] (const auto& item) { return item != ""s; });

    return directory_results;
}

void recursive_search_server(const Arguments& args)
{
    auto directories = searchServer(args, args.base_url);
    while (!directories.empty()) {
        auto dir = directories.back();
        directories.pop_back();
        auto results = searchServer(args, dir);
        std::copy(results.begin(), results.end(), std::back_inserter(directories));
    }
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

    std::cout << args << std::endl;

    if (args.recursive)
        recursive_search_server(args);
    else
        searchServer(args, args.base_url);

    return 0;
}
