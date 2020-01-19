#include <iostream>
#include <string>

#include "parameters/arguments.h"
#include "argparsing/argparsing.h"
#include "parameters/argparsing_module.h"

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
        .add_optional(
            "ignore_codes"s,
            &Arguments::ignore_codes,
            { 404u, 400u },
            { "-s"s, "--ignored-status-codes"s },
            "Status codes to ignore"s)
        .build();

    parser.register_module<ArgparsingModule>();
    return parser;
}
