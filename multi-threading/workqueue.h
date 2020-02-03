#pragma once

#include <algorithm>
#include <cstddef>
#include <list>
#include <type_traits>
#include <vector>

template <typename T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

template <typename TWorkItem>
class WorkQueue
{
    private:
        using TWorkPool = std::vector<std::list<TWorkItem>>;
        TWorkPool _work_pool;
        typename TWorkPool::iterator _next;

    public:
        explicit WorkQueue(std::size_t queue_size)
            : _work_pool(queue_size), _next(_work_pool.begin())
        {}

        template <typename T, typename = std::enable_if<std::is_same_v<remove_cvref_t<T>, remove_cvref_t<TWorkItem>>>>
        void add_item(T&& item)
        {
            _next->push_back(item);
            if (++_next == _work_pool.end())
                _next = _work_pool.begin();
        }

        template <typename TIter>
        void add_items(TIter begin, TIter end)
        {
            std::for_each(begin, end, [&] (auto&& item) { this->add_item(item); });
        }
        
        TWorkItem take_item(std::size_t worker_id)
        {
            auto item = _work_pool[worker_id].front();
            _work_pool[worker_id].pop_front();
            return item;
        }

        bool work_available(std::size_t worker_id) const
        {
            return !_work_pool[worker_id].empty();
        }

        bool is_empty() const
        {
            return std::all_of(_work_pool.begin(), _work_pool.end(), [] (auto&& items) { return items.empty(); });
        }

        std::size_t pool_size() const
        {
            return _work_pool.size();
        }
};

