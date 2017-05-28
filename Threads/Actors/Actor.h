#include <atomic>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <iostream>
#include <functional>
#include <mutex>
#include <thread>
#include <future>

using namespace std::chrono_literals;

class Actor
{
public:
	typedef std::function<void()> WorkFunction;

	Actor()
	: _pingCount(0),
	  _stop(false),
	  _worker([=]() { doWork(); })
	{}

	~Actor()
	{
		std::cout << "Destroying actor" << std::endl;
		send(nullptr);
		_worker.join();
		std::cout << "Destruction complete" << std::endl;
	}

	bool isRunning()
	{
		return !_stop;
	}

	std::future<int> stop()
	{
		auto p = std::make_shared<std::promise<int>>();
		auto fut = p->get_future();
		send([=]() {
			std::cout << "Explicit stop in thread " << std::this_thread::get_id() << std::endl;
			_stop = true;
			p->set_value(1);
		});
		return fut;
	}

	std::future<int> ping()
	{
		auto p = std::make_shared<std::promise<int>>();
		auto fut = p->get_future();
		send([=]() {
			std::cout << "Ping " << ++_pingCount << " in thread " << std::this_thread::get_id() << std::endl;
			std::this_thread::sleep_for(1s);
			p->set_value(_pingCount);
		});
		return fut;
	}

private:
	void send(WorkFunction w)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_workQueue.push_back(w);
		_signal.notify_one();
	}

	// This is the function that runs in the worker thread
	void doWork()
	{
		std::cout << "Start thread " << std::this_thread::get_id() << std::endl;
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
		std::cout << "Exit thread " << std::this_thread::get_id() << std::endl;
	}

	std::deque<WorkFunction> _workQueue;

	int				 _pingCount;
	std::atomic_bool _stop;
	std::mutex 		_mutex;
	std::condition_variable _signal;
	std::thread 	_worker;
};
