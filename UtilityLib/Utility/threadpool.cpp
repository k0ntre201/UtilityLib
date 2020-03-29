#include "threadpool.hpp"

#include <iostream>

std::shared_ptr<UtilityLib::Threading::ThreadPool> UtilityLib::Threading::ThreadPoolSingleton::_instance = nullptr;

UtilityLib::Threading::ThreadPool::~ThreadPool()
{
	stop();
}

size_t UtilityLib::Threading::ThreadPool::getPoolSize()
{
	return _pool.size();
}

UtilityLib::Threading::ThreadPool::ThreadPool(size_t n)
{
	terminateThreads = false;
	for (size_t i = 0; i < n; ++i)
	{
		_pool.push_back(std::thread([=]() { loop(); }));
	}
}

void UtilityLib::Threading::ThreadPool::stop()
{
	terminateThreads = true;

	_notifier.notify_all();
	for (auto& p : _pool)
	{
		p.join();
	}
}

void UtilityLib::Threading::ThreadPool::loop()
{
	while (true)
	{
		std::unique_ptr<TaskBase> task;
		{
			std::unique_lock<std::mutex> lck(_queueLock);
			_notifier.wait(lck, [&]() -> bool {return (!_tasks.empty() || terminateThreads); });

			if (terminateThreads || _tasks.empty())
				break;

			task = std::move(_tasks.front());
			_tasks.pop();
		}
		(*task)();
	}
}
