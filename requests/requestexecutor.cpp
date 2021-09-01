#include "requests/requestexecutor.h"

#include <algorithm>
#include <cstdio>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <cpr/cpr.h>

#include "parameters/arguments.h"
#include "multi-threading/threadpool.h"
#include "multi-threading/workqueue.h"
#include "requests/executioncontext.h"
#include "requests/requestfactory.h"
#include "support/fileio.h"
#include "support/template_formatting.h"

bool RequestExecutor::status_code_indicates_existance(const int status_code) const
{
    return std::find(
        _args.ignore_codes.begin(),
        _args.ignore_codes.end(),
        static_cast<uint16_t>(status_code))
            == _args.ignore_codes.end();
}

bool RequestExecutor::passes_content_length_check(const uint32_t content_length) const
{
    return std::find(
        _args.ignore_content_lengths.begin(),
        _args.ignore_content_lengths.end(),
        static_cast<uint32_t>(content_length))
            == _args.ignore_content_lengths.end();
}

uint32_t get_content_length(cpr::Response& response)
{
    return std::atoi(response.header["Content-Length"].c_str());
}

bool RequestExecutor::response_passes_checks(cpr::Response& response) const
{
    return status_code_indicates_existance(response.status_code) &&
        passes_content_length_check(get_content_length(response));
}

cpr::Response RequestExecutor::get_response(
        const std::string& url,
        const std::string& data,
        const std::map<std::string, std::string>& templates)
{
    using namespace std::string_literals;

    if (data == ""s)
        _context->logger.log("Trying: \""s + url + "\"\r"s);
    else
        _context->logger.log("Trying: \""s + url + "\" - \"" + data + "\"\r"s);
    return _context->request_factory.make_request(url, data, templates);
}

std::optional<std::string> RequestExecutor::execute(const std::string& item, const std::string& request_template)
{
    using namespace std::string_literals;
    auto templates = std::map<std::string, std::string>{{"{WORD}"s, item}, {"{BASE_URL}"s, _context->base_url}};
    auto url = format_template(request_template, templates);
    auto data = format_template(_context->request_data, templates);

    auto response = get_response(url, data, templates);

    if (response_passes_checks(response))
    {
        auto message_addendum = (data != ""s) ? "\" - \""s + data : ""s;
        _context->logger.log_line(
            "\""s
            + url
            + message_addendum
            + "\" - "s
            + std::to_string(get_content_length(response))
            + " (CL) - "s
            + std::to_string(response.status_code)
            + " (S)"s);
        return url;
    }

    return {};
}

std::vector<std::string> RequestExecutor::execute(const std::string& item)
{
    auto results = std::vector<std::string>();
    for (const auto& request_template : _context->request_templates)
    {
        auto result = execute(item, request_template);
        if (result)
            results.push_back(result.value());
    }

    return results;
}

std::shared_ptr<WorkQueue<std::string>> RequestExecutor::create_work_queue(std::size_t queue_size) const
{
    auto word_list = get_word_list(_args.wordlist_file);
    auto work_pool = std::make_shared<WorkQueue<std::string>>(queue_size);
    work_pool->add_items(word_list.begin(), word_list.end());
    return work_pool;
}

std::vector<std::string> RequestExecutor::search(const std::vector<std::string>& request_templates)
{
    using namespace std::string_literals;

    _context = std::make_shared<ExecutionContext>(_args.base_url, request_templates, _request_factory, _args.ignore_codes, _args.request_body);
    auto work_pool = create_work_queue(_args.thread_count);
    std::function work_func = [&](const std::string& item) { return execute(item); };
    auto thread_pool = ThreadPool(work_func, work_pool);

    auto pool_results = thread_pool.execute();

    auto found_items = std::vector<std::string>{};
    for (const auto& pool_result : pool_results)
        found_items.insert(found_items.end(), pool_result.begin(), pool_result.end());

    _context->logger.log_line();

    return found_items;
}

void RequestExecutor::search()
{
    search(_args.request_templates);
}

void RequestExecutor::recursive_search()
{
    using namespace std::string_literals;

    auto urls = search(_args.request_templates);
    while (!urls.empty()) {
        auto url = urls.back();
        urls.pop_back();

        auto new_template = url + "/{WORD}"s;
        auto results = search({new_template});
        urls.insert(urls.end(), results.begin(), results.end());
    }
}
