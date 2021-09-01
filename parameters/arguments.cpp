#include <iostream>
#include <string>

#include "parameters/arguments.h"
#include "argparsing/argparsing.h"
#include "parameters/parameter_conversions.h"

std::ostream& operator<<(std::ostream& out, const Arguments& args)
{
    out << "base_url: " << args.base_url << '\n';
    out << "request_method: " << args.request_method << '\n';
    out << "thread_count: " << std::to_string(args.thread_count) << '\n';

    out << "wordlist_file: " << args.wordlist_file << '\n';

    out << "recursive: " << (args.recursive ? "true" : "false") << '\n';

    out << "ignore_ssl_errors: " << (args.ignore_ssl_errors ? "true" : "false") << '\n';

    out << "ignored_status_codes: ";
    for (auto& code : args.ignore_codes)
        out << std::to_string(code) << ' ';
    out << '\n';

    out << "ignored_content_lengths: ";
    for (auto& cl : args.ignore_content_lengths)
        out << std::to_string(cl) << ' ';
    out << '\n';

    out << "request_templates: ";
    for (auto& request_template : args.request_templates)
        out << '"' << request_template << "\" ";
    out << '\n';

    out << "headers: \n";
    for (auto& header : args.headers)
        out << "\t'" << header.name << "' = '" << header.value << "'" << '\n';
    out << '\n';
    return out;
}

ap::ArgumentParser<Arguments> createArgumentParser()
{
    using namespace std::string_literals;

    auto parser = ap::ArgumentParserBuilder<Arguments>()
        .add_optional(
            "help"s,
            &Arguments::help,
            false,
            { "-h"s, "--help"s },
            "Print this help message and exit"s,
            true)
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
            "Recurse all found items as if they are directories"s)
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
            "request_templates"s,
            &Arguments::request_templates,
            { "{BASE_URL}/{WORD}" },
            { "-t"s, "--request-templates"s },
            "Template defining how to construct URLs to fetch. Default: {BASE_URL}/{WORD}"s,
            request_template_parser_factory)
        .add_optional(
            "ignore_ssl_errors"s,
            &Arguments::ignore_ssl_errors,
            false,
            { "-k"s, "--ignore-ssl-errors"s },
            "Ignore SSL errors"s)
        .add_optional(
            "ignore_codes"s,
            &Arguments::ignore_codes,
            { 404u, 400u },
            { "-s"s, "--ignored-status-codes"s },
            "Ignore any responses having the indicated status codes. Default: 404,400"s,
            status_code_parser_factory)
        .add_optional(
            "ignore_content_lengths"s,
            &Arguments::ignore_content_lengths,
            { },
            { "-c"s, "--ignored-content-lengths"s },
            "Ignore any responses having the indicated content lengths. Default: <none>"s,
            content_length_parser_factory)
        .add_optional(
            "request_method"s,
            &Arguments::request_method,
            RequestMethod::DEFAULT,
            { "-X"s, "--request-method"s },
            "HTTP method to use when making requests. Possible values are HEAD, POST, PUT, PATCH, GET. Default: HEAD"s,
            request_method_parser_factory)
        .add_optional(
            "thread_count"s,
            &Arguments::thread_count,
            10u,
            { "-T"s, "--thread-count"s },
            "Number of threads to use for making requests. Default: 10"s)
        .add_optional(
            "request_data"s,
            &Arguments::request_body,
            ""s,
            { "-d"s, "--data"s },
            "Body of the post request. Changes request_method to POST if not otherwise provided"s)
        .add_optional(
            "content_type"s,
            &Arguments::content_type,
            "x-www-form-urlencoded"s,
            { "-D"s, "--content-type"s },
            "HTTP content type of the <request_data>. Default: x-www-form-urlencoded when data is provided"s)
        .add_optional(
            "headers"s,
            &Arguments::headers,
            { },
            { "-H"s, "--headers"s },
            "Headers to add to request. Default <none>"s,
            header_parser_factory)
        .add_positional(
            "base_url"s,
            &Arguments::base_url,
            ""s,
            "URL to the server to attempt to query"s)
        .build();

    return parser;
}
