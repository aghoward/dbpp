#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <cpr/cpr.h>

#include "parameters/arguments.h"
#include "requests/executioncontext.h"
#include "requests/requestfactory.h"
#include "multi-threading/workqueue.h"

class RequestExecutor {
    private:
        std::shared_ptr<ExecutionContext> _context;
        RequestFactory _request_factory;
        Arguments _args;

        bool status_code_indicates_existance(const int status_code) const;
        bool passes_content_length_check(const uint32_t content_length) const;
        bool response_passes_checks(cpr::Response& response) const;

        std::shared_ptr<WorkQueue<std::string>> create_work_queue(std::size_t queue_size) const;
        cpr::Response get_response(const std::string& url, const std::string& data);
        std::optional<std::string> execute(const std::string& item, const std::string& request_template);
        std::vector<std::string> execute(const std::string& item);
        std::vector<std::string> search(const std::vector<std::string>& request_templates);

    public:
        RequestExecutor(const Arguments& args)
            : _context(nullptr),
            _request_factory(
                args.username,
                args.password,
                !args.ignore_ssl_errors,
                args.request_method,
                args.content_type,
                args.headers),
            _args(args)
        {}

        void search();
        void recursive_search();
};

