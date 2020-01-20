#include <iostream>
#include <string>

#include "parameters/arguments.h"
#include "argparsing/argparsing.h"
#include "parameters/parameter_conversions.h"

std::ostream& operator<<(std::ostream& out, const Arguments& args)
{
    out << "base_url: " << args.base_url << '\n';
    out << "wordlist_file: " << args.wordlist_file << '\n';
    out << "recursive: " << (args.recursive ? "true" : "false") << '\n';
    out << "ignore_ssl_errors: " << (args.ignore_ssl_errors ? "true" : "false") << '\n';
    out << "ignored_status_codes: ";
    for (auto& code : args.ignore_codes)
        out << std::to_string(code) << ' ';
    out << '\n';
    out << "request_templates: ";
    for (auto& request_template : args.request_templates)
        out << '"' << request_template << "\" ";
    out << '\n';
    return out;
}

ap::ArgumentParser<Arguments> createArgumentParser()
{
    using namespace std::string_literals;

    auto parser = ap::ArgumentParserBuilder<Arguments>()
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
            "request_templates"s,
            &Arguments::request_templates,
            { "{BASE_URL}/{WORD}" },
            { "-t"s, "--request-templates"s },
            "Template defining how to construct URLs to fetch"s,
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
            "Status codes to ignore"s,
            status_code_parser_factory)
        .build();

    return parser;
}
