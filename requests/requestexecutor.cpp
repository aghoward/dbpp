#include "requests/requestexecutor.h"

#include <algorithm>
#include <cstdio>
#include <memory>
#include <optional>
#include <string>
#include <vector>

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

std::optional<std::string> RequestExecutor::execute(const std::string& item, const std::string& request_template)
{
    using namespace std::string_literals;
    auto url = format_template(request_template, {{"{WORD}"s, item}, {"{BASE_URL}"s, _context->base_url}});

    _context->logger.log("Trying: \""s + url + "\"\r");
    auto response = _context->request_factory.make_request(url);
    if (status_code_indicates_existance(response.status_code))
    {
        _context->logger.log_line("\""s + url + "\" - "s + std::to_string(response.status_code));
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

std::shared_ptr<WorkQueue<std::string, 10>> RequestExecutor::create_work_queue() const
{
    auto word_list = get_word_list(_args.wordlist_file);
    auto work_pool = std::make_shared<WorkQueue<std::string, 10>>();
    work_pool->add_items(word_list.begin(), word_list.end());
    return work_pool;
}

std::vector<std::string> RequestExecutor::search(const std::vector<std::string>& request_templates)
{
    using namespace std::string_literals;

    _context = std::make_shared<ExecutionContext>(_args.base_url, request_templates, _request_factory, _args.ignore_codes);
    auto work_pool = create_work_queue();
    auto thread_pool = ThreadPool([&](const std::string& item) { return execute(item); }, work_pool);

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
