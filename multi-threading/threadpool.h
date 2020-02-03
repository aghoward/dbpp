#pragma once

#include <cstddef>
#include <future>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

#include "worker.h"
#include "workqueue.h"

template <typename TWorkItem, typename TWorkFunc, typename TResultItem = std::invoke_result_t<TWorkFunc, TWorkItem>>
class ThreadPool
{
    private:
        using TWorkPool = std::shared_ptr<WorkQueue<TWorkItem>>;
        using TPool = std::vector<Worker<TWorkItem, TResultItem, TWorkFunc>>;
        
        TWorkPool _work_pool;
        TPool _workers;

        TPool build_pool(TWorkFunc func, std::size_t pool_size) const
        {
            auto pool = TPool();
            for (auto i = 0u; i < pool_size; i++)
                pool.push_back(Worker<TWorkItem, TResultItem, TWorkFunc>(_work_pool, i, func));
            return pool;
        }

    public:
        ThreadPool(TWorkFunc func, TWorkPool work_pool)
            : _work_pool(work_pool), _workers(build_pool(func, _work_pool->pool_size()))
        {}

        std::vector<TResultItem> execute()
        {
            auto futures = std::vector<std::future<std::vector<TResultItem>>>{};
            auto results = std::vector<TResultItem>{};
            for (auto& worker : _workers)
                futures.push_back(std::async(std::launch::async, [&]() -> std::vector<TResultItem> { return worker.do_all(); }));
            std::for_each(
                futures.begin(),
                futures.end(),
                [&] (auto&& f)
                {
                    f.wait();
                    auto task_result = f.get();
                    results.reserve(results.size() + task_result.size());
                    results.insert(results.end(), task_result.begin(), task_result.end());
                });

            return results;
        }
};
