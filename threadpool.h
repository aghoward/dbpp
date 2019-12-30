#pragma once

#include <array>
#include <cstddef>
#include <future>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

#include "worker.h"
#include "workqueue.h"

template <std::size_t TPoolSize, typename TWorkItem, typename TWorkFunc, typename TResultItem = std::invoke_result_t<TWorkFunc, TWorkItem>>
class ThreadPool
{
    private:
        using TWorkPool = std::shared_ptr<WorkQueue<TWorkItem, TPoolSize>>;
        using TPool = std::array<Worker<TPoolSize, TWorkItem, TResultItem, TWorkFunc>, TPoolSize>;
        
        TWorkPool _work_pool;
        TPool _workers;

        template <std::size_t ... Indices>
        TPool build_pool_h(TWorkFunc func, std::index_sequence<Indices...>) const
        {
            return { Worker<TPoolSize, TWorkItem, TResultItem, TWorkFunc>(_work_pool, Indices, func) ... };
        }

        TPool build_pool(TWorkFunc func) const
        {
            return build_pool_h(func, std::make_index_sequence<TPoolSize>{});
        }

    public:
        ThreadPool(TWorkFunc func, TWorkPool work_pool)
            : _work_pool(work_pool), _workers(build_pool(func))
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
