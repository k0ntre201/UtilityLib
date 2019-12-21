#include "threadpool.hpp"

#include <iostream>

std::shared_ptr<UtilityLib::threading::ThreadPool> UtilityLib::threading::ThreadPoolSingleton::_instance = nullptr;

UtilityLib::threading::ThreadPool::~ThreadPool()
{
	std::cout << "Destroy thread pool\n";
	stop();
}

size_t UtilityLib::threading::ThreadPool::getPoolSize()
{
	return _pool.size();
}

UtilityLib::threading::ThreadPool::ThreadPool(size_t n)
{
	std::cout << "Create thread pool\n";
	terminateThreads = false;
	for (size_t i = 0; i < n; ++i)
	{
		_pool.push_back(std::thread([=]() { loop(); }));
	}
}

void UtilityLib::threading::ThreadPool::stop()
{
	terminateThreads = true;

	_notifier.notify_all();
	for (auto& p : _pool)
	{
		p.join();
	}
}

void UtilityLib::threading::ThreadPool::loop()
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
		//std::cout << "Thread id:"<< std::this_thread::get_id()<<"\n";
	}
}
