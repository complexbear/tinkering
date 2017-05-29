#include <atomic>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <iostream>
#include <functional>
#include <mutex>
#include <thread>
#include <future>

template<typename ResultType>
class Actor
{
public:
	typedef std::future<ResultType> FutureResult;

	Actor()
	: _stop(false),
	  _worker([=]() { doWork(); })
	{}

	~Actor()
	{
		send(nullptr);
		_worker.join();
	}

	bool isRunning()
	{
		return !_stop;
	}

	std::future<bool> stop()
	{
		auto p = std::make_shared<std::promise<bool>>();
		auto fut = p->get_future();
		send([=]() {
			_stop = true;
			p->set_value(true);
		});
		return fut;
	}

	
protected:
	typedef std::function<void()> WorkFunction;

	template<typename FUNC>
	std::future<ResultType> execute(const FUNC& func)
	{
		auto p = std::make_shared<std::promise<ResultType>>();
		auto fut = p->get_future();
		send([=]() {
			p->set_value(func());
		});
		return fut;
	}

	void send(WorkFunction w)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_workQueue.push_back(w);
		_signal.notify_one();
	}

	// This is the function that runs in the worker thread
	void doWork()
	{
		while(! _stop)
		{
			std::unique_lock<std::mutex> lock(_mutex);
			while (_workQueue.empty()) {
				_signal.wait(lock);
			}

			auto workItem = _workQueue.front();
			if(workItem == nullptr)
			{
				_stop = true;
			}
			else
			{
				_workQueue.pop_front();
				workItem();
			}
		}
	}

	std::deque<WorkFunction> _workQueue;

	std::atomic_bool _stop;
	std::mutex 		_mutex;
	std::condition_variable _signal;
	std::thread 	_worker;
};
