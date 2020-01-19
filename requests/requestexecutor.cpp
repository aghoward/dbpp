#include "requests/requestexecutor.h"

#include <algorithm>
#include <cstdio>
#include <memory>
#include <string>
#include <vector>

#include "fileio.h"
#include "parameters/arguments.h"
#include "multi-threading/threadpool.h"
#include "multi-threading/workqueue.h"
#include "requests/executioncontext.h"
#include "requests/requestfactory.h"

bool RequestExecutor::status_code_indicates_existance(const int status_code) const
{
    return std::find(
        _args.ignore_codes.begin(),
        _args.ignore_codes.end(),
        static_cast<uint16_t>(status_code))
            == _args.ignore_codes.end();
}

std::string RequestExecutor::execute(const std::string& item)
{
    using namespace std::string_literals;
    auto url = _context->base_url + "/"s + item;
    if (_context->extension != ""s)
        url += "."s + _context->extension;

    _context->logger.log("Trying: \""s + url + "\"\r");
    auto resp = _context->request_factory.make_request(url);

    if (status_code_indicates_existance(resp.status_code)) {
        _context->logger.log_line("\""s + url + "\" - "s + std::to_string(resp.status_code));
    }

    _context->logger.log("Trying: \""s + url + "\"\r");
    auto dir_resp = _context->request_factory.make_request(url + "/"s);

    if (status_code_indicates_existance(dir_resp.status_code)) {
        _context->logger.log_line("\""s + url + "/\" - "s + std::to_string(dir_resp.status_code));
        return url;
    }

    return ""s;
}

std::shared_ptr<WorkQueue<std::string, 10>> RequestExecutor::create_work_queue() const
{
    auto word_list = get_word_list(_args.wordlist_file);
    auto work_pool = std::make_shared<WorkQueue<std::string, 10>>();
    work_pool->add_items(word_list.begin(), word_list.end());
    return work_pool;
}

std::vector<std::string> RequestExecutor::search(const std::string& base_url)
{
    using namespace std::string_literals;

    _context = std::make_shared<ExecutionContext>(base_url, _args.extension, _request_factory, _args.ignore_codes);
    auto work_pool = create_work_queue();
    auto thread_pool = ThreadPool([&](const std::string& item) { return execute(item); }, work_pool);

    auto pool_results = thread_pool.execute();

    auto directory_results = std::vector<std::string>{};
    std::copy_if(pool_results.begin(), pool_results.end(), std::back_inserter(directory_results),
        [] (const auto& item) { return item != ""s; });

    _context->logger.log_line();

    return directory_results;
}

void RequestExecutor::search()
{
    search(_args.base_url);
}

void RequestExecutor::recursive_search()
{
    auto directories = search(_args.base_url);
    while (!directories.empty()) {
        auto dir = directories.back();
        directories.pop_back();
        auto results = search(dir);
        std::copy(results.begin(), results.end(), std::back_inserter(directories));
    }
}
