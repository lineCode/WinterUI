#pragma once

#include "api.hh"

#include <atomic>
#include <algorithm>
#include <thread>
#include <functional>
#include <future>
#include <queue>

/// Managing a group of reusable threads that run tasks

namespace Beast
{
	namespace Util
	{
		/// A thread pool using templated tasks
		/// To spin off an asynchronous thread, call enqueue(function, args)
		/// To spin off a blocking thread, make an std::future<return type> variable to hold enqueue's return and call .get() on it before the result of the thread needs to be used
		
		struct ThreadPool
		{
			BEAST_API ThreadPool(size_t poolSize = std::max(std::thread::hardware_concurrency() - 1u, 1u));
			BEAST_API ~ThreadPool();
			
			template <typename F, typename... Args> auto enqueue(F func, Args ... args) //Note: thread pool can't accept moved args because of buggy type deduction in C++
			{
				auto invokeBinding = std::bind(std::forward<F>(func), std::forward<Args>(args)...);
				using invokePkg = std::packaged_task<std::invoke_result_t<decltype(invokeBinding)>()>;
				invokePkg pkg {std::move(invokeBinding)};
				auto future = pkg.get_future();
				this->queueMutex.lock();
				this->taskQueue.push(std::make_unique<Task<invokePkg>>(std::move(pkg)));
				this->queueMutex.unlock();
				this->queueCV.notify_one();
				return future;
			}
			
			private:
			std::atomic_bool runSem {true};
			std::condition_variable queueCV;
			std::mutex queueMutex, cvMutex;
			std::vector<std::thread> threads;
			
			struct TaskBase
			{
				BEAST_API virtual void execute() = 0;
			};
			
			template <typename P> struct Task : public TaskBase
			{
				P pkg;
				Task(P &&pkg) : pkg {std::forward<P &&>(pkg) }{}
				virtual void execute() override {pkg();}
			};
			
			std::queue<std::unique_ptr<TaskBase>> taskQueue;
			
			BEAST_API void threadRun();
		};
	}
}
