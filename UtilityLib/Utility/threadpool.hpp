#ifndef UTILITYLIB_UTILITY_THREADPOOL_HPP
#define UTILITYLIB_UTILITY_THREADPOOL_HPP

#include <functional>
#include <future>
#include <thread>
#include <memory>
#include <mutex>
#include <vector>
#include <queue>

#include "nocopyable.hpp"


namespace UtilityLib::threading
{
	class ThreadPool : public nocopyable
	{
	public:
		ThreadPool(size_t n);

		// Add task to thread pool. Only invocable objects are allowed to be executed.
		template <typename F, typename... Args,
			std::enable_if_t<std::is_invocable_v<F&&, Args &&...>, int> = 0>
			auto addTask(F f, Args... args)
		{
			std::packaged_task<std::invoke_result_t<F, Args...>()> task([_f = std::move(f), largs = std::tuple(std::forward<Args>(args)...)]() mutable { return std::apply(std::move(_f), std::move(largs)); });
			auto fut = task.get_future();
			{
				std::unique_lock<std::mutex> lck(_queueLock);
				_tasks.emplace(std::unique_ptr<TaskBase>(new Task(std::move(task))));
			}
			_notifier.notify_all();

			return std::move(fut);
		}

		~ThreadPool();

		size_t getPoolSize();
	
	protected:
		void stop();
		void loop();

		// Helper struct for store generic tasks
		struct TaskBase
		{
			virtual ~TaskBase() {}
			virtual void operator()() = 0;
		};

		template <typename F>
		struct Task : TaskBase
		{
			Task(F&& f) :_f(std::forward<F>(f)) {}

			// Inherited via TaskBase
			virtual void operator()() override
			{
				_f();
			}
		private:
			F _f;
		};

	
	private:
		bool terminateThreads = false;

		std::vector<std::thread> _pool;
		std::mutex _queueLock;
		std::condition_variable _notifier;
		std::queue<std::unique_ptr<TaskBase>> _tasks;

	};

	class ThreadPoolSingleton : public ThreadPool
	{
	public:
		// Get instance of thread pool. Pool has size of hardwate cores as a default.
		static std::shared_ptr<ThreadPool> getInstance(size_t n = std::thread::hardware_concurrency() + 1)
		{
			if (_instance == nullptr)
				_instance = std::shared_ptr<ThreadPool>(new ThreadPool(n));

			return _instance;
		}
	private:
		ThreadPoolSingleton(size_t n):ThreadPool(n) {}
		static std::shared_ptr<ThreadPool> _instance;
	};


}

#endif // !UTILITYLIB_UTILITY_THREADPOOL_HPP

