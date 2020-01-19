#pragma once

#include <memory>
#include <string>

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
        std::shared_ptr<WorkQueue<std::string, 10>> create_work_queue() const;
        std::string execute(const std::string& item);
        std::vector<std::string> search(const std::string& base_url);

    public:
        RequestExecutor(const Arguments& args)
            : _context(nullptr),
            _request_factory(args.username, args.password, args.ignore_ssl_errors),
            _args(args)
        {}

        void search();
        void recursive_search();
};

