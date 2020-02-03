#pragma once

#include "workqueue.h"

#include <chrono>
#include <memory>
#include <optional>
#include <thread>

template <typename TWorkItem, typename TResultItem, typename TWorkFunc>
class Worker
{
    private:
        using TWorkPool = std::shared_ptr<WorkQueue<TWorkItem>>;
        TWorkPool _work_pool;
        std::size_t _worker_id;
        TWorkFunc _func;

    public:
        Worker(TWorkPool work_pool, std::size_t worker_id, TWorkFunc func)
            : _work_pool(work_pool), _worker_id(worker_id), _func(func)
        {}

        std::optional<TResultItem> do_one()
        {
            using namespace std::chrono_literals;
            while (!_work_pool->is_empty() && !_work_pool->work_available(_worker_id))
                std::this_thread::sleep_for(200ms);

            if (!_work_pool->work_available(_worker_id))
                return {};

            auto item = _work_pool->take_item(_worker_id);
            return _func(item);
        }

        std::vector<TResultItem> do_all()
        {
            auto results = std::vector<TResultItem>{};

            while (!_work_pool->is_empty()) {
                auto result = do_one();
                if (result)
                    results.push_back(result.value());
            }

            return results;
        }
};

